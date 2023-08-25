/*********************************
 * Reviewer:  				
 * Author: Daniel Binyamin			
 * File: Command.cpp					
 * ******************************/


#pragma once
#include <cstdint> /* uint32_t */
#include <memory> /* shared_ptr */
#include <sys/types.h>       /* sendmsg */
#include "nbd_comm.h" 
#include "config.hpp"



namespace ilrd
{

/********************************** Command **********************************/    

class Command
{

public:
    Command(std::shared_ptr<message_info> msgInfo);
    virtual ~Command(){}
    virtual int Send(std::shared_ptr<message_info> msgInfo);
    virtual void Build(std::shared_ptr<message_info> msgInfo) = 0;
    
};

/*********************************** Write ***********************************/    

class Write : public Command
{
public:
    Write(std::shared_ptr<message_info> msgInfo);
    ~Write(){};
    void Build(std::shared_ptr<message_info> msgInfo);
    
};


/*********************************** Read ***********************************/    

class Read : public Command
{

public:
    Read(std::shared_ptr<message_info> msgInfo);
    ~Read(){};
    void Build(std::shared_ptr<message_info> msgInfo);

};


} // namespace ilrd