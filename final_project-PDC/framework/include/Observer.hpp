/*********************************
 * Reviewer:  				
 * Author: Daniel Binyamin			
 * File: Observer.cpp					
 * ******************************/

#pragma once
#include <unordered_map>
#include <memory>
#include <set>
#include <stdint.h> /* uint32_t */
#include <iostream>

#include "function.hpp"

namespace ilrd
{

template<typename RET>
class Callback;


template<typename EVENT>
class Dispatcher
{

public:
	~Dispatcher();
	void Add(const Callback<EVENT>& callback);
    void Remove(const Callback<EVENT>& callback) ;
	void Notify(EVENT event); 
    
private:
	std::set<const Callback<EVENT>*> m_callbacks;

};

template<typename EVENT>
class Callback
{
public:
	Callback(const Function<void(EVENT)>& func);
	~Callback();
private:

	friend Dispatcher<EVENT>;

	Function<void(EVENT)> m_function;
	mutable Dispatcher<EVENT>* m_dispatcher;
};




template <typename EVENT>
inline Dispatcher<EVENT>::~Dispatcher()
{
	m_callbacks.erase(m_callbacks.begin(), m_callbacks.end());
}

template <typename EVENT>
inline void Dispatcher<EVENT>::Add(const Callback<EVENT> &callback)
{
	m_callbacks.insert(&callback); 
	callback.m_dispatcher = this;
	
}

template <typename EVENT>
inline void Dispatcher<EVENT>::Remove(const Callback<EVENT> &callback) 
{
	m_callbacks.erase(&callback); 

}

template <typename EVENT>
inline void Dispatcher<EVENT>::Notify(EVENT event)
{
	for(typename std::set<const Callback<EVENT>*>::const_iterator iter = 
													m_callbacks.begin(); 
													iter != m_callbacks.end(); 
													++iter)
	{
		(*iter)->m_function(event);
	}
	
}

template <typename EVENT>
inline Callback<EVENT>::Callback(const Function<void(EVENT)> &func)
{
	m_function = func;
}

template <typename EVENT>
inline Callback<EVENT>::~Callback()
{
	m_dispatcher->Remove(*this); 
}



}