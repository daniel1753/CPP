/*********************************
 * Reviewer:  				
 * Author: Daniel Binyamin			
 * File: RequestHost.cpp					
 * ******************************/

#pragma once
#include <stdint.h> /* uint32_t */
#include <sys/socket.h>      /* socket ,sendmsg , iov ,msg*/

namespace ilrd
{

const uint32_t IOT_COUNT = 8 ;
const uint32_t FREG_SIZE = 1024;

enum E_STATUS {FAIL, SUCCESS, DONE};
enum E_PEEK {REQ_UID, FREG_IDX};
enum E_INFO {FREG_COUNT, IOT, FREG_OFFSET};

}//namespace ilrd