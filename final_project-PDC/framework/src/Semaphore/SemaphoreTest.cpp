/*********************************
 * Reviewer:  				
 * Author: Daniel Binyamin			
 * File: Semaphore_test.cpp				
 * ******************************/

#include <thread>
#include <iostream>
#include <unistd.h>
#include "Semaphore.hpp" 

using namespace ilrd;
void Waiter();
void poster();

Semaphore sem(1);

int main()
{
    
	std::thread t1 (Waiter);
	poster();	


	return 0;
}

void Waiter()
{
	int i(0);
	
	while(1)
	{
		sem.Wait();
		std::cout<< ++i << std::endl; 
	}
}

void poster()
{
	while (1)
	{
		sem.Post();
		sleep(2);
	}
	
}



