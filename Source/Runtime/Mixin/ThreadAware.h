#pragma once

#include <thread>

class ThreadAware
{
    std::thread::id m_owningThreadID;
protected:
    bool IsOwningThread() const { return m_owningThreadID == std::this_thread::get_id();}
public:
    ThreadAware() : m_owningThreadID(std::this_thread::get_id()) {}
};