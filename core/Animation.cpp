#include "Animation.h"
ActorGroup::ActorGroup(Control *parent, float xScale, float yScale):
    ControlImpl(parent, xScale, yScale)
{
    m_rect = {0, 0, getParent()->getRect().width, getParent()->getRect().height};
}

void ActorGroup::clear(void){
    m_offsets.clear();
    m_actors.clear();
}
void ActorGroup::freeActors(Control *newParent){
    for (auto &actor : m_actors){
        actor->setParent(newParent);
    }
    clear();
}
void ActorGroup::draw(float x, float y, Uint8 alpha){
    for (int i = 0; i < m_actors.size(); i++){
        m_actors[i]->draw(x + m_offsets[i].x, y + m_offsets[i].y, alpha);
    }
}

ActorGroup& ActorGroup::addActor(SPoint offset, shared_ptr<Actor>actor){
    m_offsets.push_back({offset.x, offset.y});
    m_actors.push_back(actor);
    return *this;
}

shared_ptr<ActorGroup> ActorGroup::build(void){
    auto newActorGroup = make_shared<ActorGroup>(this->getParent(), m_xScale, m_yScale);
    for (int i = 0; i < m_actors.size(); i++){
        newActorGroup->addActor(m_offsets[i], m_actors[i]);
    }
    this->clear();
    return newActorGroup;
}

ActorGroupBuilder::ActorGroupBuilder(Control *parent, float xScale, float yScale):
    m_actorGroup(make_shared<ActorGroup>(parent, xScale, yScale))
{
}
ActorGroupBuilder& ActorGroupBuilder::addActor(SPoint offset, shared_ptr<Actor>actor)
{
    m_actorGroup->addActor(offset, actor);
    return *this;
}
shared_ptr<ActorGroup> ActorGroupBuilder::build(void){
    return m_actorGroup;
}


TrackerDecorator::TrackerDecorator(shared_ptr<Tracker> tracker):
    m_tracker(tracker)
{
}
float TrackerDecorator::setInitialX(float initialX){
    return initialX;
}
float TrackerDecorator::setInitialY(float initialY){
    return initialY;
};
Uint8 TrackerDecorator::setInitialAlpha(Uint8 initialAlpha){
    return initialAlpha;
};

float TrackerDecorator::calculateOffsetX(Uint64 nextFrame, float currentX, float currentY){
    if (m_tracker != nullptr){
        return m_tracker->calculateOffsetX(nextFrame, currentX, currentY);
    }
    return 0;
}
float TrackerDecorator::calculateOffsetY(Uint64 nextFrame, float currentX, float currentY) {
    if (m_tracker != nullptr){
        return m_tracker->calculateOffsetY(nextFrame, currentX, currentY);
    }
    return 0;
}
Uint8 TrackerDecorator::calculateOffsetAlpha(Uint64 nextFrame, Uint8 currentAlpha){
    if (m_tracker != nullptr){
        return m_tracker->calculateOffsetAlpha(nextFrame, currentAlpha);
    }
    return 0;
}

Animation::Animation(Control *parent, Uint64 frameMSDuration, float xScale, float yScale):
    ControlImpl(parent, xScale, yScale),
    m_tracker(nullptr),
    m_nextFrameMsTick(0),
    m_frameMSDuration(frameMSDuration),
    m_animationDuration(0),
    m_calculateHandlerX(nullptr),
    m_calculateHandlerY(nullptr),
    m_calculateHandlerAlpha(nullptr),
    m_currentFrame(0),
    m_isDisplaying(false),
    m_isPlaying(false),
    m_isLoop(false),
    m_targetX(0.0f),
    m_targetY(0.0f),
    m_targetAlpha(SDL_ALPHA_OPAQUE)
{
    m_rect = {0, 0, getParent()->getRect().width, getParent()->getRect().height};
}

void Animation::freeFrames(Control *newParent) {
    for (auto &frame : m_frames){
        frame->freeActors(newParent);
    }
    clear();
}

void Animation::update(void) {
    if(!m_enable){
        return;
    }
    ControlImpl::update();

    if (!m_isPlaying){
        return;
    }
    Uint64 currentTick = SDL_GetTicks();
    if (currentTick < m_nextFrameMsTick) {
        return;
    }
    m_currentFrame = m_currentFrame + ((currentTick - m_nextFrameMsTick) / m_frameMSDuration + 1);
    if (m_currentFrame >= m_frames.size()) {
        if (m_isLoop) {
            m_currentFrame = 0;
        } else {
            m_currentFrame = m_frames.size() - 1;

            // 发出动画结束事件
            triggerEvent({EventName::AnimationEnded, this});
        }
    }

    if (m_calculateHandlerX != nullptr) {
        m_targetX += m_calculateHandlerX(m_currentFrame, m_targetX, m_targetX);
    } else {
        m_targetX += calculateOffsetX(m_currentFrame, m_targetX, m_targetY);
    }
    if (m_calculateHandlerY != nullptr) {
        m_targetY += m_calculateHandlerY(m_currentFrame, m_targetX, m_targetX);
    } else {
        m_targetY += calculateOffsetY(m_currentFrame, m_targetX, m_targetY);
    }
    if (m_calculateHandlerAlpha != nullptr) {
        m_targetAlpha += m_calculateHandlerAlpha(m_currentFrame, m_targetAlpha);
    } else {
        m_targetAlpha += calculateOffsetAlpha(m_currentFrame, m_targetAlpha);
    }

    m_nextFrameMsTick = currentTick + m_frameMSDuration;
}
void Animation::draw(void) {
    inheritRenderer();

    if (!m_visible){
        return;
    }
    m_frames[m_currentFrame]->draw(m_targetX, m_targetY, m_targetAlpha);

    ControlImpl::draw();
}

void Animation::setRect(SRect rect){
    for(auto &frame : m_frames){
        frame->setRect(rect);
    }
}

void Animation::prepare(float initialX, float initialY, int startFrame, bool isLoop) {
    m_isLoop = isLoop;
    m_currentFrame = startFrame;
    m_targetX = initialX;
    m_targetY = initialY;
}

void Animation::pause(void) {
    m_isPlaying = false;
}
void Animation::resume(void) {
    m_isPlaying = true;
}
bool Animation::isPlaying(void) {
    return m_isPlaying;
}
float Animation::calculateOffsetX(Uint64 nextFrame, float currentX, float currentY){
    if (m_tracker != nullptr){
        return m_tracker->calculateOffsetX(nextFrame, currentX, currentY);
    }
    return 0;
}
float Animation::calculateOffsetY(Uint64 nextFrame, float currentX, float currentY){
    if ( m_tracker != nullptr){
        return m_tracker->calculateOffsetY(nextFrame, currentX, currentY);
    }
    return 0;
}
Uint8 Animation::calculateOffsetAlpha(Uint64 nextFrame, Uint8 currentAlpha){
    if (m_tracker != nullptr){
        return m_tracker->calculateOffsetAlpha(nextFrame, currentAlpha);
    }
    if (m_targetAlpha != SDL_ALPHA_OPAQUE){
        return SDL_ALPHA_OPAQUE - m_targetAlpha;
    }
    return 0;
}

void Animation::clear(void) {
    m_frames.clear();
}

/*********************************************************for Builder mode**********************************************************/
Animation& Animation::setInitialPos(float x, float y){
    m_targetX = x;
    m_targetY = y;
    return *this;
}
Animation& Animation::setInitialAlpha(Uint8 alpha){
    m_targetAlpha = alpha;
    return *this;
}
Animation& Animation::setFrameMSDuration(Uint64 duration){
    m_frameMSDuration = duration;
    return *this;
}
Animation& Animation::setTracker(shared_ptr<Tracker> tracker){
    m_tracker = tracker;
    if (m_tracker != nullptr){
        m_targetX = m_tracker->setInitialX(m_targetX);
        m_targetY = m_tracker->setInitialY(m_targetY);
        m_targetAlpha = m_tracker->setInitialAlpha(m_targetAlpha);
    }
    return *this;
}
Animation&  Animation::setCalculateHandlerX(CalculateCallBack handler) {
    m_calculateHandlerX = handler;
    return *this;
}
Animation&  Animation::setCalculateHandlerY(CalculateCallBack handler) {
    m_calculateHandlerY = handler;
    return *this;
}
Animation&  Animation::setCalculateHandlerAlpha(CalculateAlphaCallBack handler) {
    m_calculateHandlerAlpha = handler;
    return *this;
}
Animation& Animation::setStartFrame(int startFrame){
    m_currentFrame = startFrame;
    return *this;
}
Animation& Animation::setLoop(bool isLoop){
    m_isLoop = isLoop;
    return *this;
}
Animation& Animation::addFrame(shared_ptr<ActorGroup> frame) {
    // frame->setParent(this);
    m_frames.push_back(frame);
    m_animationDuration += m_frameMSDuration;
    return *this;
}
Animation& Animation::addPhotosInPathAsFrames(fs::path filePath, string extName, bool matchParentRect){
    unique_ptr<TinyFS> fsystem = make_unique<TinyFS>();
    auto flist = fsystem->getFileList(filePath, extName);
    for (auto &f : flist) {
        addFrame(ActorGroup(this)
                    .addActor({0, 0}, make_shared<Actor>(this, f, matchParentRect))
                    .build());
    }
    return *this;
}
shared_ptr<Animation> Animation::build(void){
    auto newAnimation = make_shared<Animation>(this->getParent(), m_xScale, m_yScale);
    newAnimation->setFrameMSDuration(m_frameMSDuration);
    for (int i = 0; i < m_frames.size(); i++){
        newAnimation->addFrame(m_frames[i]);
    }
    newAnimation->setTracker(m_tracker);
    newAnimation->setCalculateHandlerX(m_calculateHandlerX);
    newAnimation->setCalculateHandlerY(m_calculateHandlerY);
    newAnimation->setCalculateHandlerAlpha(m_calculateHandlerAlpha);
    newAnimation->setStartFrame(m_currentFrame);
    newAnimation->setLoop(m_isLoop);
    newAnimation->setInitialPos(m_targetX, m_targetY);
    newAnimation->setInitialAlpha(m_targetAlpha);
    this->clear();
    return newAnimation;
}

AnimationBuilder::AnimationBuilder(Control *parent, Uint64 frameMSDuration, float xScale, float yScale):
    m_animation(nullptr)
{
    m_animation = make_shared<Animation>(parent, frameMSDuration, xScale, yScale);
}
AnimationBuilder& AnimationBuilder::setInitialPos(float x, float y){
    m_animation->setInitialPos(x, y);
    return *this;
}
AnimationBuilder& AnimationBuilder::setInitialAlpha(Uint8 alpha){
    m_animation->setInitialAlpha(alpha);
    return *this;
}
AnimationBuilder& AnimationBuilder::setLoop(bool isLoop){
    m_animation->setLoop(isLoop);
    return *this;
}
AnimationBuilder& AnimationBuilder::setFrameMSDuration(Uint64 duration){
    m_animation->setFrameMSDuration(duration);
    return *this;
}
AnimationBuilder& AnimationBuilder::setStartFrame(int startFrame){
    m_animation->setStartFrame(startFrame);
    return *this;
}
AnimationBuilder& AnimationBuilder::addFrame(shared_ptr<ActorGroup>frame) {
    m_animation->addFrame(frame);
    return *this;
}
AnimationBuilder& AnimationBuilder::addPhotosInPathAsFrames(fs::path filePath, string extName, bool matchParentRect){
    m_animation->addPhotosInPathAsFrames(filePath, extName, matchParentRect);
    return *this;
}
AnimationBuilder& AnimationBuilder::setCalculateHandlerX(Animation::CalculateCallBack handler){
    m_animation->setCalculateHandlerX(handler);
    return *this;
}
AnimationBuilder& AnimationBuilder::setCalculateHandlerY(Animation::CalculateCallBack handler){
    m_animation->setCalculateHandlerY(handler);
    return *this;
}
AnimationBuilder& AnimationBuilder::setCalculateHandlerAlpha(Animation::CalculateAlphaCallBack handler){
    m_animation->setCalculateHandlerAlpha(handler);
    return *this;
}
AnimationBuilder& AnimationBuilder::setTracker(shared_ptr<Tracker>tracker){
    m_animation->setTracker(tracker);
    return *this;
}
shared_ptr<Animation> AnimationBuilder::build(void){
    return m_animation;
}