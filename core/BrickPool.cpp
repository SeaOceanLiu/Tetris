#include "BrickPool.h"

BrickPool::BrickPool(Control *parent, fs::path pathPrefix, float xScale, float yScale):
    ControlImpl(parent, xScale, yScale),
    m_minBrickIdx(0),
    m_maxBrickIdx(0)
{
    m_bricks.push_back(LuotiAniBuilder(this)
                                    .loadAniDesc(ResourceLoader::RID_cyanBlock_jsonc)
                                    .prepare()
                                    .setRect({0, 0, ConstDef::SINGLE_BLOCK_SIZE.width, ConstDef::SINGLE_BLOCK_SIZE.height})
                                    .build());
    m_bricks.push_back(LuotiAniBuilder(this)
                                    .loadAniDesc(ResourceLoader::RID_darkGreenBlock_jsonc)
                                    .prepare()
                                    .setRect({0, 0, ConstDef::SINGLE_BLOCK_SIZE.width, ConstDef::SINGLE_BLOCK_SIZE.height})
                                    .build());
    m_bricks.push_back(LuotiAniBuilder(this)
                                    .loadAniDesc(ResourceLoader::RID_deepBlueBlock_jsonc)
                                    .prepare()
                                    .setRect({0, 0, ConstDef::SINGLE_BLOCK_SIZE.width, ConstDef::SINGLE_BLOCK_SIZE.height})
                                    .build());
    m_bricks.push_back(LuotiAniBuilder(this)
                                    .loadAniDesc(ResourceLoader::RID_greenBlock_jsonc)
                                    .prepare()
                                    .setRect({0, 0, ConstDef::SINGLE_BLOCK_SIZE.width, ConstDef::SINGLE_BLOCK_SIZE.height})
                                    .build());
    m_bricks.push_back(LuotiAniBuilder(this)
                                    .loadAniDesc(ResourceLoader::RID_purpleBlock_jsonc)
                                    .prepare()
                                    .setRect({0, 0, ConstDef::SINGLE_BLOCK_SIZE.width, ConstDef::SINGLE_BLOCK_SIZE.height})
                                    .build());
    m_bricks.push_back(LuotiAniBuilder(this)
                                    .loadAniDesc(ResourceLoader::RID_redBlock_jsonc)
                                    .prepare()
                                    .setRect({0, 0, ConstDef::SINGLE_BLOCK_SIZE.width, ConstDef::SINGLE_BLOCK_SIZE.height})
                                    .build());
    m_bricks.push_back(LuotiAniBuilder(this)
                                    .loadAniDesc(ResourceLoader::RID_grayBlock_jsonc)
                                    .prepare()
                                    .setRect({0, 0, ConstDef::SINGLE_BLOCK_SIZE.width, ConstDef::SINGLE_BLOCK_SIZE.height})
                                    .build());
    m_bricks.push_back(LuotiAniBuilder(this)
                                    .loadAniDesc(ResourceLoader::RID_yellowBlock_jsonc)
                                    .prepare()
                                    .setRect({0, 0, ConstDef::SINGLE_BLOCK_SIZE.width, ConstDef::SINGLE_BLOCK_SIZE.height})
                                    .build());
    // 固定第9个是穿透
    m_bricks.push_back(LuotiAniBuilder(this)
                                    .loadAniDesc(ResourceLoader::RID_pierceBlock_jsonc)
                                    .prepare()
                                    .setRect({0, 0, ConstDef::SINGLE_BLOCK_SIZE.width, ConstDef::SINGLE_BLOCK_SIZE.height})
                                    .setLoop(true)
                                    .setAutoStart()
                                    .build());
    // 固定第10个是炸弹
    m_bricks.push_back(LuotiAniBuilder(this)
                                    .loadAniDesc(ResourceLoader::RID_bombBlock_jsonc)
                                    .prepare()
                                    .setRect({0, 0, ConstDef::SINGLE_BLOCK_SIZE.width, ConstDef::SINGLE_BLOCK_SIZE.height})
                                    .setLoop(true)
                                    .setAutoStart()
                                    .build());

    m_minBrickIdx = 0;
    m_maxBrickIdx = 7;

    hide();
}

BrickPool::~BrickPool()
{

}

shared_ptr<LuotiInstance> BrickPool::getBrick(int brickIdx){
    if (brickIdx >= 0 && brickIdx < 10){
        return make_shared<LuotiInstance>(this, m_bricks[brickIdx], brickIdx);
    } else {
        SDL_Log("Invalid brick index: %d", brickIdx);
        return nullptr;
    }
}

void BrickPool::draw(float x, float y, int brickIdx, Uint8 alpha){
    inheritRenderer();

    if (brickIdx >= 0 && brickIdx < m_bricks.size()){
        shared_ptr<LuotiAni> brick = m_bricks[brickIdx];
        brick->draw(x, y, alpha);
    } else {
        SDL_Log("Invalid brick index: %d", brickIdx);
    }
}

void BrickPool::draw(SPoint pos, int brickIdx, Uint8 alpha){
    draw(pos.x, pos.y, brickIdx, alpha);
}

void BrickPool::update(void){
    for (auto &brick : m_bricks){
        brick->update();
    }
}

int BrickPool::getBrickCount(void){
    return (int)m_bricks.size();
}

int BrickPool::getMinBrickIdx(void){
    return m_minBrickIdx;
}

int BrickPool::getMaxBrickIdx(void){
    return m_maxBrickIdx;
}