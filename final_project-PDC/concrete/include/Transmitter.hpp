/*********************************
 * Reviewer:  				
 * Author: Daniel Binyamin			
 * File: Transmitter.cpp					
 * ******************************/

#pragma once
 
#include <set>
#include <memory>

#include "Command.hpp"
#include "Observer.hpp"

namespace ilrd
{
    
class Transmitter
{
public:

    Transmitter(NbdRequest *NbdReq);
    int SendTask(std::shared_ptr<message_info> thread_info);
    int ReplyHandler(int sock);
    static void Add(const Callback<AtlasHeader *> &callback);

private:

    std::set<uint32_t> m_iot_set;
    static Dispatcher<AtlasHeader*> m_disp;
    NbdRequest *m_NbdReq;
};





} // namespace ilrd
