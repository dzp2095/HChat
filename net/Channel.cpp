//
// Created by 邓志鹏 on 02/03/2020.
//

#include "Channel.h"
#include "poll.h"
#include "EventLoop.h"
#include "Logging.hpp"

namespace HChat{
	
	#ifndef POLLRDHUP
	#define POLLRDHUP 0x2000
	#endif
	
//    const int Channel::kNoneEvent = 0;
//    const int Channel::kReadEvent = POLLIN | POLLPRI;
//    const int Channel::kWriteEvent = POLLOUT;

    Channel::Channel(HChat::EventLoop *loop, int fd)
        :loop(loop),
        fd(fd),
        events(0),
        revents(0),
        index_4poller(kNew),
        addedToLoop(false){
    
    }
	Channel:: ~Channel() = default;
    
    void Channel::update() {
    	addedToLoop= true;
        loop->updateChannel(this);
    }
	
	void Channel::handleEvent() {
		event_handling = true;
		if ((revents & POLLHUP) && !(revents & POLLIN))
		{
			if (close_callback) close_callback();
		}
		
		if (revents & POLLNVAL){
			LOG << "Channel::handleEvent()  POLLNVAL";
		}
		
		if (revents & (POLLNVAL | POLL_ERR)){
			if (error_callback){
				error_callback();
			}
		}
		
		if (revents & (POLLIN | POLLRDHUP | POLLPRI)){
			if (read_callback){
				read_callback();
			}
		}
		
		if (revents & POLLOUT){
			if (write_callback){
				write_callback();
			}
		}
		event_handling = false;
	}
	
	
}

