#include "Material.h"

Material::Material(Control *parent, float xScale, float yScale):
    ControlImpl(parent, xScale, yScale),
    m_anchorPoint({0,0}),
    m_anchorType(AnchorType::AT_TOP_LEFT),
    m_filePath()
{
}
Material::Material(Control *parent, fs::path filePath, float xScale, float yScale):
    ControlImpl(parent, xScale, yScale),
    m_anchorPoint({0,0}),
    m_anchorType(AnchorType::AT_TOP_LEFT),
    m_filePath(filePath)
{
}
Material::Material(const Material& other):
    ControlImpl(other),
    m_anchorPoint(other.m_anchorPoint),
    m_anchorType(other.m_anchorType),
    m_filePath(other.m_filePath)
{
}

Material& Material::operator=(const Material& other){
    ControlImpl::operator=(other);
    m_anchorPoint = other.m_anchorPoint;
    m_anchorType = other.m_anchorType;
    m_filePath = other.m_filePath;
    return *this;
}

Material::~Material(){
    if(m_texture != nullptr){
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
    }
}

void Material::draw(void){
    draw(m_rect.left + m_anchorPoint.x, m_rect.top + m_anchorPoint.y);
}

void Material::draw(SPoint pos, Uint8 alpha){
    draw(pos.x, pos.y, alpha);
}
void Material::draw(float posx, float posy, Uint8 alpha){
    inheritRenderer();


    SRect targetRect = getRect();
    targetRect.left = posx - m_anchorPoint.x;
    targetRect.top = posy - m_anchorPoint.y;


    SRect drawRect = mapToDrawRect(targetRect); //getDrawRect();
    SDL_FRect drawRectF = {drawRect.left, drawRect.top, drawRect.width, drawRect.height};

    if(!SDL_SetTextureBlendMode(m_texture, SDL_BLENDMODE_BLEND)){
        SDL_Log("SDL_SetTextureBlendMode Error: %s", SDL_GetError());
    }

    if(!SDL_SetTextureAlphaMod(m_texture, alpha)){
        SDL_Log("SDL_SetTextureAlphaMod Error: %s", SDL_GetError());
    }

    if(!SDL_RenderTexture(getRenderer(), m_texture, nullptr, &drawRectF)){
        SDL_Log("SDL_RenderTexture Error: %s", SDL_GetError());
        throw(SDL_GetError());
    }
}

void Material::setAnchorPoint(AnchorType anchorType){
    m_anchorType = anchorType;
    switch (anchorType){
        case AnchorType::AT_TOP_LEFT:
            setAnchorPointTopLeft();
            break;
        case AnchorType::AT_MID_LEFT:
            setAnchorPointMidLeft();
            break;
        case AnchorType::AT_BOTTOM_LEFT:
            setAnchorPointBottomLeft();
            break;
        case AnchorType::AT_TOP_RIGHT:
            setAnchorPointTopRight();
            break;
        case AnchorType::AT_MID_RIGHT:
            setAnchorPointMidRight();
            break;
        case AnchorType::AT_BOTTOM_RIGHT:
            setAnchorPointBottomRight();
            break;
        case AnchorType::AT_TOP_CENTER:
            setAnchorPointTopCenter();
            break;
        case AnchorType::AT_CENTER:
            setAnchorPointCenter();
            break;
        case AnchorType::AT_BOTTOM_CENTER:
            setAnchorPointBottomCenter();
            break;
        default:
            break;
    }
}

void Material::correctAnchorPoint(void){
    if (m_anchorType == AnchorType::AT_SPECIAL){
        return;
    }
    setAnchorPoint(m_anchorType);
}
void Material::setAnchorPoint(SPoint &anchorPoint){
    setAnchorPoint(anchorPoint.x, anchorPoint.y);
}
void Material::setAnchorPoint(float x, float y){
    m_anchorType = AnchorType::AT_SPECIAL;

    m_anchorPoint.x = x;
    m_anchorPoint.y = y;
}
void Material::setAnchorPointTopLeft(){
    m_anchorType = AnchorType::AT_TOP_LEFT;

    m_anchorPoint.x = 0;
    m_anchorPoint.y = 0;
}
void Material::setAnchorPointMidLeft(){
    m_anchorType = AnchorType::AT_MID_LEFT;

    m_anchorPoint.x = 0;
    m_anchorPoint.y = m_rect.height / 2;
}
void Material::setAnchorPointBottomLeft(){
    m_anchorType = AnchorType::AT_BOTTOM_LEFT;

    m_anchorPoint.x = 0;
    m_anchorPoint.y = m_rect.height;
}

void Material::setAnchorPointTopRight(){
    m_anchorType = AnchorType::AT_TOP_RIGHT;

    m_anchorPoint.x = m_rect.width;
    m_anchorPoint.y = 0;
}
void Material::setAnchorPointMidRight(){
    m_anchorType = AnchorType::AT_MID_RIGHT;

    m_anchorPoint.x = m_rect.width;
    m_anchorPoint.y = m_rect.height / 2;
}
void Material::setAnchorPointBottomRight(){
    m_anchorType = AnchorType::AT_BOTTOM_RIGHT;

    m_anchorPoint.x = m_rect.width;
    m_anchorPoint.y = m_rect.height;
}

void Material::setAnchorPointTopCenter(){
    m_anchorType = AnchorType::AT_TOP_CENTER;

    m_anchorPoint.x = m_rect.width / 2;
    m_anchorPoint.y = 0;
}
void Material::setAnchorPointCenter(){
    m_anchorType = AnchorType::AT_CENTER;

    m_anchorPoint.x = m_rect.width / 2;
    m_anchorPoint.y = m_rect.height / 2;
}
void Material::setAnchorPointBottomCenter(){
    m_anchorType = AnchorType::AT_BOTTOM_CENTER;

    m_anchorPoint.x = m_rect.width / 2;
    m_anchorPoint.y = m_rect.height;
}

SPoint Material::anchorPointTranslate(SPoint point){
    SPoint translatedPoint;
    translatedPoint.x = point.x - m_anchorPoint.x;
    translatedPoint.y = point.y - m_anchorPoint.y;
    return translatedPoint;
}