#ifndef AnimationH
#define AnimationH
#include <vector>
#include <unordered_map>
#include <functional>
#include <SDL3/SDL.h>

#include "Actor.h"
#include "EventQueue.h"
#include "TinyFS.h"

class ActorGroup: public ControlImpl{
friend class ActorGroupBuilder;
private:
    std::vector<SPoint> m_offsets;  // Offsets(x, y) of the actors
    std::vector<shared_ptr<Actor>> m_actors;

public:
    ActorGroup(Control *parent, float xScale=1.0f, float yScale=1.0f);
    // ActorGroup(Control *parent, SPoint offset, shared_ptr<Actor>actor, float xScale=1.0f, float yScale=1.0f);

    void clear(void);
    virtual void draw(float x, float y, Uint8 alpha=SDL_ALPHA_OPAQUE);
    vector<shared_ptr<Actor>> getActors(void) { return m_actors; };
    void freeActors(Control *newParent);

    ActorGroup& addActor(SPoint offset, shared_ptr<Actor>actor);
    shared_ptr<ActorGroup> build(void);
};

class ActorGroupBuilder {
private:
    shared_ptr<ActorGroup> m_actorGroup;
public:
    ActorGroupBuilder(Control *parent, float xScale=1.0f, float yScale=1.0f);
    ActorGroupBuilder& addActor(SPoint offset, shared_ptr<Actor>actor);
    shared_ptr<ActorGroup> build(void);
};

class Tracker {
public:
    virtual ~Tracker() = default;
    virtual float setInitialX(float initialX) = 0;
    virtual float setInitialY(float initialY) = 0;
    virtual Uint8 setInitialAlpha(Uint8 initialAlpha) = 0;

    virtual float calculateOffsetX(Uint64 nextFrame, float currentX, float currentY) = 0;
    virtual float calculateOffsetY(Uint64 nextFrame, float currentX, float currentY) = 0;
    virtual Uint8 calculateOffsetAlpha(Uint64 nextFrame, Uint8 currentAlpha) = 0;
};

class TrackerDecorator: public Tracker{
protected:
    shared_ptr<Tracker> m_tracker;
public:
    TrackerDecorator(shared_ptr<Tracker>tracker);
    virtual ~TrackerDecorator() = default;
    float setInitialX(float initialX) override;
    float setInitialY(float initialY) override;
    Uint8 setInitialAlpha(Uint8 initialAlpha) override;

    float calculateOffsetX(Uint64 nextFrame, float currentX, float currentY) override;
    float calculateOffsetY(Uint64 nextFrame, float currentX, float currentY) override;
    Uint8 calculateOffsetAlpha(Uint64 nextFrame, Uint8 currentAlpha) override;
};

class Animation: public ControlImpl{
    friend class AnimationBuilder;
    using CalculateCallBack = std::function<float (Uint64, float, float)>;  // float calculateCallBack(int nextFrame, float currentX, float currentY)
    using CalculateAlphaCallBack = std::function<Uint8 (Uint64, Uint8)>;  // Uint8 calculateCallBack(int nextFrame, Uint8 currentAlpha)
private:
    Uint64 m_nextFrameMsTick;   // 下一次切换帧的时间戳（毫秒）
    Uint64 m_frameMSDuration;   // 动画每帧的持续时间（毫秒）
    Uint64 m_animationDuration; // 动画的总持续时间（毫秒）

    CalculateCallBack m_calculateHandlerX;
    CalculateCallBack m_calculateHandlerY;
    CalculateAlphaCallBack m_calculateHandlerAlpha;
protected:
    shared_ptr<Tracker>m_tracker;

    Uint64 m_currentFrame; // 当前帧的索引
    std::vector<shared_ptr<ActorGroup>> m_frames; // 存储动画帧
    bool m_isDisplaying;    // 动画是否正在显示
    bool m_isPlaying;       // 动画是否正在播放
    bool m_isLoop;          // 动画是否循环播放

    float m_targetX;        // 动画的坐标
    float m_targetY;        // 动画的坐标
    Uint8 m_targetAlpha;    // 动画的透明度

private:
    float calculateOffsetX(Uint64 nextFrame, float currentX, float currentY);
    float calculateOffsetY(Uint64 nextFrame, float currentX, float currentY);
    Uint8 calculateOffsetAlpha(Uint64 nextFrame, Uint8 currentAlpha);

public:
    Animation(Control *parent, Uint64 frameMSDuration=33, float xScale=1.0f, float yScale=1.0f);
    virtual ~Animation(){};
    vector<shared_ptr<ActorGroup>> getFrames(void) { return m_frames; };
    void freeFrames(Control *newParent);
    void update(void);
    void draw(void);
    void setRect(SRect rect);

    void prepare(float initialX, float initialY, int startFrame=0, bool isLoop=false);
    void pause(void);
    void resume(void);
    bool isPlaying(void);
    void clear(void);

    Animation& setInitialPos(float x, float y);
    Animation& setInitialAlpha(Uint8 alpha);
    Animation& setLoop(bool isLoop);
    Animation& setFrameMSDuration(Uint64 duration);
    Animation& setStartFrame(int startFrame);
    Animation& addFrame(shared_ptr<ActorGroup>frame);
    Animation& addPhotosInPathAsFrames(fs::path filePath, string extName, bool matchParentRect);
    Animation& setCalculateHandlerX(CalculateCallBack handler);
    Animation& setCalculateHandlerY(CalculateCallBack handler);
    Animation& setCalculateHandlerAlpha(CalculateAlphaCallBack handler);
    Animation& setTracker(shared_ptr<Tracker>tracker);
    shared_ptr<Animation> build(void);
};

class AnimationBuilder {
private:
    shared_ptr<Animation> m_animation;
public:
    AnimationBuilder(Control *parent, Uint64 frameMSDuration=33, float xScale=1.0f, float yScale=1.0f);
    AnimationBuilder& setInitialPos(float x, float y);
    AnimationBuilder& setInitialAlpha(Uint8 alpha);
    AnimationBuilder& setLoop(bool isLoop);
    AnimationBuilder& setFrameMSDuration(Uint64 duration);
    AnimationBuilder& setStartFrame(int startFrame);
    AnimationBuilder& addFrame(shared_ptr<ActorGroup>frame);
    AnimationBuilder& addPhotosInPathAsFrames(fs::path filePath, string extName, bool matchParentRect);
    AnimationBuilder& setCalculateHandlerX(Animation::CalculateCallBack handler);
    AnimationBuilder& setCalculateHandlerY(Animation::CalculateCallBack handler);
    AnimationBuilder& setCalculateHandlerAlpha(Animation::CalculateAlphaCallBack handler);
    AnimationBuilder& setTracker(shared_ptr<Tracker>tracker);
    shared_ptr<Animation> build(void);
};
#endif // AnimationH