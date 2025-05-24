#include "Actor.h"

Actor::Actor(Control *parent, float xScale, float yScale):
    Material(parent, xScale, yScale),
    m_matchParentRect(false)
{
    setParent(parent);
}

Actor::Actor(Control *parent, fs::path filePath, bool matchParentRect, float xScale, float yScale):
    Material(parent, filePath, xScale, yScale),
    m_matchParentRect(matchParentRect)
{
    setParent(parent);
    loadFromFile(filePath);
}

Actor::Actor(const Actor& other):
    Material(other),
    m_matchParentRect(other.m_matchParentRect)
{
}

Actor& Actor::operator=(const Actor& other) {
    Material::operator=(other);
    m_matchParentRect = other.m_matchParentRect;
    return *this;
}

void Actor::loadFromFile(fs::path filePath) {
    m_surface = IMG_Load(filePath.string().c_str());
    if (m_surface == nullptr) {
        SDL_Log("LoadFromFile Error: %s\n", SDL_GetError());
        return;
    }

    m_rect.left = 0;
    m_rect.top = 0;
    if (!m_matchParentRect) {
        m_rect.width = m_surface->w;
        m_rect.height = m_surface->h;
    } else {
        m_rect.width = getParent()->getRect().width;
        m_rect.height = getParent()->getRect().height;
    }

    m_texture = SDL_CreateTextureFromSurface(getRenderer(), m_surface); //创建纹理
    if (m_texture == nullptr) {
        SDL_Log("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
        return;
    }
}

void Actor::setParent(Control *parent){
    m_parent = parent;
    if (m_matchParentRect) {
        m_rect.width = m_parent->getRect().width;
        m_rect.height = m_parent->getRect().height;

        if (m_surface != nullptr) {
            m_texture = SDL_CreateTextureFromSurface(getRenderer(), m_surface); //创建纹理
            if (m_texture == nullptr) {
                SDL_Log("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
                return;
            }
        }
    }
}


ActorBuilder::ActorBuilder(Control *parent, float xScale, float yScale):
    m_actor(nullptr)
{
    m_actor = make_shared<Actor>(parent, xScale, yScale);
}
ActorBuilder& ActorBuilder::loadFromFile(fs::path filePath){
    m_actor->loadFromFile(filePath);
    return *this;
}
ActorBuilder& ActorBuilder::setMatchParentRect(bool matchParentRect){
    m_actor->m_matchParentRect = matchParentRect;
    return *this;
}
shared_ptr<Actor> ActorBuilder::build(void){
    return m_actor;
}
