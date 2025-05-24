#include "FadeTracker.h"

FadeTracker::FadeTracker(shared_ptr<Tracker> tracker, Uint8 alphaStep, FadeOperation fadeOperation=FadeOperation::FadeIn):
    TrackerDecorator(tracker),
    m_fadeOperation(fadeOperation),
    m_alphaStep(alphaStep),
    m_isEnded(false),
    m_isFirstFrame(true)
{}

FadeTracker::~FadeTracker(){}

void FadeTracker::setFadeOperation(FadeOperation fadeOperation)
{
    m_fadeOperation = fadeOperation;
    m_isEnded = false;
    m_isFirstFrame = true;
}

bool FadeTracker::isEnded(void){
    return m_isEnded;
}
float FadeTracker::setInitialX(float initialX){
    return initialX;
}
float FadeTracker::setInitialY(float initialY){
    return initialY;
};
Uint8 FadeTracker::setInitialAlpha(Uint8 initialAlpha){
    return 0;
};

float FadeTracker::calculateOffsetX(Uint64 nextFrame, float currentX, float currentY) {
    return 0;
}
float FadeTracker::calculateOffsetY(Uint64 nextFrame, float currentX, float currentY) {
    return 0;
}
Uint8 FadeTracker::calculateOffsetAlpha(Uint64 nextFrame, Uint8 currentAlpha){
    if (m_isEnded){
        return 0;
    }

    switch (m_fadeOperation){
        case FadeOperation::FadeIn:
            // 第一帧时，将alpha调整为0
            if (m_isFirstFrame){
                m_isFirstFrame = false;
                return -currentAlpha;
            }
            if (currentAlpha + m_alphaStep > 255){
                m_isEnded = true;
                return 255;
            }
            return m_alphaStep;
        case FadeOperation::FadeOut:
            // 第一帧时，将alpha调整为255
            if (m_isFirstFrame){
                m_isFirstFrame = false;
                return SDL_ALPHA_OPAQUE - currentAlpha;
            }
            if (currentAlpha - m_alphaStep <= 0){
                m_isEnded = true;
                return 0;
            }
            return -m_alphaStep;
            break;
        default:
            break;
    }
    return 0;
}