
#include <cstdlib> /* malloc */
#include <cstring> /* memcpy */
#include <iostream>/* cout */
#include <unistd.h>
#include <thread>
#include <vector>

/* #include "nbd_comm.h"
#include "reactor.hpp"
#include "function.hpp"
#include "select.hpp"  */

#include "BtsQueue.hpp"
#include "function.hpp"

#define NUM_OF_CONSUMERS 10

using namespace ilrd;



// threadPool
/* int main()
{
    
    return 0;
}

 */





/* BtsQueue */
/*
void producer(BtsQueue<int> &queue, int id);
void consumer(BtsQueue<int> &queue, int id);
std::mutex mx;

int main()
{
    BtsQueue<int> q;
    std::thread cons[10];
    std::thread prod(producer, std::ref(q), 100);
    

    for (int i = 0; i < 10; ++i)
    {
        cons[i]= std::thread(consumer, std::ref(q), i);
    }

    prod.join();
    for (int i = 0; i < 10; ++i) 
    {
        cons[i].join();
    }

    return 0;
}

void producer(BtsQueue<int> &queue, int id)
{
    for (int i = 0; i < 10; i++)
    {
        int value = id * 10 + i;
        //mx.lock();
        std::cout << "Producer " << id << " producing " << value << std::endl;
       // mx.unlock();
        
        queue.Enqueue(value);
    }
}

 void consumer(BtsQueue<int> &queue, int id)
{

    int value; 
    queue.Dequeue(value);        

    //mx.lock();
    std::cout << "Consumer " << id << " consuming " << value << std::endl;
    //mx.unlock();

        
} */

/* finction whit inovation arg*/


int Bar(float arg2 )
{
	std::cout<< "this is a free func "<< arg2 << std::endl;
	return 1;

}

class Wigt
{
public:

	explicit Wigt();
	int Foo(int b);

private:
	int m_a;

};


int main()
{
	int arg1 =1;
	Wigt w1;
	Function<int(int)>f1(&Wigt::Foo, &w1);
	Function<int(float)>f2(&Bar);

	f1(5);
	f2(2.5);
	
	return 0;
}

Wigt::Wigt(): m_a(5){}

int Wigt::Foo(int b)
{
	std::cout<< "this is a member func "<< b<< std::endl;
    return b;
}


/* finction no inovation arg*/

/* int Bar(const char *arg2 )
{
	std::cout<< "this is a free func "<< arg2 << std::endl;
	return 1;

}

class Wigt
{
public:

	explicit Wigt(int a =5);
	int Foo(int b);

private:
	int m_a;

};


int main()
{
	int arg1 =1;
	Wigt w1(arg1);
	Function<int(void)>f1(&Wigt::Foo, &w1,arg1);
	Function<int(void)>f2(&Bar, "hello");

	f1();
	f2();
	
	return 0;
}

Wigt::Wigt(int a): m_a(a){}

int Wigt::Foo(int b)
{
	std::cout<< "this is a member func "<< b<< std::endl;
    return b;
}
 */