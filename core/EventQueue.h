#ifndef EventQueueH
#define EventQueueH

#include <queue>
#include <mutex>
#include "ConstDef.h"
#include "StateMachine.h"

enum class EventName: int{
    None,

    FINGER_DOWN,
    FINGER_UP,
    FINGER_MOTION,

    MOUSE_LBUTTON_DOWN,
    MOUSE_LBUTTON_UP,
    MOUSE_RBUTTON_DOWN,
    MOUSE_RBUTTON_UP,
    MOUSE_MBUTTON_DOWN,
    MOUSE_MBUTTON_UP,
    MOUSE_MOVING,

    Begin,  //KeyEvent: S
    Paused, //KeyEvent: P
    GridOnOff,  //KeyEvent: Delete

    MoveDown,   //KeyEvent: Down
    MoveLeft,   //KeyEvent: Left
    MoveRight,  //KeyEvent: Right
    Rotate,     //KeyEvent: Space
    SpeedUp,    //KeyEvent: U

    Fillfull,   //ScreenEvent: Fillfull
    Draw,       //SystemEvent: Draw
    Update,     //SystemEvent: Update
    Timer,      //SystemEvent: Timer

    AnimationEnded, //Animationvent: AnimationEnded. Todo: 在事件的参数中增加id来表示是哪个动画
    AudioEnded,
    NextMusic,
    NextBackground,

    Exit
};

class GameEvent: public Event<void *> {
public:
    GameEvent(): Event<void *>(EventName::None, 0){}
    // 构造函数
    GameEvent(EventName eventName, void *param): Event<void *>(eventName, param) {}
    // 拷贝构造函数
    GameEvent(const GameEvent &event): Event<void *>(event) {}
    // 移动构造函数
    GameEvent(GameEvent &&event): Event<void *>(event) {}
    // 拷贝赋值运算符
    GameEvent &operator=(const GameEvent &event) {
        Event<void *>::operator=(event);
        return *this;
    }
    // 移动赋值运算符
    GameEvent &operator=(GameEvent &&event) {
        Event<void *>::operator=(event);
        return *this;
    }
    ~GameEvent() {}

    bool isPositionEvent(void) const {
        switch(m_eventName){
            case EventName::FINGER_DOWN         :
            case EventName::FINGER_UP           :
            case EventName::FINGER_MOTION       :
            case EventName::MOUSE_LBUTTON_DOWN  :
            case EventName::MOUSE_LBUTTON_UP    :
            case EventName::MOUSE_RBUTTON_DOWN  :
            case EventName::MOUSE_RBUTTON_UP    :
            case EventName::MOUSE_MBUTTON_DOWN  :
            case EventName::MOUSE_MBUTTON_UP    :
            case EventName::MOUSE_MOVING        :
                return true;
            default:
                return false;
        }
    }
};

class EventQueue{
private:
    static std::mutex m_mtxForEventQueue;
    std::queue<GameEvent *> m_eventQueue;
    EventQueue(){}
    ~EventQueue(){clear();}
public:
    static EventQueue* getInstance(void){
        static EventQueue instance; // 静态局部变量，程序运行期间只会被初始化一次
        return &instance;
    }
    void pushEventIntoQueue(GameEvent *event);
    GameEvent* popEventFromQueue(void);
    void clear(void);
};
#endif