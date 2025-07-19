#include "Label.h"

Label::Label(Control *parent, SRect rect, float xScale, float yScale):
    ControlImpl(parent, xScale, yScale),
    m_ttfText(nullptr),
    m_font(nullptr),
    m_textEngin(nullptr),
    m_hoverCursor(nullptr),
    m_defaultCursor(nullptr),
    m_textSize(0.0f, 0.0f),
    m_translatedPos({0, 0}),
    m_normalStateColor({0, 0, 0, SDL_ALPHA_OPAQUE}),
    m_hoverStateColor({0, 0, 0, SDL_ALPHA_OPAQUE}),
    m_pressedStateColor({0, 0, 0, SDL_ALPHA_OPAQUE}),
    m_shadowColor({255, 255, 255, SDL_ALPHA_OPAQUE}),
    m_shadowOffset({2, 2}),
    m_AlignmentMode(AlignmentMode::AM_MID_LEFT),
    m_fontSize(10),
    m_fontName(FontName::HarmonyOS_Sans_Condensed_Thin),
    m_fontFile(ResourceLoader::m_fontFiles[m_fontName]),
    m_caption("Label"),
    m_shadowEnabled(false),
    m_hotRect({0, 0, 0, 0}),
    m_onClick(nullptr),
    m_state(LabelState::Normal),
    m_fontStyle(TTF_STYLE_NORMAL),
    m_id(INT_MAX)
{
    m_rect = rect;

    m_hoverCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_POINTER);
    if (m_hoverCursor == nullptr) {
        SDL_Log("Failed to create cursor: %s", SDL_GetError());
    }
    m_defaultCursor = SDL_GetCursor();
    if(m_defaultCursor == nullptr) {
        SDL_Log("Failed to get default cursor: %s", SDL_GetError());
    }
}

Label::~Label(void){
    if (m_ttfText != nullptr) {
        TTF_DestroyText(m_ttfText);
        m_ttfText = nullptr;
    }
    if (m_textEngin != nullptr) {
        TTF_DestroyRendererTextEngine(m_textEngin);
        m_textEngin = nullptr;
    }
    if (m_font != nullptr) {
        TTF_CloseFont(m_font);   // 注意必须保证在TTF_Quit()之前执行关闭字体的操作，否则会报错
        m_font = nullptr;
    }
    if (m_hoverCursor != nullptr) {
        SDL_DestroyCursor(m_hoverCursor);
        m_hoverCursor = nullptr;
    }
}
void Label::createTextEngine(void){
    SetFontStyle(m_fontStyle);

    if (m_textEngin != nullptr) {
        TTF_DestroyRendererTextEngine(m_textEngin);
        m_textEngin = nullptr;
    }
    m_textEngin = TTF_CreateRendererTextEngine(getRenderer());
    if (m_textEngin == nullptr) {
        SDL_Log("Failed to create text engine: %s", SDL_GetError());
        return;
    }
}
void Label::loadFromFile(void){
    fs::path fullPath = ConstDef::pathPrefix / m_fontFile;
    m_font = TTF_OpenFont(fullPath.string().c_str(), m_fontSize * getScaleXX());
    if (m_font == nullptr) {
        SDL_Log("Failed to load font: %s", SDL_GetError());
        throw "Failed to load font: %s", SDL_GetError();
        return;
    }

    createTextEngine();
}

void Label::loadFromResource(string resourceId){
    shared_ptr<Resource> resource = ResourceLoader::getInstance()->getResource(resourceId);
    if (resource == nullptr || resource->resourceType != ResourceLoader::RT_FONTS
        || resource->pMem == nullptr) {

        SDL_Log("LoadFromResource Error: '%s' is not a font\n", resourceId.c_str());
        return;
    }
    SDL_IOStream *resourceStream = SDL_IOFromConstMem(resource->pMem.get(), resource->resourceSize);
    if (resourceStream == nullptr) {
        SDL_Log("Failed to create IO stream for: %s", resourceId.c_str());
        return;
    }
    m_font = TTF_OpenFontIO(resourceStream, true, m_fontSize * getScaleXX());
    if (m_font == nullptr) {
        SDL_Log("Failed to load font: %s", SDL_GetError());
        throw "Failed to load font: %s", SDL_GetError();
        return;
    }

    createTextEngine();
}

void Label::update(void){
    if(!m_enable) return;

    ControlImpl::update();
}

void Label::draw(void){
    if(!m_visible || m_ttfText == nullptr) return;

    SRect targetRect = m_rect;

    targetRect.left = m_translatedPos.x;
    targetRect.top = m_translatedPos.y;
    targetRect = mapToDrawRect(targetRect);

    if (m_shadowEnabled) {
        if(!TTF_SetTextColor(m_ttfText, m_shadowColor.r, m_shadowColor.g, m_shadowColor.b, m_shadowColor.a)) {
            SDL_Log("Failed to set shadow text color: %s", SDL_GetError());
        }
        if (!TTF_DrawRendererText(m_ttfText,
                                    targetRect.left + m_shadowOffset.x * m_xxScale,
                                    targetRect.top + m_shadowOffset.y * m_yyScale)) {
            SDL_Log("Failed to render shadow text: %s", SDL_GetError());
        }
    }

    SDL_Color color;
    switch(m_state) {
        case LabelState::Hover:
            color = m_hoverStateColor;
            break;
        case LabelState::Pressed:
            color = m_pressedStateColor;
            break;
        case LabelState::Normal:
        default:
            color = m_normalStateColor;
            break;
    }
    if(!TTF_SetTextColor(m_ttfText, color.r, color.g, color.b, color.a)) {
        SDL_Log("Failed to set text color: %s", SDL_GetError());
    }

    if (!TTF_DrawRendererText(m_ttfText, targetRect.left, targetRect.top)) {
        SDL_Log("Failed to render text: %s", SDL_GetError());
    }

    ControlImpl::draw();
}

bool Label::handleEvent(shared_ptr<Event> event){
    if(!m_enable || !m_visible) return false;

    if (ControlImpl::handleEvent(event)) return true;

    if (EventQueue::isPositionEvent(event->m_eventName)){
        shared_ptr<SPoint> pos = any_cast<shared_ptr<SPoint>>(event->m_eventParam);
        // SRect drawRect = getDrawRect(); // 获取当前控件的绘制区域
        SRect detectRect = mapToDrawRect(m_hotRect); // 将热区映射到绘制区域
        if (detectRect.contains(pos->x, pos->y)){
            switch(event->m_eventName){
                case EventName::FINGER_DOWN:
                case EventName::FINGER_MOTION:
                    // Todo: 如果是触控，一般是要考虑连续触发，所以这里没有像下面MOUSE_LBUTTON_DOWN那样处理成判断按钮状态，但这里最好做成参数控制
                    if (m_onClick != nullptr){
                        m_onClick(dynamic_pointer_cast<Label>(this->getThis()));
                    }
                    m_state = LabelState::Pressed;
                    return true;
                case EventName::MOUSE_LBUTTON_DOWN:
                    m_state = LabelState::Pressed;
                    return true;
                case EventName::MOUSE_LBUTTON_UP:
                    if (m_onClick != nullptr && m_state == LabelState::Pressed){
                        m_onClick(dynamic_pointer_cast<Label>(this->getThis()));
                    }
                case EventName::FINGER_UP:
                    m_state = LabelState::Normal;
                    return true;
                case EventName::MOUSE_MOVING:
                // case EventName::FINGER_MOTION:
                    m_state = LabelState::Hover;
                    if(m_hoverCursor != nullptr){
                        SDL_SetCursor(m_hoverCursor);
                    }
                    return true;
                default:
                    break;
            }
            return true;
        } else {
            m_state = LabelState::Normal;
            if(m_defaultCursor){
                SDL_SetCursor(m_defaultCursor);
            }
        }
    }
    return false;
}

int Label::getId(void) const{
    return m_id;
}

SRect Label::getHotRect(void) const{
    return m_hotRect;
}

/*********************************************************for Builder mode**********************************************************/

Label& Label::setNormalStateColor(SDL_Color color){
    m_normalStateColor = color;
    return *this;
}
Label& Label::setHoverStateColor(SDL_Color color){
    m_hoverStateColor = color;
    return *this;
}
Label& Label::setPressedStateColor(SDL_Color color){
    m_pressedStateColor = color;
    return *this;
}
Label& Label::setCaption(string caption){
    m_caption = caption;

    if (m_font == nullptr || m_textEngin == nullptr) return *this;

    if (m_ttfText == nullptr) {
        m_ttfText = TTF_CreateText(m_textEngin, m_font, caption.c_str(), caption.length());
        if (m_ttfText == nullptr) {
            SDL_Log("Failed to create static text: %s", SDL_GetError());
            throw "Failed to create static text: %s", SDL_GetError();
            return *this;
        }
    } else {
        if(!TTF_SetTextString(m_ttfText, caption.c_str(), caption.length())) {
            SDL_Log("Failed to set text string: %s", SDL_GetError());
            throw "Failed to set text string: %s", SDL_GetError();
            return *this;
        }
    }

    int width, height;
    if (!TTF_GetTextSize(m_ttfText, &width, &height)){
        SDL_Log("Failed to get text size: %s", SDL_GetError());
        throw "Failed to get text size: %s", SDL_GetError();
        return *this;
    }
    m_textSize = {static_cast<float>(width / getScaleXX()), static_cast<float>(height / getScaleYY())};  // 除以倍率获得原始大小，后续用来计算对齐时才能正确计算;

    // 重新计算对齐位置
    setAlignmentMode(m_AlignmentMode);
    return *this;
}
Label& Label::setFont(FontName fontName){
    m_fontName = fontName;
    m_fontFile = fs::path(ResourceLoader::m_fontFiles[fontName]);
    return *this;
}
Label& Label::setAlignmentMode(AlignmentMode Alignment){
    m_AlignmentMode = Alignment;
    switch (m_AlignmentMode) {
        case AlignmentMode::AM_TOP_CENTER:
            m_translatedPos = {(m_rect.width - m_textSize.width) / 2, 0};
            break;
        case AlignmentMode::AM_TOP_RIGHT:
            m_translatedPos = {m_rect.width - m_textSize.width, 0};
            break;
        case AlignmentMode::AM_MID_LEFT:
            m_translatedPos = {0, (m_rect.height - m_textSize.height) / 2};
            break;
        case AlignmentMode::AM_CENTER:
            m_translatedPos = {(m_rect.width - m_textSize.width) / 2, (m_rect.height - m_textSize.height) / 2};
            break;
        case AlignmentMode::AM_MID_RIGHT:
            m_translatedPos = {m_rect.width - m_textSize.width, (m_rect.height - m_textSize.height) / 2};
            break;
        case AlignmentMode::AM_BOTTOM_LEFT:
            m_translatedPos = {0, m_rect.height - m_textSize.height};
            SDL_Log("(%f, %f, %f, %f), (%f, %f),  bottom left: (%f, %f)", m_rect.left, m_rect.top, m_rect.width, m_rect.height,
                m_textSize.width, m_textSize.height, m_translatedPos.x, m_translatedPos.y);
            break;
        case AlignmentMode::AM_BOTTOM_CENTER:
            m_translatedPos = {(m_rect.width - m_textSize.width) / 2, m_rect.height - m_textSize.height};
            break;
        case AlignmentMode::AM_BOTTOM_RIGHT:
            m_translatedPos = {m_rect.width - m_textSize.width, m_rect.height - m_textSize.height};
            break;
        case AlignmentMode::AM_TOP_LEFT:
        default:
            m_translatedPos = {0, 0};
            break;
    }
    m_hotRect = {m_translatedPos.x, m_translatedPos.y, m_textSize.width, m_textSize.height};
    return *this;
}
Label& Label::setFontSize(int fontSize){
    if (fontSize == m_fontSize) return *this;
    m_fontSize = fontSize;

    if (m_font == nullptr) return *this;
    if(!TTF_SetFontSize(m_font, fontSize * getScaleXX())) {
        SDL_Log("Failed to set font size: %s", SDL_GetError());
        throw "Failed to set font size: %s", SDL_GetError();
        return *this;
    }

    // Todo: 下面需要对已经生成的参数重新计算一遍
    setCaption(m_caption);
    return *this;
}
Label& Label::setShadow(bool enabled){
    m_shadowEnabled = enabled;
    return *this;
}
Label& Label::setShadowColor(SDL_Color color){
    m_shadowColor = color;
    return *this;
}
Label& Label::setShadowOffset(SPoint offset){
    m_shadowOffset = offset;
    return *this;
}
Label& Label::setOnClick(OnClickHandler handler){
    m_onClick = handler;
    return *this;
}
Label& Label::setId(int id){
    m_id = id;
    return *this;
}
Label& Label::SetFontStyle(TTF_FontStyleFlags fontStyle){
    m_fontStyle = fontStyle;

    if (m_font == nullptr) return *this;

    TTF_SetFontStyle(m_font, fontStyle); // Todo: 下面需要对已经生成的参数重新计算一遍
    setCaption(m_caption);
    return *this;
}

shared_ptr<Label> Label::build(void){
    auto newLabel = make_shared<Label>(this->getParent(), m_rect, m_xScale, m_yScale);
    newLabel->setFontSize(m_fontSize);
    newLabel->setFont(m_fontName);
    newLabel->loadFromResource(m_fontFile.string());
    newLabel->setNormalStateColor(m_normalStateColor);
    newLabel->setHoverStateColor(m_hoverStateColor);
    newLabel->setPressedStateColor(m_pressedStateColor);
    newLabel->setCaption(m_caption);
    newLabel->SetFontStyle(m_fontStyle);
    newLabel->setAlignmentMode(m_AlignmentMode);
    newLabel->setFontSize(m_fontSize);
    newLabel->setShadow(m_shadowEnabled);
    newLabel->setShadowColor(m_shadowColor);
    newLabel->setShadowOffset(m_shadowOffset);
    newLabel->setOnClick(m_onClick);
    newLabel->setId(m_id);
    return newLabel;
}

LabelBuilder::LabelBuilder(Control *parent, SRect rect, float xScale, float yScale):
    m_label(nullptr)
{
    m_label = make_shared<Label>(parent, rect, xScale, yScale);
}
LabelBuilder& LabelBuilder::setNormalStateColor(SDL_Color color){
    m_label->setNormalStateColor(color);
    return *this;
}
LabelBuilder& LabelBuilder::setHoverStateColor(SDL_Color color){
    m_label->setHoverStateColor(color);
    return *this;
}
LabelBuilder& LabelBuilder::setPressedStateColor(SDL_Color color){
    m_label->setPressedStateColor(color);
    return *this;
}
LabelBuilder& LabelBuilder::setCaption(string caption){
    m_label->setCaption(caption);
    return *this;
}
LabelBuilder& LabelBuilder::setFont(FontName fontName){
    m_label->setFont(fontName);
    m_label->loadFromResource(m_label->m_fontFile.string());
    return *this;
}
LabelBuilder& LabelBuilder::setAlignmentMode(AlignmentMode Alignment){
    m_label->setAlignmentMode(Alignment);
    return *this;
}
LabelBuilder& LabelBuilder::setFontSize(int fontSize){
    m_label->setFontSize(fontSize);
    return *this;
}
LabelBuilder& LabelBuilder::setShadow(bool enabled){
    m_label->setShadow(enabled);
    return *this;
}
LabelBuilder& LabelBuilder::setShadowColor(SDL_Color color){
    m_label->setShadowColor(color);
    return *this;
}
LabelBuilder& LabelBuilder::setShadowOffset(SPoint offset){
    m_label->setShadowOffset(offset);
    return *this;
}
LabelBuilder& LabelBuilder::setOnClick(Label::OnClickHandler handler){
    m_label->setOnClick(handler);
    return *this;
}
LabelBuilder& LabelBuilder::setId(int id){
    m_label->setId(id);
    return *this;
}

LabelBuilder& LabelBuilder::SetFontStyle(TTF_FontStyleFlags fontStyle){
    m_label->SetFontStyle(fontStyle);
    return *this;
}

shared_ptr<Label> LabelBuilder::build(void){
    return m_label;
}