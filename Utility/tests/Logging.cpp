//
//  Logging.cpp
//  HChat
//
//  Created by 邓志鹏 on 09/02/2020.
//  Copyright © 2020 邓志鹏. All rights reserved.
//

#include <stdio.h>
#include "Logging.hpp"
#include <iostream>
#include <thread>

using namespace HChat;
using namespace std;

void type_test()
{
    // 13 lines
    cout << "----------type test-----------" << '\n';
    LOG << 0;
    LOG << 1234567890123;
    LOG << 1.0f;
    LOG << 3.1415926;
    LOG << (short) 1;
    LOG << (long long) 1;
    LOG << (unsigned int) 1;
    LOG << (unsigned long) 1;
    LOG << (long double) 1.6555556;
    LOG << (unsigned long long) 1;
    LOG << 'c';
    LOG << "abcdefg";
    LOG << string("This is a string");
}

void threadFunc()
{
    for (int i = 0; i < 100000; ++i)
    {
        LOG << i;
    }
}

void stressing_multi_threads(int threadNum = 4)
{
    
}

void other()
{
    // 1 line
    cout << "----------other test-----------" << endl;
    LOG << "fddsa" << 'c' << 0 << 3.666 << string("This is a string");
}


void file_output(const char * msg,size_t len)
{
    FILE* file = fopen (".//HChat.log", "a");
    size_t n = fwrite(msg, 1,len, file);
}

void file_flush()
{
//    fflush(file);
}

int main()
{
    Logger::setOutput(file_output);
    type_test();
    other();
}
