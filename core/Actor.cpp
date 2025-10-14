#include "Actor.h"

Actor::Actor(Control *parent, float xScale, float yScale):
    Material(parent, xScale, yScale),
    m_matchParentRect(false)
{
    setParent(parent);
}

Actor::Actor(Control *parent, bool matchParentRect, float xScale, float yScale):
    Material(parent, xScale, yScale),
    m_matchParentRect(matchParentRect)
{
    // setParent(parent);
}

Actor::Actor(Control *parent, fs::path filePath, bool matchParentRect, float xScale, float yScale):
    Material(parent, filePath, xScale, yScale),
    m_matchParentRect(matchParentRect)
{
    setParent(parent);
    loadFromFile(filePath);
}

Actor::Actor(Control *parent, string resourceId, bool matchParentRect, float xScale, float yScale):
    Material(parent, resourceId, xScale, yScale),
    m_matchParentRect(matchParentRect)
{
    setParent(parent);
    loadFromResource(resourceId);
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

    loadTextureFromSurface(m_surface);
}

void Actor::loadFromResource(string resourceId) {
    shared_ptr<Resource> resource = ResourceLoader::getInstance()->getResource(resourceId);
    if (resource == nullptr || (resource->resourceType != ResourceLoader::RT_IMAGES
                            && resource->resourceType != ResourceLoader::RT_ANIMATIONS
                            && resource->resourceType != ResourceLoader::RT_BACKGROUND)
        || resource->pMem == nullptr) {

        SDL_Log("LoadFromResource Error: '%s' is not an images\n", resourceId.c_str());
        return;
    }

    SDL_IOStream *resourceStream = SDL_IOFromConstMem(resource->pMem.get(), resource->resourceSize);
    if( resourceStream == nullptr){
        SDL_Log("Create IOStream Error: %s\n", SDL_GetError());
        return;
    }
    m_surface = IMG_Load_IO(resourceStream, true);  // 这里传递了true给closeio参数，所以完成图片载入后，会自动关闭resourceStream
    if (m_surface == nullptr) {
        SDL_Log("loadFromResource Error: %s\n", SDL_GetError());
        return;
    }

    loadTextureFromSurface(m_surface);
}

void Actor::loadTextureFromSurface(SDL_Surface *surface) {
    m_rect.left = 0;
    m_rect.top = 0;
    if (!m_matchParentRect) {
        m_rect.width = (float)surface->w;
        m_rect.height = (float)surface->h;
    } else {
        m_rect.width = getParent()->getRect().width;
        m_rect.height = getParent()->getRect().height;
    }

    m_texture = SDL_CreateTextureFromSurface(getRenderer(), surface); //创建纹理
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
