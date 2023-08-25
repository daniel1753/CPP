/*********************************
 * Reviewer:  				
 * Author: Daniel Binyamin			
 * File: reactor.cpp					
 * ******************************/


#pragma once

#include <unordered_map>

#include <cstdio>
#include "IMonitor.hpp"
#include "function.hpp"

namespace ilrd
{
template<typename MONITOR> 
class Reactor
{
public:
    explicit Reactor();

    void RegisterReadHandler(int fd, const Function<int(void)>& func);
    void Run();
    //Void Stop();
  
  
private:
    std::unordered_map<int, Function<int(void)>> m_handlers;
    MONITOR m_monit;

};


template<typename MONITOR>
inline Reactor<MONITOR>::Reactor()
{
    (void)static_cast<IMonitor *>(&m_monit);
}

template <typename MONITOR>
inline void Reactor<MONITOR>::RegisterReadHandler(int fd, const Function<int(void)> &func)
{
    m_handlers[fd] = func;
    m_monit.SetRead(fd);
}

template <typename MONITOR>
void Reactor<MONITOR>::Run()
{
    int nfd_in_set =0;

    while (1)
    {
        nfd_in_set = m_monit.WaitForEvent();
        for (;0 < nfd_in_set; --nfd_in_set)
        {
            m_handlers[m_monit.GetNextReadFd()]();
        }
    
    }
    
}

}//ilrd