/*********************************
 * Reviewer:  				
 * Author: Daniel Binyamin			
 * File: reactor_test.cpp				
 * ******************************/
#include "function.hpp"
#include "select.hpp"
#include "reactor.hpp" 
#include <iostream>

using namespace ilrd;


int heandler(int a)
{
	a+=1;
	std::cout<< "a is:" << a << std::endl;
	return a;
}

int main()
{

	/* Select s1; */
	Reactor<Select> r;
	
	r.RegisterReadHandler(0,Function<int(void)>(&heandler, 5));
	r.Run();


	return 0;
}
