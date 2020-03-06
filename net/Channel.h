//
// Created by 邓志鹏 on 02/03/2020.
//

#ifndef HCHAT_CHANNEL_H
#define HCHAT_CHANNEL_H

#include <functional>
#include "Noncopyable.hpp"
#include "Poller.h"

namespace HChat{
    class EventLoop;
    class Channel : public Noncopyable<Channel>{

    public:
	    const static int kNew = -1;
	    const static int kAdded = 0;
	    const static int kDeleted = -2;
	    
        using EventCallback = std::function<void ()>;
        Channel(EventLoop* loop,int fd);
        ~Channel();
        void handleEvent();
        void setReadCallback(const EventCallback & cb){
            read_callback=cb;
        }

        void setWriteCallback(const EventCallback &cb){
            write_callback = cb;
        }

        void setErrorCallback(const EventCallback &cb){
            error_callback=cb;
        }

        void setCloseCallback(const EventCallback &cb){
            close_callback=cb;
        }
		
        bool writeEnabled()const{
	        return events & kWriteEvent;
        }
	    bool readEnabled()const{
		    return events & kReadEvent;
	    }
	    
	    void enbaleWrite() {
        	events |= kWriteEvent;
        	update();
        }
	    void enbaleRead() {
		    events |= kReadEvent;
		    update();
	    }
        
        int getFd() const {
            return fd;
        }

        EventLoop* ownerLoop(){
            return loop;
        }

        //Poller
        int getIndex() const {
            return index_4poller;
        }
        void setIndex(int idx){
            index_4poller=idx;
        }

        int getEvents() const { return events; }

        void setRevents(int revt) { revents = revt; } // used by pollers
    private:
        void update();
        
        EventLoop *loop;
        const short fd;
        int events;
        int revents;//real events 实际发生的事件
        int index_4poller;
        bool event_handling;
        bool addedToLoop;
        
        EventCallback read_callback;
        EventCallback write_callback;
        EventCallback error_callback;
        EventCallback close_callback;

    };
}



#endif //HCHAT_CHANNEL_H
