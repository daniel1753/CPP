/*********************************
 * Reviewer:  				
 * Author: Daniel Binyamin			
 * File: monitor.cpp					
 * ******************************/

#pragma once
#include <sys/select.h>

namespace ilrd
{

class IMonitor
{
public:
    virtual int WaitForEvent() = 0;
    virtual int GetNextReadFd() = 0;
    
    virtual void SetRead(int fd) = 0;
    virtual void ClearRead(int fd) = 0;
   
};



}/* ilrd */