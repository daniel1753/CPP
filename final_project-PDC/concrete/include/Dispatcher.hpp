/*********************************
 * Reviewer:  				
 * Author: Daniel Binyamin			
 * File: Dispatcher.cpp					
 * ******************************/

#pragma once
 
#include <memory> /* shared_ptr */
#include <unordered_map> /* unordered_map */

#include "RequestHost.hpp" /* RequestHost */
#include "config.hpp"

namespace ilrd
{

class RequestDispatcher
{

public:

    int NbdRequestHandler(int *sokets);
    int IotRequestHandler(int sock);

private:

    std::unordered_map<uint32_t,std::shared_ptr<RequestHost>> m_activeRequest;
};



}