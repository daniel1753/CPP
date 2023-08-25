/*********************************
 * Reviewer:  		
 * Author: Daniel Binyamin			
 * File: ThreadPool.cpp					
 * ******************************/

#include <iostream>
#include <csignal>

#include "ThreadPool.hpp"

#define BADAPPALE -0xBADA

namespace ilrd
{

int BadApple(int num);

ThreadPool::ThreadPool(int num_of_threads):m_cancel(0) ,m_threads(num_of_threads)
{
    for (int i = 0; i < num_of_threads ; i++)
    {
        m_threads[i] = (std::thread(&ThreadPool::TaskDispatcher, this));
    }
}

ThreadPool::~ThreadPool()
{
    size_t i;
    if(m_cancel == 0)
    { 
        for(i =0 ; i < m_threads.size()  ;++i )
        {
            pthread_kill(m_threads[i].native_handle(),SIGTERM);
        }
    }

}

ThreadPool::Future ThreadPool::Async(const Function<int(void)>& func)
{
    std::shared_ptr<Task> task_ptr(new Task(func));
    m_tasks.Enqueue(task_ptr);
    return (Future(task_ptr));

}

void ThreadPool::ShutDown()
{
    Function<int(void)> badApp(BadApple, 0);
    for (size_t i = 0; i < m_threads.size() ; ++i)
    {
        Async(badApp);
    }
    m_cancel = 1;

    for (size_t i = 0; i < m_threads.size() ; ++i)
    {
        m_threads[i].join();

    }
}

void ThreadPool::Cancel()
{
    m_tasks.Clear();
    ShutDown();
}

void ThreadPool::TaskDispatcher()
{
    std::shared_ptr<Task> task_ptr;
    int run_stat = 1;
    while (run_stat != BADAPPALE)
    {
        m_tasks.Dequeue(task_ptr);
        run_stat = (*task_ptr)();
    }
}

int BadApple(int num)
{
    (void)num;
    return BADAPPALE;
}


/************************************ task ************************************/
    
ThreadPool::Task::Task(const Function<int(void)> &func ,int priority):
                       m_function(func),m_status(0),
                       m_priority(priority)
{}

void ThreadPool::Task::SemWait()
{
    m_sem.Wait();
}

void ThreadPool::Task::SemPost()
{
    m_sem.Post();
}

int ThreadPool::Task::Status()
{
    return m_status;
}

int ThreadPool::Task::operator()()
{
    m_status = m_function();
    SemPost();
    return m_status;
}

/*********************************** Future ***********************************/

ThreadPool::Future::Future(const std::shared_ptr<Task> &s_ptr): m_ptr(s_ptr)
{}

int ThreadPool::Future::Wait() const
{
    m_ptr->SemWait();
    return m_ptr->Status();
}

} // namespace ilrd
