#ifndef ControlH
#define ControlH
#include <vector>
#include <SDL3/SDL.h>
#include "ConstDef.h"
#include "EventQueue.h"

using namespace std;

class Control{
protected:
    // 事件队列
    EventQueue *m_eventQueueInstance;
public:
    virtual ~Control() = default;
    virtual void update(void) = 0;
    virtual void draw(void) = 0;
    virtual bool handleEvent(GameEvent &event) = 0;  //事件处理，返回值表示是否处理了该事件，true表示处理了，false表示未处理
    // virtual shared_ptr<Control> addControl(shared_ptr<Control> child) = 0;
    virtual Control& addControl(shared_ptr<Control> child) = 0;
    virtual void removeControl(shared_ptr<Control> child) = 0;
    virtual void setParent(Control *parent) = 0;
    virtual Control* getParent(void) = 0;
    virtual void setRect(SRect rect) = 0;
    virtual SRect getRect(void) = 0;
    virtual float getScaleXX(void) = 0;
    virtual float getScaleYY(void) = 0;
    virtual void setScaleX(float xScale=1.0f) = 0;
    virtual void setScaleY(float yScale=1.0f) = 0;
    virtual void show(void) = 0;
    virtual void hide(void) = 0;
    virtual void setEnable(bool enable) = 0;
    virtual bool getEnable(void) = 0;
    virtual SDL_Renderer *getRenderer(void) = 0;
    virtual void setRenderer(SDL_Renderer *renderer) = 0;
    virtual shared_ptr<Control> getThis(void) = 0;
    virtual SRect getDrawRect(void) = 0;
    virtual SRect mapToDrawRect(SRect rect) = 0;
};

class ControlImpl: virtual public Control, public enable_shared_from_this<ControlImpl>{
protected:
    bool m_visible;
    bool m_enable;
    float m_xScale;
    float m_yScale;
    float m_xxScale;
    float m_yyScale;

    SDL_Surface *m_surface;
    SDL_Renderer *m_renderer;
    SDL_Texture *m_texture;

    SRect m_rect;
    Control *m_parent;
    vector<shared_ptr<Control>> m_children; //子控件
public:
    ControlImpl(Control *parent, float xScale=1.0f, float yScale=1.0f);
    ControlImpl(const ControlImpl& other);
    ~ControlImpl() = default;
    ControlImpl& operator=(const ControlImpl& other);
    void update(void) override;
    void draw(void) override;
    bool handleEvent(GameEvent &event) override;
    // shared_ptr<Control> addControl(shared_ptr<Control> child) override;
    Control& addControl(shared_ptr<Control> child) override;
    void removeControl(shared_ptr<Control> child) override;
    void setParent(Control *parent) override;
    Control* getParent(void) override;
    float getScaleXX(void) override;
    float getScaleYY(void) override;
    void setScaleX(float xScale=1.0f) override;
    void setScaleY(float yScale=1.0f) override;
    void setRect(SRect rect) override;
    SRect getRect(void) override;
    void show(void) override;
    void hide(void) override;
    void setEnable(bool enable) override;
    bool getEnable(void) override;
    SDL_Renderer *getRenderer(void) override;
    void setRenderer(SDL_Renderer *renderer) override;
    shared_ptr<Control> getThis(void) override;
    SRect getDrawRect(void) override;
    SRect mapToDrawRect(SRect rect) override;

    void triggerEvent(GameEvent event);
    void inheritRenderer(void);
};

/*主界面需要继承该类，以支持事件列队的处理入口eventLoopEntry*/
class TopControl: virtual public Control{
public:
    TopControl(void){m_eventQueueInstance = EventQueue::getInstance();}
    void eventLoopEntry(void){
        GameEvent *eventInQueue = m_eventQueueInstance->popEventFromQueue();
        while(eventInQueue != nullptr){
            handleEvent(*eventInQueue);
            // Todo: 目前只有PositionEvent需要释放内存，看下是否能优化为shared_ptr，避免内存泄漏
            // if(eventInQueue->isPositionEvent()){
            //     SPoint *pos = (SPoint*)eventInQueue->m_eventParam;
            //     delete pos;
            // }
            delete eventInQueue;
            eventInQueue = m_eventQueueInstance->popEventFromQueue();
        }
    }
};
#endif  // ControlH