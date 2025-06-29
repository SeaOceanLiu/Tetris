#ifndef StateMachineH
#define StateMachineH

#include <any>
#include <functional>
#include <unordered_map>

#include <SDL3/SDL.h>

using namespace std;

enum class State: int;      // 使用时，需要重新定义该枚举值
enum class EventName: int;  // 使用时，需要重新定义该枚举值

class Event{
public:
    EventName m_eventName;
    std::any m_eventParam;

    Event(EventName eventName, std::any param):
        m_eventName(eventName),
        m_eventParam(param)
    {
    }
    // 拷贝构造函数
    Event(const Event& event):
        m_eventName(event.m_eventName),
        m_eventParam(event.m_eventParam)
    {
    }
    // 移动构造函数
    Event(Event&& event):
        m_eventName(event.m_eventName),
        m_eventParam(event.m_eventParam)
    {
    }
    // 赋值运算符重载
    Event& operator=(const Event& event){
        m_eventName = event.m_eventName;
        m_eventParam = event.m_eventParam;
        return *this;
    }
    // 移动赋值运算符重载
    Event& operator=(Event&& event){
        m_eventName = event.m_eventName;
        m_eventParam = event.m_eventParam;
        return *this;
    }

    virtual ~Event(){};
};

// StateMachine 模板类
class StateMachine
{
    using EnterLeaveHandler = std::function<void (State)>;
    using EventHandler = std::function<bool(shared_ptr<Event>)>;

protected:
    State m_currentState;
    std::unordered_map<State, EnterLeaveHandler> m_enterStateHandlers;
    std::unordered_map<State, EnterLeaveHandler> m_leaveStateHandlers;
    std::unordered_map<State, EventHandler> m_stateEventHandlers;
public:
    StateMachine(State initialState):
        m_currentState(initialState)
    {
    }
    virtual ~StateMachine(){};

    void registerEnterStateHandler(State state, EnterLeaveHandler handler){
        m_enterStateHandlers[state] = handler;
    }
    void registerLeaveStateHandler(State state, EnterLeaveHandler handler){
        m_leaveStateHandlers[state] = handler;
    }
    // 注册状态事件处理程序
    void registerStateEventHandler(State state, EventHandler handler) {
        m_stateEventHandlers[state] = handler;
    }

    State getState(void){
        return m_currentState;
    }

    void setState(State state){
        if (m_currentState == state){
            return;
        }
        // SDL_Log("Leave state %d", static_cast<int>(m_currentState));
        auto it = m_leaveStateHandlers.find(m_currentState);
        if (it != m_leaveStateHandlers.end()) {
            it->second(state);
        }
        auto lastState = m_currentState;
        m_currentState = state;
        // SDL_Log("Enter state %d", static_cast<int>(m_currentState));
        it = m_enterStateHandlers.find(m_currentState);
        if (it != m_enterStateHandlers.end()) {
            it->second(lastState);
        }
    }

    // 触发状态事件
    bool stateEvent(shared_ptr<Event> event) {
        auto it = m_stateEventHandlers.find(m_currentState);
        if (it != m_stateEventHandlers.end()) {
            return it->second(event);
        } else {
            SDL_Log("No handler for event %d in state %d", static_cast<int>(event->m_eventName), static_cast<int>(m_currentState));
            return false;
        }
    }
};

#endif // StateMachineH