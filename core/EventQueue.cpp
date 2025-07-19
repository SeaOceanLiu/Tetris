#include "EventQueue.h"

std::mutex EventQueue::m_mtxForEventQueue; // 定义静态成员变量

void EventQueue::pushEventIntoQueue(shared_ptr<Event> event){
    m_mtxForEventQueue.lock();
    m_eventQueue.push(event);
    m_mtxForEventQueue.unlock();
}

shared_ptr<Event> EventQueue::popEventFromQueue(void){
    m_mtxForEventQueue.lock();
    if (m_eventQueue.empty()){
        m_mtxForEventQueue.unlock();
        return nullptr;
    }
    shared_ptr<Event> event = m_eventQueue.front();
    m_eventQueue.pop();
    m_mtxForEventQueue.unlock();
    return event;
}
void EventQueue::clear(void){
    //m_mtxForEventQueue.lock();
    shared_ptr<Event> event = popEventFromQueue();
    while(event != nullptr){
        // 改为shared_ptr后，不需要手动释放内存
        event = popEventFromQueue();
    }
    //m_mtxForEventQueue.unlock();
}