//
// Created by 邓志鹏 on 01/03/2020.
//

#include "EventLoop.h"
#include "Logging.hpp"
#include "PollerFactory.h"
#include "Channel.h"

namespace HChat{
    thread_local EventLoop* current_thread_loop = nullptr;
    
	EventLoop::EventLoop(): looping(false),
							quit(false),
	                        thread_id(std::this_thread::get_id()),
	                        poller(createPoller(this)){
		assert(!looping);
		assertInLoopThread();
	    LOG << "EventLoop Created "<< this << "in thread" << thread_id;
	    if (current_thread_loop!= nullptr){
	        LOG << "Current thread has a loop already! "<< current_thread_loop;
	    } else{
	        current_thread_loop=this;
	    }
	}

    EventLoop::~EventLoop() {

    }
	
	void EventLoop::loop() {
		looping= true;
		while (!quit.load()){
			activeChannels.clear();
			poller->poll(kPollTimeMs,&activeChannels);
			for (const auto& channel:activeChannels){
				channel->handleEvent();
			}
		}
		LOG << "EventLoop" << this <<" stop looping";
	}
	
	void EventLoop::updateChannel(Channel *channel) {
		assert(channel->ownerLoop()==this);
		assertInLoopThread();
		poller->updateChannel(channel);
	}
	
	void EventLoop::quitLoop() {
		quit.store(true);
	}
	
}
