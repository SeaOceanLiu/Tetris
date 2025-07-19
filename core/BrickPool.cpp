#include "BrickPool.h"

BrickPool::BrickPool(Control *parent, fs::path pathPrefix, float xScale, float yScale):
    ControlImpl(parent, xScale, yScale),
    m_minBrickIdx(0),
    m_maxBrickIdx(0)
{
    for(int brickIdx = 1; brickIdx < 10; brickIdx++){
        string brickResourceId = "images/bitmap" + to_string(brickIdx) + ".bmp";
        shared_ptr<Actor> brick = make_shared<Actor>(this, brickResourceId, false, xScale, yScale);
        brick->setAnchorPointTopLeft();
        m_bricks.push_back(brick);
    }

    m_minBrickIdx = 0;
    m_maxBrickIdx = 6;

    hide();
}

BrickPool::~BrickPool()
{

}

shared_ptr<Actor> BrickPool::getBrick(int brickIdx){
    if (brickIdx >= 0 && brickIdx < 10){
        return m_bricks[brickIdx];
    } else {
        SDL_Log("Invalid brick index: %d", brickIdx);
        return nullptr;
    }
}

void BrickPool::draw(float x, float y, int brickIdx, Uint8 alpha){
    inheritRenderer();

    if (brickIdx >= 0 && brickIdx < m_bricks.size()){
        shared_ptr<Actor> brick = m_bricks[brickIdx];
        brick->draw(x, y, alpha);
    } else {
        SDL_Log("Invalid brick index: %d", brickIdx);
    }
}

void BrickPool::draw(SPoint pos, int brickIdx, Uint8 alpha){
    draw(pos.x, pos.y, brickIdx, alpha);
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