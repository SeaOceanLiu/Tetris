#include "BlockGroup.h"

BlockGroup::BlockGroup(Control *parent, fs::path pathPrefix):
    ControlImpl(parent),
    m_pJsonFileContent(nullptr),
    m_iRowCount(0),
    m_iColCount(0)
{
    fs::path filePath = pathPrefix / "config" / "RBlock.jsonc";
    SDL_Log("Open json file: %s", filePath.string().c_str());
    SDL_IOStream *jsonFIleStream = SDL_IOFromFile(filePath.string().c_str(), "r");
    if (jsonFIleStream == nullptr) {
        SDL_Log("Open json file error: %s", SDL_GetError());
        throw "Open xml file error";
    }

    size_t iFileLen = SDL_GetIOSize(jsonFIleStream);
    if (iFileLen <= 0) {
        SDL_Log("Get xml file size error: %s", SDL_GetError());
        throw "Get xml file size error";
    }
    m_pJsonFileContent = new char[iFileLen + 1];
    m_pJsonFileContent[iFileLen] = '\0';
    if (SDL_ReadIO(jsonFIleStream, (void *)m_pJsonFileContent, iFileLen) != iFileLen) {
        SDL_Log("Read xml file content error: %s", SDL_GetError());
        throw "Read xml file content error";
    }
    m_jsonBLockGroup = json::parse(m_pJsonFileContent, nullptr, false, true);

    m_iColCount = m_jsonBLockGroup["BlockSize"].at("x").get<int>();
    m_iRowCount = m_jsonBLockGroup["BlockSize"].at("y").get<int>();

    for (const auto& blockData : m_jsonBLockGroup["BlockData"]) {
        int groupId = blockData.at("groupId").get<int>();
        SDL_Log("Reading BlockGroup: %d", groupId);

        BlockType type = blockData.at("type").get<BlockType>();
        bool canRotate = blockData.at("canRotate").get<bool>();
        auto newBlockGroup = make_shared<SingleGroup>(groupId, type, canRotate);

        int iLeftCol = m_iColCount;
        int iRightCol = 0;
        int iTopRow = 0;
        int iBottomRow = 0;

        auto degree0BodyInfo = make_shared<BlockBodyInfo>();
        degree0BodyInfo->m_leftCol = m_iColCount;
        degree0BodyInfo->m_rightCol = 0;
        degree0BodyInfo->m_topRow = 0;
        degree0BodyInfo->m_bottomRow = 0;
        vector<vector<int>> degree0Body;
        int row = 0;
        for (const auto& bodyRowDefine : blockData.at("define")) {
            vector<int> colData;
            for (int col = 0; col < bodyRowDefine.size(); col++) {
                int data = bodyRowDefine[col].get<int>();
                colData.push_back(data);

                if (data != 0) {
                    degree0BodyInfo->m_leftCol = min(degree0BodyInfo->m_leftCol, col);
                    degree0BodyInfo->m_rightCol = max(degree0BodyInfo->m_rightCol, col);
                    if (degree0BodyInfo->m_topRow == 0){
                        degree0BodyInfo->m_topRow = row;
                    }
                    degree0BodyInfo->m_bottomRow = row;
                }
            }
            degree0BodyInfo->body.push_back(colData);
            row++;
        }
        newBlockGroup->body[RotateAngle::DEGREE0] = degree0BodyInfo;

        if (canRotate) {
            auto degree90BodyInfo = make_shared<BlockBodyInfo>();
            generateRoteateBody(degree0BodyInfo, degree90BodyInfo);
            newBlockGroup->body[RotateAngle::DEGREE90] = degree90BodyInfo;

            auto degree180BodyInfo = make_shared<BlockBodyInfo>();
            generateRoteateBody(degree90BodyInfo, degree180BodyInfo);
            newBlockGroup->body[RotateAngle::DEGREE180] = degree180BodyInfo;

            auto degree270BodyInfo = make_shared<BlockBodyInfo>();
            generateRoteateBody(degree180BodyInfo, degree270BodyInfo);
            newBlockGroup->body[RotateAngle::DEGREE270] = degree270BodyInfo;
        }
        m_blockGroups[groupId] = newBlockGroup;
    }
    SDL_Log("Total loaded %zu block group", m_blockGroups.size());
}
BlockGroup::~BlockGroup() {
    if (m_pJsonFileContent != nullptr) {
        delete[] m_pJsonFileContent;
        m_pJsonFileContent = nullptr;
    }
}
void BlockGroup::generateRoteateBody(shared_ptr<BlockBodyInfo> sourceBodyInfo, shared_ptr<BlockBodyInfo> targetBodyInfo){
    /*
        1. 先将m_leftCol、m_rightCol、m_topRow、m_bottomRow计算出来
    */
    targetBodyInfo->m_leftCol = sourceBodyInfo->m_topRow;
    targetBodyInfo->m_topRow = m_iColCount - sourceBodyInfo->m_rightCol - 1;
    targetBodyInfo->m_rightCol = sourceBodyInfo->m_bottomRow;
    targetBodyInfo->m_bottomRow = m_iRowCount - sourceBodyInfo->m_leftCol - 1;
    /*
        2. 再直接逆时针旋转90度
        2.1 定义原始矩阵如下
        Source:
          j
          |
          V
    i-->{00 01 02 03 04}
        {10 11 12 13 14}
        {20 21 22 23 24}
        {30 31 32 33 34}
        {40 41 42 43 44}

        2.2 可以直接循环做旋转
    --->Target:
        {04 14 24 34 44}
        {03 13 23 33 43}
        {02 12 22 32 42}
        {01 11 21 31 41}
    j-->{00 10 20 30 40}
         ^
         |
         i

        2.3 另一种方法：
        2.3.1 先做转置：
    --->{00 10 20 30 40}
        {01 11 21 31 41}
        {02 12 22 32 42}
        {03 13 23 33 43}
        {04 14 24 34 44}
        2.3.2 再对每一列做反转：
    --->{04 14 24 34 44}
        {03 13 23 33 43}
        {02 12 22 32 42}
        {01 11 21 31 41}
        {00 10 20 30 40}

        3. 最后决定采用直接做循环做旋转
    */
    targetBodyInfo->body = sourceBodyInfo->body;
    for(int i = 0; i < sourceBodyInfo->body.size(); i++) {
        for (int j = 0; j < sourceBodyInfo->body[i].size(); j++) {
            targetBodyInfo->body[sourceBodyInfo->body[i].size() -1 - j][i] = sourceBodyInfo->body[i][j];
        }
    }
}

int BlockGroup::getBlockCount(void)
{
    return m_blockGroups.size();
}

int BlockGroup::getRowCount(void)
{
    return m_iRowCount;
}

int BlockGroup::getColCount(void)
{
    return m_iColCount;
}