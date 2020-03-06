//
// Created by 邓志鹏 on 01/03/2020.
//

#include "KqueuePoller.h"
#include <sys/event.h>
#include "Logging.hpp"
#include <fcntl.h>
#include "util.h"
#include "Channel.h"

#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cinttypes>
namespace HChat{
	/* A simple routine to return a string for a set of flags. */
	char *flagstring(int flags)
	{
		static char ret[512];
		const char * res = "";
		
		ret[0]='\0'; // clear the string.
		if (flags & NOTE_DELETE) {strcat(ret,res);strcat(ret,"NOTE_DELETE");res="|";}
		if (flags & NOTE_WRITE) {strcat(ret,res);strcat(ret,"NOTE_WRITE");res="|";}
		if (flags & NOTE_EXTEND) {strcat(ret,res);strcat(ret,"NOTE_EXTEND");res="|";}
		if (flags & NOTE_ATTRIB) {strcat(ret,res);strcat(ret,"NOTE_ATTRIB");res="|";}
		if (flags & NOTE_LINK) {strcat(ret,res);strcat(ret,"NOTE_LINK");res="|";}
		if (flags & NOTE_RENAME) {strcat(ret,res);strcat(ret,"NOTE_RENAME");res="|";}
		if (flags & NOTE_REVOKE) {strcat(ret,res);strcat(ret,"NOTE_REVOKE");res="|";}
		if (flags & EVFILT_READ) {strcat(ret,res);strcat(ret,"EVFILT_READ");res="|";}
		if (flags & EVFILT_WRITE) {strcat(ret,res);strcat(ret,"EVFILT_WRITE");res="|";}
		
		return ret;
	}
	KqueuePoller::KqueuePoller(EventLoop *loop) : Poller(loop),
												  events(kInitEvenSize){
		kqueue_fd = kqueue();
		if (kqueue_fd<0){
			LOG<< "Could not open kernel queue.";
		}
		assert(kqueue_fd>=0);
	}
	
	void KqueuePoller::poll(int wait_ms,ChannelList* activeChannels) {
		struct timespec timeout{};
		timeout.tv_sec = wait_ms / 1000;
		timeout.tv_nsec = (wait_ms % 1000) * 1000 * 1000;
		int num_events = kevent(kqueue_fd, nullptr,0,events.data(), static_cast<int>(events.size()),&timeout);
		if (num_events > 0){
			LOG<<num_events<< " events happend";
			fillActiveChannels(num_events,activeChannels);
		}else if (num_events==0){
			LOG<<"nothing happened";
		} else{
			LOG<<"Error occured when call Poller::poll()";
			abort();
		}
	}
	
	void KqueuePoller::addChannel(Channel *ch) {
		assertInLoopThread();
		const int index = ch->getIndex();
		assert(index==Channel::kNew);
		int fd = ch->getFd();
		channels[fd]=ch;
		ch->setIndex(Channel::kAdded);
		struct kevent ev[2];
		int n = 0;
		if (ch->readEnabled()) {
			EV_SET(&ev[n++], fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, ch);
		}
		if (ch->writeEnabled()) {
			EV_SET(&ev[n++], fd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, ch);
		}
		//应用更新
		int r = kevent(kqueue_fd, ev, n, nullptr, 0, nullptr);
		if (r==0){
			LOG << "Failed to update event";
			abort();
		}
	}
	
	void KqueuePoller::removeChannel(Channel *ch) {
		struct kevent ke{};
		int fd = ch->getFd();
		assert(channels[fd]==ch);
		assert(channels.find(fd)!=channels.end());
		int index=ch->getIndex();
		assert(index==Channel::kAdded ||  index==Channel::kDeleted);
		auto n=channels.erase(fd);
		assert(n==1);
		if (index==Channel::kAdded){
			if (ch->readEnabled()) {
				EV_SET(&ke, ch->getFd(), EVFILT_READ, EV_DELETE, 0, 0, nullptr);
				kevent(kqueue_fd, &ke, 1, nullptr, 0, nullptr);
			}
			if (ch->writeEnabled()) {
				EV_SET(&ke, ch->getFd(), EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
				kevent(kqueue_fd, &ke, 1, nullptr, 0, nullptr);
			}
		}
		ch->setIndex(Channel::kNew);
	}
	
	void KqueuePoller::updateChannel(Channel *ch) {
		assertInLoopThread();
		const int index = ch->getIndex();
		short fd = ch->getFd();
		if (index==Channel::kNew){
			assert(channels.find(fd)==channels.end());
			channels[fd]=ch;;
		} else{
			assert(channels.find(fd)!=channels.end());
			assert(channels[fd] == ch);
		}
		
		ch->setIndex(Channel::kAdded);
		int n=0;
		struct kevent ev[2];
		//更新读事件
		if (ch->readEnabled()) {
			EV_SET(&ev[n++], fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, ch);
		} else {
			EV_SET(&ev[n++], fd, EVFILT_READ, EV_DELETE, 0, 0, ch);
		}
		//更新写事件
		if (ch->writeEnabled()) {
			EV_SET(&ev[n++], fd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, ch);
		} else {
			EV_SET(&ev[n++], fd, EVFILT_WRITE, EV_DELETE, 0, 0, ch);
		}
		int r = kevent(kqueue_fd, ev, n, nullptr, 0, nullptr);
		
		if (r==0){
			LOG << "Failed to update event";
			abort();
		}
	}
	
	bool KqueuePoller::hasChannel(Channel *channel) const {
		assertInLoopThread();
		auto it = channels.find(channel->getFd());
		return it != channels.end() && it->second == channel;
	}
	
	void KqueuePoller::fillActiveChannels(int numEvents, Poller::ChannelList *activeChannels) const {
		assert(numEvents <= events.size());
		for (int i = 0; i < numEvents; ++i) {
			auto fd = events[i].ident;
			Channel* ch = static_cast<Channel*>( events[i].udata ) ;
			auto itr = channels.find(fd);
			assert(itr != channels.end());
			assert(itr->second == ch);
			int mask=0;
			int filter = events[i].filter;
			if ( filter & EVFILT_READ){
				mask|=kReadEvent;
			}
			
			if (filter & EVFILT_WRITE){
				mask|=kWriteEvent;
			}
			
			ch->setRevents(mask);
			activeChannels->push_back(ch);
		}
	}
	
	
}

