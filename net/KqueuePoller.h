//
// Created by 邓志鹏 on 01/03/2020.
//

#ifndef HCHAT_KQUEUEPOLLER_H
#define HCHAT_KQUEUEPOLLER_H

#include "Poller.h"
#include "poll.h"
#include <vector>
#include <sys/event.h>



namespace HChat{
	class KqueuePoller: public Poller {
		const int static kInitEvenSize=16;
	public:
		 explicit KqueuePoller(EventLoop *loop);
		 void poll(int wait_ms,ChannelList* activeChannels) override ;
		 void addChannel(Channel *ch) override;
		 void removeChannel(Channel *ch) override;
		 void updateChannel(Channel *ch) override;
		 bool hasChannel(Channel* channel) const override;
	
	private:
		void fillActiveChannels(int numEvents,ChannelList* activeChannels) const;
		int kqueue_fd;
		using EventList = std::vector<struct kevent>;
		EventList events;
		
	};
	
}

#endif //HCHAT_KQUEUEPOLLER_H
