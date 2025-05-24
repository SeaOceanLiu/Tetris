#ifndef FadeTrackerH
#define FadeTrackerH

#include <SDL3/SDL.h>
#include "ConstDef.h"
#include "Animation.h"

enum class FadeOperation{
    FadeIn,
    FadeOut
};

class FadeTracker : public TrackerDecorator{
private:
    FadeOperation m_fadeOperation;
    Uint8 m_alphaStep;
    bool m_isEnded;
    bool m_isFirstFrame;
public:
    FadeTracker(shared_ptr<Tracker> tracker, Uint8 alphaStep, FadeOperation fadeOperation);
    ~FadeTracker();

    void setFadeOperation(FadeOperation fadeOperation);
    bool isEnded(void);

    float setInitialX(float initialX) override;
    float setInitialY(float initialY) override;
    Uint8 setInitialAlpha(Uint8 initialAlpha) override;
    float calculateOffsetX(Uint64 nextFrame, float currentX, float currentY) override;
    float calculateOffsetY(Uint64 nextFrame, float currentX, float currentY) override;
    Uint8 calculateOffsetAlpha(Uint64 nextFrame, Uint8 currentAlpha) override;
};

#endif