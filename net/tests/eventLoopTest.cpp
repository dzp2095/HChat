//
// Created by 邓志鹏 on 06/03/2020.
//

#include "EventLoop.h"
#include <fcntl.h>
#include "Channel.h"
#include <iostream>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/event.h>
#include <errno.h>
#include <string.h>

using namespace std;
using namespace HChat;

EventLoop * g_loop;
int main(){

	EventLoop loop;
	g_loop = &loop;
	//标准输入流
	int stdin_fd = STDOUT_FILENO;
	Channel channel(&loop,stdin_fd);
	char realBuffer[4096];
	void *buffer=realBuffer;
	channel.setReadCallback([stdin_fd,buffer](){ read(stdin_fd, static_cast<void *>(buffer), sizeof(buffer));});
	channel.enbaleRead();
	//标准输出流
	
	
	loop.loop();
}