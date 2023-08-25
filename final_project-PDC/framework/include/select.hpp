/*********************************
 * Reviewer:  				
 * Author: Daniel Binyamin			
 * File: select.cpp					
 * ******************************/

#pragma once
#include <sys/select.h>
#include "IMonitor.hpp"

namespace ilrd
{

class Select : public IMonitor
{
public:

	explicit Select();
	
    int WaitForEvent();
    int GetNextReadFd();
    
    void SetRead(int fd);
   // int SetWrite(int fd);
   // int SetError(int fd);
    
    void ClearRead(int fd);
   // int ClearWrite(int fd);
   // int ClearError(int fd);
     
private:

 fd_set m_read_fd;
 fd_set m_master_fd;
 int m_max_fd;
	
};

}//ilrd