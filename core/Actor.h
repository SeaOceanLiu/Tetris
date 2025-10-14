#ifndef ActorH
#define ActorH
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "Material.h"

class Actor: public Material{
friend class ActorBuilder;
private:
protected:
    bool m_matchParentRect; //是否强制使用目标矩形
public:
    Actor(Control *parent, float xScale=1.0f, float yScale=1.0f);
    Actor(Control *parent, bool matchParentRect=false, float xScale=1.0f, float yScale=1.0f);
    Actor(Control *parent, fs::path filePath, bool matchParentRect=false, float xScale=1.0f, float yScale=1.0f);
    Actor(Control *parent, string resourceId, bool matchParentRect=false, float xScale=1.0f, float yScale=1.0f);
    Actor(const Actor& other);
    Actor& operator=(const Actor& other);
    void loadFromFile(fs::path filePath) override;
    void loadFromResource(string resourceId) override;
    void setParent(Control *parent) override;   // 由于要考虑匹配父控件绘图区域大小，所以需要重载该函数，以使其在设备父控件时匹配父控件绘图区域大小
    void loadTextureFromSurface(SDL_Surface *surface);
};

class ActorBuilder{
private:
    shared_ptr<Actor> m_actor;
public:
    ActorBuilder(Control *parent, float xScale=1.0f, float yScale=1.0f);
    ActorBuilder& loadFromFile(fs::path filePath);
    ActorBuilder& setMatchParentRect(bool matchParentRect);
    shared_ptr<Actor> build(void);
};
#endif