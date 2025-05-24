#include "Panel.h"

Panel::Panel(Control *parent, SRect rect, float xScale, float yScale):
    ControlImpl(parent, xScale, yScale),
    m_bgColor(ConstDef::PANEL_BG_COLOR),
    m_borderColor(ConstDef::PANEL_BORDER_COLOR),
    m_background(nullptr),
    m_isTransparent(false),
    m_isBorderVisible(true)
{
    m_rect = rect;
}

void Panel::nextBackground(void){
    if (m_background == nullptr) return;

    m_background->next();
}
void Panel::resetBackground(void){
    if (m_background == nullptr) return;

    m_background->startFrom(0);
}

void Panel::update(void){
    if (!m_enable) return;

    if (m_background != nullptr) {
        m_background->update();
    }

    // 如果有动画，这里需要更新动画
    for (const auto& [animation, point] : m_animations) {
        animation->update();
    }

    // 如果有子控件，这里需要更新子控件
    ControlImpl::update();
}
void Panel::draw(void){
    if (!m_visible) return;

    SRect drawRect = getDrawRect();

    // 1. 先绘制当前控件的外观
    if(!m_isTransparent) {
        if(!SDL_SetRenderDrawColor(getRenderer(), m_bgColor.r, m_bgColor.g, m_bgColor.b, m_bgColor.a)){
            SDL_Log("Panel failed to set background color: %s", SDL_GetError());
        }
        if (!SDL_RenderFillRect(getRenderer(), drawRect.toSDLFRect())){
            SDL_Log("Panel failed to fill render rect: %s", SDL_GetError());
        }
    }
    if (m_background) {
        m_background->draw();
    }

    // 2. 绘制当前控件的动画
    for (const auto& [animation, point] : m_animations) {
        animation->draw();
    }

    // 3. 绘制当前控件的图标
    for (const auto& [actor, point] : m_actors) {
        actor->draw(point.x, point.y);
    }

    // 4. 接着绘制子控件
    ControlImpl::draw();

    // 5. 最后绘制边框
    if(!m_isTransparent && m_isBorderVisible) {
        if(!SDL_SetRenderDrawColor(getRenderer(), m_borderColor.r, m_borderColor.g, m_borderColor.b, m_borderColor.a)){
            SDL_Log("Panel fFailed to set border color: %s", SDL_GetError());
        }
        if(!SDL_RenderRect(getRenderer(), drawRect.toSDLFRect())){
            SDL_Log("Panel failed to draw border: %s", SDL_GetError());
        }
    }
}
bool Panel::handleEvent(GameEvent &event){
    switch(event.m_eventName) {
        case EventName::NextBackground:
            if (m_background) {
                nextBackground();
                return true;
            }
        default:
            break;
    }
    return ControlImpl::handleEvent(event);

}


Panel& Panel::setBackground(shared_ptr<PhotoCarousel> background, int startFrom){
    m_background = background;
    if (m_background == nullptr) return *this;

    m_background->startFrom(startFrom);
    m_background->setParent(this);
    return *this;
}
Panel& Panel::addAnimation(float x, float y, shared_ptr<Animation> animation){
    if (animation == nullptr) return *this;

    animation->setParent(this);
    animation->setRect(getRect());
    m_animations[animation] = SPoint(x, y);
    return * this;
}
Panel& Panel::addActor(float x, float y, shared_ptr<Actor> actor){
    if (actor == nullptr) return *this;

    actor->setParent(this);
    actor->setRect(getRect());
    m_actors[actor] = SPoint(x, y);
    return * this;
}
Panel& Panel::setBGColor(SDL_Color color){
    m_bgColor = color;
    return *this;
}
Panel& Panel::setBorderColor(SDL_Color color)
{
    m_borderColor = color;
    return *this;
}
Panel& Panel::setTransparent(bool isTransparent)
{
    m_isTransparent = isTransparent;
    return *this;
}
Panel& Panel::addControl(shared_ptr<Control> control){
    ControlImpl::addControl(control);
    return *this;
}
shared_ptr<Panel> Panel::build(void){
    shared_ptr<Panel> newPanel =  make_shared<Panel>(this->getParent(), m_rect, m_xScale, m_yScale);
    newPanel->setBGColor(m_bgColor);
    newPanel->setBorderColor(m_borderColor);
    newPanel->setTransparent(m_isTransparent);
    newPanel->setBackground(m_background);
    for (const auto& [animation, point] : m_animations) {
        newPanel->addAnimation(point.x, point.y, animation);
    }

    for(const auto& [actor, point] : m_actors) {
        newPanel->addActor(point.x, point.y, actor);
    }
    return newPanel;
}

PanelBuilder::PanelBuilder(Control *parent, SRect rect, float xScale, float yScale):
    m_panel(nullptr)
{
    m_panel = make_shared<Panel>(parent, rect, xScale, yScale);
}
PanelBuilder& PanelBuilder::setBackground(shared_ptr<PhotoCarousel> background, int startFrom){
    m_panel->setBackground(background, startFrom);
    return *this;
}
PanelBuilder& PanelBuilder::addAnimation(float x, float y, shared_ptr<Animation> animation){
    m_panel->addAnimation(x, y, animation);
    return *this;
}
PanelBuilder& PanelBuilder::addActor(float x, float y, shared_ptr<Actor> actor){
    m_panel->addActor(x, y, actor);
    return *this;
}
PanelBuilder& PanelBuilder::setBGColor(SDL_Color color){
    m_panel->setBGColor(color);
    return *this;
}
PanelBuilder& PanelBuilder::setBorderColor(SDL_Color color){
    m_panel->setBorderColor(color);
    return *this;
}
PanelBuilder& PanelBuilder::setTransparent(bool isTransparent){
    m_panel->setTransparent(isTransparent);
    return *this;
}
PanelBuilder& PanelBuilder::setBorderVisible(bool isBorderVisible){
    m_panel->m_isBorderVisible = isBorderVisible;
    return *this;
}
PanelBuilder& PanelBuilder::addControl(shared_ptr<Control> control){
    m_panel->addControl(control);
    return *this;
}
shared_ptr<Panel> PanelBuilder::build(void){
    return m_panel;
}