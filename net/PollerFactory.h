//
// Created by 邓志鹏 on 03/03/2020.
//

#ifndef HCHAT_POLLERFACTORY_H
#define HCHAT_POLLERFACTORY_H

#ifdef __APPLE__

#include "KqueuePoller.h"
#include "Poller.h"
namespace HChat{
	Poller *createPoller(EventLoop *loop){
		return new KqueuePoller(loop);
	}
}

#elif __linux__


#endif

#endif //HCHAT_POLLERFACTORY_H
