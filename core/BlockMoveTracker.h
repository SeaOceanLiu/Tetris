#ifndef BlockMoveTrackerH
#define BlockMoveTrackerH

#include <SDL3/SDL.h>
#include "ConstDef.h"
#include "Animation.h"

enum class MoveDirection{
    NONE,
    LEFT,
    RIGHT,
    DOWN
};

class BlockMoveTracker : public TrackerDecorator{
private:
    SPoint m_moveOffset;
    SPoint m_maxOffset;
    MoveDirection m_moveDirection;
public:
    BlockMoveTracker(shared_ptr<Tracker> tracker, SPoint maxOffset);
    ~BlockMoveTracker();

    void setMoveDirection(MoveDirection direction);

    float calculateOffsetX(Uint64 nextFrame, float currentX, float currentY) override;
    float calculateOffsetY(Uint64 nextFrame, float currentX, float currentY) override;

};

#endif