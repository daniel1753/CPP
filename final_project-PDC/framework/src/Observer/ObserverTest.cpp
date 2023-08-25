/*********************************
 * Reviewer:  				
 * Author: Daniel Binyamin			
 * File: Observer_test.cpp				
 * ******************************/
#include <unistd.h>

#include "Observer.hpp" 
#include "function.hpp"
using namespace ilrd;


class Shop
{
public:
	void Subscribe(Callback<int>* cb);
	void UnSubscribe(Callback<int>* cb);
	void Notify(int event);
private:
	Dispatcher<int> product_intrested;
};

class Client
{

public:
	Client(Function<void(int)> &func);
	~Client();
	Callback<int>* get();

private:
	Callback<int>* m_callback;

};


void ClientAction1(int num)
{
	std::cout<< "client1 notiyfed , arg:"<< num <<std::endl;
}

void ClientAction2(int num)
{
	std::cout<< "client2 notiyfed , arg:"<< num << std::endl;

}
int main()
{
    Shop CShop;

	Function<void(int)> f1(ClientAction1);
	Function<void(int)> f2(ClientAction2);

	Client c1(f1);
	Client c2(f2);
	
	CShop.Subscribe(c1.get());
	CShop.Subscribe(c2.get());
    
	for(int i=0 ; i < 4 ; i++)
	{
		CShop.Notify(i);
		sleep(3);
	}
	CShop.UnSubscribe(c1.get());
	CShop.UnSubscribe(c2.get());
	
	return 0;
}



Client::Client(Function<void(int)>& func)
{
	m_callback = new Callback<int>(func);
}

Client::~Client()
{
	delete m_callback;
}

Callback<int> *Client::get()
{
    return m_callback;
}

void Shop::Subscribe(Callback<int> *cb)
{
	product_intrested.Add(*cb);
}

void Shop::UnSubscribe(Callback<int> *cb)
{
	product_intrested.Remove(*cb);
}

void Shop::Notify(int event)
{
	product_intrested.Notify(event);
}
