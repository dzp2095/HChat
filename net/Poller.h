//
// Created by 邓志鹏 on 02/03/2020.
//

#ifndef HCHAT_POLLER_H
#define HCHAT_POLLER_H

#include "Noncopyable.hpp"
#include "EventLoop.h"
#include <poll.h>
#include <vector>
#include <unordered_map>

namespace HChat{
	const int kMaxEvents = 2000;
	
	#ifndef POLLRDHUP
	#define POLLRDHUP 0x2000
	#endif
	
	const int kReadEvent = POLLIN;
	const int kWriteEvent = POLLOUT;
	const int kNoneEvent = 0;
	
	class Channel;
	class Poller:public Noncopyable<Poller>{
	public:
		using ChannelList=std::vector<Channel*> ;
		explicit Poller(EventLoop *loop);
		virtual ~Poller()= default;
		
		virtual void poll(int wait_ms,ChannelList* activeChannels) = 0;
		virtual void addChannel(Channel *ch) = 0;
		virtual void removeChannel(Channel *ch) = 0;
		virtual void updateChannel(Channel *ch) = 0;
		virtual bool hasChannel(Channel* channel) const=0;
		
		void assertInLoopThread() const{
			ownner_loop->assertInLoopThread();
		}
	protected:
		using ChannelMap = std::unordered_map<short, Channel*> ;
		//文件描述符到 Channel的映射
		ChannelMap channels;
	
	private:
		EventLoop *ownner_loop;
	};
	
}

#endif //HCHAT_POLLER_H
