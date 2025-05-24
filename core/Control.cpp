#include "Control.h"
ControlImpl::ControlImpl(Control *parent, float xScale, float yScale):
    // m_weakThis(this),
    // m_sharedThis(nullptr),
    m_parent(parent),
    m_enable(true),
    m_visible(true),
    m_xScale(xScale),
    m_yScale(yScale),
    m_xxScale(parent==nullptr?xScale:xScale*parent->getScaleXX()),
    m_yyScale(parent==nullptr?yScale:yScale*parent->getScaleYY()),
    m_renderer(nullptr),
    m_surface(nullptr),
    m_texture(nullptr),
    m_rect({0, 0, 0, 0})
{
    m_eventQueueInstance = EventQueue::getInstance();
    if (m_parent != nullptr){
        inheritRenderer();
    }
}

ControlImpl::ControlImpl(const ControlImpl &other):
    m_parent(other.m_parent),
    m_enable(other.m_enable),
    m_visible(other.m_visible),
    m_xScale(other.m_xScale),
    m_yScale(other.m_yScale),
    m_renderer(other.m_renderer),
    m_surface(other.m_surface),
    m_texture(other.m_texture),
    m_rect(other.m_rect)
{
    m_eventQueueInstance = other.m_eventQueueInstance;

    for(const auto& child : other.m_children){
        // shared_ptr<ControlImpl> newChild = make_shared<ControlImpl>(&child); // 这里需要深拷贝
        // addControl(newChild);
    }

}
ControlImpl& ControlImpl::operator=(const ControlImpl &other){
    m_parent = other.m_parent;
    m_enable = other.m_enable;
    m_visible = other.m_visible;
    m_xScale = other.m_xScale;
    m_yScale = other.m_yScale;
    m_renderer = other.m_renderer;
    m_surface = other.m_surface;
    m_texture = other.m_texture;
    m_rect = other.m_rect;
    m_eventQueueInstance = other.m_eventQueueInstance;

    for(const auto& child : other.m_children){
        // shared_ptr<ControlImpl>  newChild = make_shared<ControlImpl>(child); // 这里需要深拷贝
        // addControl(newChild);
    }

    return *this;
}

void ControlImpl::update(void){
    if(!m_enable) return;

    for (auto& child : m_children){
        child->update();
    }
}

void ControlImpl::draw(void){
    inheritRenderer();

    if (!m_visible) return;
    // draw the control
    // ...
    // draw the children
    for (auto& child : m_children){
        child->draw();
    }
}
  //事件处理，返回值表示是否处理了该事件，true表示处理了，false表示未处理
// 定义ControlImpl类的成员函数handleEvent，用于处理游戏事件
bool ControlImpl::handleEvent(GameEvent &event){
    // 检查当前控件是否可见且启用
    if (m_visible && m_enable){
        // 逆向遍历当前控件的所有子控件，保证后添加的控件先处理事件，因为后添加的控件在屏幕上位于上层
        for (auto it = m_children.rbegin(); it != m_children.rend(); ++it){
            // 调用子控件的handleEvent函数处理事件
            if ((*it)->handleEvent(event)){
                // 如果子控件处理了事件，则返回true
                return true;
            }
        }
    }
    // 如果当前控件及其子控件均未处理事件，则返回false
    return false;
}

// shared_ptr<Control> ControlImpl::addControl(shared_ptr<Control> child){
//     if (child == nullptr) return child;

//     // 如果控件已经存在，则直接返回
//     if (std::find(m_children.begin(), m_children.end(), child) != m_children.end()){
//         return child;
//     }
//     m_children.push_back(child);

//     child->setParent(this);
//     child->setRenderer(getRenderer());
//     child->setScaleX(m_xScale);
//     child->setScaleY(m_yScale);

//     return child;
// }

Control& ControlImpl::addControl(shared_ptr<Control> child){
    if (child == nullptr) return *this;

    // 如果控件已经存在，则直接返回
    if (std::find(m_children.begin(), m_children.end(), child) != m_children.end()){
        return *this;
    }
    m_children.push_back(child);

    child->setParent(this);
    child->setRenderer(getRenderer());
    child->setScaleX(m_xScale);
    child->setScaleY(m_yScale);

    return *this;
}

void ControlImpl::removeControl(shared_ptr<Control> child){
    m_children.erase(std::remove(m_children.begin(), m_children.end(), child), m_children.end());
}
// 只调用setParent的话，是不会添加到父控件的children中的，用于自行控制绘制逻辑和事件处理逻辑
// 如果要自动绘制和处理事件，需要调用addControl
void ControlImpl::setParent(Control *parent){
    m_parent = parent;
}

Control* ControlImpl::getParent(void){
    return m_parent;
}

float ControlImpl::getScaleXX(void){
    return m_xxScale;
}
float ControlImpl::getScaleYY(void){
    return m_yyScale;
}
void ControlImpl::setScaleX(float xScale){
    m_xScale = xScale;
    for (auto& child : m_children){
        child->setScaleX(xScale);
    }
}
void ControlImpl::setScaleY(float yScale){
    m_yScale = yScale;
    for (auto& child : m_children){
        child->setScaleY(yScale);
    }
}

void ControlImpl::setRect(SRect rect){
    m_rect = rect;
}

SRect ControlImpl::getRect(void){
    return m_rect;
}

void ControlImpl::show(void){
    m_visible = true;
}

void ControlImpl::hide(void){
    m_visible = false;
}

void ControlImpl::setEnable(bool enable){
    m_enable = enable;
}
bool ControlImpl::getEnable(void){
    return m_enable;
}

SDL_Renderer* ControlImpl::getRenderer(void){
    if (m_renderer != nullptr){
        return m_renderer;
    }
    if (m_parent != nullptr){
        m_renderer = m_parent->getRenderer();
    } else {
        throw std::runtime_error("No renderer found!");
    }
    return m_renderer;
}
shared_ptr<Control> ControlImpl::getThis(void){
    return shared_from_this();
}
void ControlImpl::setRenderer(SDL_Renderer* renderer){
    if (m_renderer == renderer) return;

    m_renderer = renderer;
    for (auto& child : m_children){
        child->setRenderer(renderer);
    }
}

SRect ControlImpl::getDrawRect(void){
    Control *parent = getParent();
    SRect parentDrawRect;
    if (parent != nullptr){
        parentDrawRect = parent->getDrawRect();
        return {m_rect.left * parent->getScaleXX() + parentDrawRect.left,
            m_rect.top * parent->getScaleYY() + parentDrawRect.top,
            m_rect.width * getScaleXX(),
            m_rect.height * getScaleYY()};
    }
    return {m_rect.left, m_rect.top, m_rect.width * m_xxScale, m_rect.height * m_yyScale};
}

SRect ControlImpl::mapToDrawRect(SRect rect){
    SRect drawRect = getDrawRect();
    return {rect.left * m_xxScale + drawRect.left, rect.top * m_yyScale + drawRect.top, rect.width * m_xxScale, rect.height * m_yyScale};
}

void ControlImpl::triggerEvent(GameEvent event){
    GameEvent *eventPtr = new GameEvent(event);
    m_eventQueueInstance->pushEventIntoQueue(eventPtr);
}

void ControlImpl::inheritRenderer(void) {
    if (m_renderer == nullptr){
        if (m_parent == nullptr) {
            SDL_Log("ControlImpl::draw() Error: m_renderer is nullptr and can't get it from m_parent for is's nullptr too!");
            throw("ControlImpl::draw() Error: m_renderer is nullptr and can't get it from m_parent for is's nullptr too!");
        }
        m_renderer = m_parent->getRenderer();
    }
}
