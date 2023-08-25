/*********************************
 * Reviewer:  				
 * Author: Daniel Binyamin			
 * File: Factory_test.cpp				
 * ******************************/

#include "Factory.hpp" 
#include <iostream>



namespace ilrd
{


class shape 
{
public:
    shape(int param) : m_h(param){}
    virtual ~shape(){}
    virtual int getH(){return m_h;}
    virtual void SetH(int hight){m_h = hight;}
    virtual int clculate() = 0;
private:
    int m_h;

};

class squer: public shape
{
public:
    squer(int side);
    ~squer(){};
    int clculate()
    {
        return getH() * getH();
    }


private:


};

squer::squer(int side):shape(side)
{

}


class circle :public shape
{

public:
    circle(int radius);
    ~circle(){};

    int clculate()
    {
        return getH() * getH() * 3.14;
    }

};

circle::circle(int radius) : shape(radius)
{
}


} // namespace ilrd

using namespace ilrd;

int main()
{


    Factory<shape,int,int> fact;

    fact.Add<squer>(1);
    fact.Add<circle>(2);

    std::shared_ptr<shape> s1 = fact.Create(1,5);
    std::shared_ptr<shape> s2 = fact.Create(2,5);

    std::cout<<"squer area" << s1->clculate()<< std::endl;
    std::cout<<"circule area" << s2->clculate()<< std::endl;
	

	return 0;
}