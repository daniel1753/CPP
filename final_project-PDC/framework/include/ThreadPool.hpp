/*********************************
 * Reviewer:
 * Author: Daniel Binyamin
 * File: ThreadPool.cpp
 * ******************************/

#pragma once

#include <vector> /* std::vector */
#include <thread> /* std::thread */
#include <future>    /* std::async std::future */
#include <memory> /* std::sharedpointer */


#include "BtsQueue.hpp"
#include "function.hpp"
#include "Semaphore.hpp"

namespace ilrd
{

class ThreadPool
{

public:
    class Future;

    explicit ThreadPool(int num_of_threads = 8);
    ~ThreadPool();
    ThreadPool::Future Async(const Function<int(void)> &func);

    void ShutDown(); // terminate New tasks! finishes what currently in queue
    void Cancel();   // cancel all tasks and empties the queue

    ThreadPool(const ThreadPool &other) = delete;
    void operator=(const ThreadPool &other) = delete;


private:
class Task;
class Compare
{
public:
    bool operator()(const Function<int(void)> &lhs, 
                    const Function<int(void)> &rhs)
    {
        (void)lhs;
        (void)rhs;
        return true;
    }
};
    void TaskDispatcher();

    int m_cancel;
    std::vector<std::thread> m_threads;
    BtsQueue<std::shared_ptr<Task>> m_tasks;

}; // class thread pool


/************************************ task ************************************/

class ThreadPool::Task
{
public:
    explicit Task(const Function<int(void)> &func, int priority = 0);
    /* generte cctor, dtor and assigment oper by the compiler */

    /* int Priority(); */
    void SemWait();
    void SemPost();
    int operator()();
    int Status();
    
private: 

    Function<int(void)> m_function;
    int m_status;
    int m_priority;
    Semaphore m_sem;

}; // class Task


/*********************************** Future ***********************************/


class ThreadPool::Future
{
public:
    explicit Future(const std::shared_ptr<Task> &s_ptr = nullptr);
    int Wait() const;

private:
    std::shared_ptr<Task> m_ptr;
    
};//class Future

} // namespace ilrd