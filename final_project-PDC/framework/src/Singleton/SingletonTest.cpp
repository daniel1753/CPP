/*********************************
 * Reviewer:  				
 * Author: Daniel Binyamin			
 * File: Singleton_test.cpp				
 * ******************************/

#include "Singleton.hpp" 
#include "ThreadPool.hpp"

using namespace ilrd;

int main()
{
	Singleton<ThreadPool> p1;
	Singleton<std::string> s1;
	Singleton<std::string> s2;
	
	p1->Cancel();
	

	return 0;
}
