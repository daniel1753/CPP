/*********************************
 * Reviewer:  		
 * Author: Daniel Binyamin			
 * File: Semaphore.cpp					
 * ******************************/

#include "Semaphore.hpp"

namespace ilrd
{
    


Semaphore::Semaphore(int count): m_count(count){}

void Semaphore::Post()
{
    std::unique_lock<std::mutex> unique_mutex (m_mtx);
    ++m_count;
    unique_mutex.unlock();
    m_cv.notify_one();
}

void Semaphore::Wait()
{
    std::unique_lock<std::mutex> unique_mutex (m_mtx);
    while(m_count== 0)
    {
        m_cv.wait(unique_mutex);

    }
    --m_count;   
}

} // namespace ilrd