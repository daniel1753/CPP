/*********************************
 * Reviewer:  		
 * Author: Daniel Binyamin			
 * File: select.cpp					
 * ******************************/

#include <cstddef>      /* NULL */
#include <sys/select.h> /* select, FD_ZERO */
#include <iostream>
#include "select.hpp"

#define MAX(a, b) (a > b ? a : b)

namespace ilrd
{

Select::Select()
{
    FD_ZERO(&m_read_fd);
    FD_ZERO(&m_master_fd);
    m_max_fd = 0;
}

void Select::SetRead(int fd)
{
    FD_SET(fd, &m_master_fd);
    m_max_fd = m_max_fd < fd ? fd : m_max_fd;

}

void Select::ClearRead(int fd)
{
    FD_CLR(fd, &m_master_fd);
}

int Select::WaitForEvent()
{
    m_read_fd = m_master_fd;
    return select(m_max_fd + 1, &m_read_fd, NULL, NULL, NULL);
}

int Select::GetNextReadFd()
{
    int i =0;

    for ( i = 0; i <= m_max_fd; i++)
    {
        if (FD_ISSET(i , &m_read_fd))
        {
            FD_CLR(i, &m_read_fd);
            return i;
        }  
    } 
    return 0;
}
    

}