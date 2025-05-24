#include "Button.h"
Button::Button(Control *parent, SRect rect, float xScale, float yScale):
    ControlImpl(parent, xScale, yScale),
    m_state(ButtonState::Normal),
    m_onClick(nullptr),
    m_actor(nullptr),
    m_hoverActor(nullptr),
    m_pressedActor(nullptr),
    m_caption(nullptr),
    m_animation(nullptr),
    m_isTransparent(false),
    m_id(INT_MAX),
    m_captionSize(ConstDef::BUTTON_CAPTION_SIZE)
{
    m_rect = rect;
}

void Button::update(void){
    if (!m_enable) return;

    // 如果有动画，这里需要更新动画
    if(m_animation != nullptr){
        m_animation->update();
    }

    // 如果有定时器，这里需要更新定时器

    // 如果有子控件，这里需要更新子控件
    ControlImpl::update();
}
void Button::draw(void){
    if (!m_visible) return;

    // 1. 先绘制当前控件的外观
    if (!m_isTransparent){
        SRect drawRect = getDrawRect();
        // SDL_FRect rect = {drawRect.left, drawRect.top, drawRect.width, drawRect.height};

        SDL_Color color;
        switch (m_state){
            case ButtonState::Hover:
                color = ConstDef::BUTTON_HOVER_COLOR;
                break;
            case ButtonState::Pressed:
                color = ConstDef::BUTTON_DOWN_COLOR;
                break;
            case ButtonState::Normal:
            default:
                color = ConstDef::BUTTON_NORMAL_COLOR;
                break;
        }

        if(!SDL_SetRenderDrawColor(getRenderer(), color.r, color.g, color.b, color.a)){
            SDL_Log("Failed to set grid render color: %s", SDL_GetError());
        }

        if (!SDL_RenderFillRect(getRenderer(), drawRect.toSDLFRect())){
            SDL_Log("Failed to fill render rect: %s", SDL_GetError());
        }
    }

    // 2. 绘制当前控件的图标
    auto actor = m_actor;
    switch(m_state){
        case ButtonState::Hover:
            if (m_hoverActor != nullptr){
                actor = m_hoverActor;
            }
            break;
        case ButtonState::Pressed:
            if (m_pressedActor != nullptr){
                actor = m_pressedActor;
            }
            break;
        case ButtonState::Normal:
        default:
            break;
    }

    if (actor != nullptr){
        actor->draw();
    }

    // 4. 绘制动画
    if(m_animation != nullptr){
        m_animation->draw();
    }

    // 3. 绘制当前控件的标题
    if (m_caption != nullptr){
        m_caption->draw();
    }

    // 4. 接着绘制子控件
    ControlImpl::draw();
}

bool Button::handleEvent(GameEvent &event){
    if (!m_enable || !m_visible) return false;

    if (event.isPositionEvent()){
        SPoint *pos = (SPoint *)(event.m_eventParam);
        SRect drawRect = getDrawRect(); // 获取当前控件的绘制区域
        if (drawRect.contains(pos->x, pos->y)){
            switch(event.m_eventName){
                case EventName::FINGER_DOWN:
                case EventName::FINGER_MOTION:
                    // Todo: 如果是触控，一般是要考虑连续触发，所以这里没有像下面MOUSE_LBUTTON_DOWN那样处理成判断按钮状态，但这里最好做成参数控制
                    if (m_onClick != nullptr){
                        m_onClick(dynamic_pointer_cast<Button>(this->getThis()));
                    }
                    m_state = ButtonState::Pressed;
                    return true;
                case EventName::MOUSE_LBUTTON_DOWN:
                    m_state = ButtonState::Pressed;
                    return true;
                case EventName::MOUSE_LBUTTON_UP:
                    if (m_onClick != nullptr && m_state == ButtonState::Pressed){
                        m_onClick(dynamic_pointer_cast<Button>(this->getThis()));
                    }
                    // 需要置与EventName::FINGER_UP一样的Button状态，所以直接流入下一个case
                case EventName::FINGER_UP:
                    m_state = ButtonState::Normal;
                    return true;
                case EventName::MOUSE_MOVING:
                // case EventName::FINGER_MOTION:
                    m_state = ButtonState::Hover;
                    return true;
                default:
                    break;
            }
            return true;
        } else {
            m_state = ButtonState::Normal;
        }
    }
    if (ControlImpl::handleEvent(event)) return true;
    return false;
}

int Button::getId(void) const{
    return m_id;
}

/*********************************************************for Builder mode**********************************************************/

Button& Button::setBtnNormalStateActor(shared_ptr<Actor> actor){
    if (actor == nullptr) return *this;

    actor->setRect({0, 0, m_rect.width, m_rect.height});
    actor->setParent(this);
    m_actor = actor;
    return *this;
}
Button& Button::setBtnHoverStateActor(shared_ptr<Actor> actor){
    if (actor == nullptr) return *this;

    actor->setRect({0, 0, m_rect.width, m_rect.height});
    actor->setParent(this);
    m_hoverActor = actor;
    return *this;
}

Button& Button::setBtnPressedStateActor(shared_ptr<Actor> actor){
    if (actor == nullptr) return *this;

    actor->setRect({0, 0, m_rect.width, m_rect.height});
    actor->setParent(this);
    m_pressedActor = actor;
    return *this;
}
Button& Button::setCaption(string caption){
    m_captionText = caption;

    if (m_caption != nullptr){
        m_caption.reset();
        m_caption = nullptr;
    }
    if (m_captionText.length() > 0) {
        m_caption = LabelBuilder(this, {0, 0, m_rect.width, m_rect.height})
                            .setFont(FontName::HarmonyOS_Sans_SC_Regular)
                            .setAlignmentMode(AlignmentMode::AM_CENTER)
                            .setFontSize(m_captionSize)
                            .setCaption(m_captionText)
                            .build();
    }
    return *this;
}
Button& Button::setCaptionSize(float size){
    m_captionSize = size;
    if (m_caption != nullptr){
        m_caption->setFontSize(m_captionSize);
    }
    return *this;
}
Button& Button::setAnimation(shared_ptr<Animation> animation){
    m_animation = animation;

    if (m_animation != nullptr){
        // 设置动画的父控件为当前控件
        m_animation->setParent(this);
        for(auto frame : m_animation->getFrames()){
            frame->setParent(this);
            for (auto actor : frame->getActors()){
                actor->setParent(this);
            }
        }
        m_animation->resume();
    }
    return *this;
}
Button& Button::setOnClick(OnClickHandler onClick){
    m_onClick = onClick;
    return *this;
}
Button& Button::setTransparent(bool isTransparent){
    m_isTransparent = isTransparent;
    return *this;
}
Button& Button::setId(int id){
    m_id = id;
    return *this;
}
shared_ptr<Button> Button::build(void){
    auto newBtn = make_shared<Button>(this->getParent(), m_rect, m_xScale, m_yScale);
    newBtn->setBtnNormalStateActor(m_actor);
    newBtn->setBtnHoverStateActor(m_hoverActor);
    newBtn->setBtnPressedStateActor(m_pressedActor);
    newBtn->setCaption(m_captionText);
    newBtn->setAnimation(m_animation);
    newBtn->setOnClick(m_onClick);
    newBtn->setTransparent(m_isTransparent);
    newBtn->setId(m_id);
    return newBtn;
}

ButtonBuilder::ButtonBuilder(Control *parent, SRect rect, float xScale, float yScale):
    m_button(nullptr)
{
    m_button = make_shared<Button>(parent, rect, xScale, yScale);
}
ButtonBuilder& ButtonBuilder::setBtnNormalStateActor(shared_ptr<Actor> actor){
    m_button->setBtnNormalStateActor(actor);
    return *this;
}
ButtonBuilder& ButtonBuilder::setBtnHoverStateActor(shared_ptr<Actor> actor){
    m_button->setBtnHoverStateActor(actor);
    return *this;
}
ButtonBuilder& ButtonBuilder::setBtnPressedStateActor(shared_ptr<Actor> actor){
    m_button->setBtnPressedStateActor(actor);
    return *this;
}
ButtonBuilder& ButtonBuilder::setCaption(string caption){
    m_button->setCaption(caption);
    return *this;
}
ButtonBuilder& ButtonBuilder::setCaptionSize(float size){
    m_button->setCaptionSize(size);
    return *this;
}
ButtonBuilder& ButtonBuilder::setAnimation(shared_ptr<Animation> animation){
    m_button->setAnimation(animation);
    return *this;
}
ButtonBuilder& ButtonBuilder::setOnClick(Button::OnClickHandler onClick){
    m_button->setOnClick(onClick);
    return *this;
}
ButtonBuilder& ButtonBuilder::setTransparent(bool isTransparent){
    m_button->setTransparent(isTransparent);
    return *this;
}
ButtonBuilder& ButtonBuilder::setId(int id){
    m_button->setId(id);
    return *this;
}
shared_ptr<Button> ButtonBuilder::build(void){
    return m_button;
}