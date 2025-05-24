#include "EventQueue.h"

std::mutex EventQueue::m_mtxForEventQueue; // 定义静态成员变量

void EventQueue::pushEventIntoQueue(GameEvent *event){
    m_mtxForEventQueue.lock();
    m_eventQueue.push(event);
    m_mtxForEventQueue.unlock();
}

GameEvent* EventQueue::popEventFromQueue(void){
    m_mtxForEventQueue.lock();
    if (m_eventQueue.empty()){
        m_mtxForEventQueue.unlock();
        return nullptr;
    }
    GameEvent *event = m_eventQueue.front();
    m_eventQueue.pop();
    m_mtxForEventQueue.unlock();
    return event;
}
void EventQueue::clear(void){
    //m_mtxForEventQueue.lock();
    GameEvent *event = popEventFromQueue();
    while(event != nullptr){
        // // Todo: 目前只有PositionEvent需要释放内存，看下是否能优化为shared_ptr，避免内存泄漏
        // if (event->m_eventParam != nullptr && event->isPositionEvent()){
        //     SPoint *pos = (SPoint*)event->m_eventParam;
        //     delete pos;
        // }
        delete event;
        event = popEventFromQueue();
    }
    //m_mtxForEventQueue.unlock();
}