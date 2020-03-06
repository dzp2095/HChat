//
// Created by 邓志鹏 on 01/03/2020.
//

#ifndef HCHAT_EVENTLOOP_H
#define HCHAT_EVENTLOOP_H


#include "Noncopyable.hpp"
#include "Logging.hpp"
#include <thread>
#include <atomic>
#include <vector>

namespace HChat{
	class Poller;
	class Channel;
	const static int kPollTimeMs = 10000;
    class EventLoop:public Noncopyable<EventLoop>{
    public:
        EventLoop();
        ~EventLoop();
        void loop();
        bool isInLoopThread(){
            return thread_id == std::this_thread::get_id();
        };
        void assertInLoopThread(){
            assert(isInLoopThread());
        }
        void quitLoop();
	    void updateChannel(Channel* channel);
    private:
        std::atomic<bool> looping;
	    std::atomic<bool> quit;
        const  std::thread::id thread_id;
        std::unique_ptr<Poller> poller;
	    using ChannelList=std::vector<Channel*> ;
	    ChannelList activeChannels;
	    
    };

}

#endif //HCHAT_EVENTLOOP_H
