#include "BlockMoveTracker.h"

BlockMoveTracker::BlockMoveTracker(shared_ptr<Tracker> tracker, SPoint maxOffset):
    TrackerDecorator(tracker),
    m_moveOffset({0, 0}),
    m_maxOffset(maxOffset),
    m_moveDirection(MoveDirection::NONE)
{}

BlockMoveTracker::~BlockMoveTracker(){}

void BlockMoveTracker::setMoveDirection(MoveDirection direction) {
    m_moveDirection = direction;
    m_moveOffset = {0, 0};
}

float BlockMoveTracker::calculateOffsetX(Uint64 nextFrame, float currentX, float currentY) {
    float offsetX = 0;
    switch(m_moveDirection) {
        case MoveDirection::LEFT:
            if (m_moveOffset.x > -m_maxOffset.x) {
                m_moveOffset.x -= 1 * ConstDef::ANIMATION_MOVE_STEP_X;
                offsetX = -1 * ConstDef::ANIMATION_MOVE_STEP_X;
            }
            break;
        case MoveDirection::RIGHT:
            if(m_moveOffset.x < m_maxOffset.x) {
                m_moveOffset.x += 1 * ConstDef::ANIMATION_MOVE_STEP_X;
                offsetX = 1 * ConstDef::ANIMATION_MOVE_STEP_X;
            }
            break;
        default:
            break;
    }
    if (m_tracker != nullptr) {
        return offsetX + m_tracker->calculateOffsetX(nextFrame, currentX, currentY);
    }
    return offsetX;
}
float BlockMoveTracker::calculateOffsetY(Uint64 nextFrame, float currentX, float currentY) {
    float offsetY = 0;
    if (m_moveDirection == MoveDirection::DOWN) {
        if (m_moveOffset.y < m_maxOffset.y) {
            m_moveOffset.y += 1 * ConstDef::ANIMATION_MOVE_STEP_Y;
            offsetY = 1 * ConstDef::ANIMATION_MOVE_STEP_Y;
        }
    }
    if (m_tracker != nullptr) {
        return offsetY + m_tracker->calculateOffsetY(nextFrame, currentX, currentY);
    }
    return offsetY;
}
