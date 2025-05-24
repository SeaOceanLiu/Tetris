#ifndef BlockGroupH
#define BlockGroupH

#include "nlohmann/json.hpp"
#include "BrickPool.h"

using json = nlohmann::json;

enum class BlockType: int{
    NORMAL,
    PIERCE,
    BOMB
};
enum class RotateAngle: int{
    DEGREE0,
    DEGREE90,
    DEGREE180,
    DEGREE270
};
class EnumHelper {
public:
    static RotateAngle rotate(RotateAngle r) {
        return static_cast<RotateAngle>((static_cast<int>(r) + 1) % 4);
    }
};

class BlockBodyInfo
{
public:
    int m_leftCol;
    int m_rightCol;
    int m_topRow;
    int m_bottomRow;
    vector<vector<int>> body;
    BlockBodyInfo(): m_leftCol(0), m_rightCol(0), m_topRow(0), m_bottomRow(0){}
};

class SingleGroup
{
public:
    int m_groupId;
    BlockType m_type;
    bool m_canRotate;
    RotateAngle m_currentAngle;
    unordered_map<RotateAngle, shared_ptr<BlockBodyInfo>>body;

    SingleGroup(int groupId, BlockType type, bool canRotate):
        m_groupId(groupId),
        m_type(type),
        m_canRotate(canRotate),
        m_currentAngle(RotateAngle::DEGREE0)
    {
    }

    RotateAngle rotated(RotateAngle angle) { return EnumHelper::rotate(angle);};
    BlockType getType(void){return m_type;};
    shared_ptr<BlockBodyInfo> getBodyInfo(RotateAngle angle){ return m_canRotate?body[angle]:body[RotateAngle::DEGREE0]; };
    shared_ptr<BlockBodyInfo> getRotated(RotateAngle angle) { return m_canRotate?body[EnumHelper::rotate(angle)]:body[RotateAngle::DEGREE0];};
};

class BlockGroup: public ControlImpl
{
private:
    char *m_pJsonFileContent; // json file content
    json m_jsonBLockGroup; // json object

    int m_iRowCount;
    int m_iColCount;

    void generateRoteateBody(shared_ptr<BlockBodyInfo> sourceBodyInfo, shared_ptr<BlockBodyInfo> targetBodyInfo);
public:
    unordered_map<int, shared_ptr<SingleGroup>> m_blockGroups;  // map from groupId->SingleGroup

    BlockGroup(Control *parent, fs::path pathPrefix);
    ~BlockGroup();

    int getColCount(void);
    int getRowCount(void);
    int getBlockCount(void);
};

#endif // BlockGroupH