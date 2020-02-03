//
//  Singleton.hpp
//  HChat
//
//  Created by 邓志鹏 on 04/02/2020.
//  Copyright © 2020 邓志鹏. All rights reserved.
//

#ifndef HCHAT_Singleton_h
#define HCHAT_Singleton_h
#include <assert.h>
#include <stdio.h>

namespace HChat
{

template<typename T>
/* thread-safe in C++11*/
class Singleton
{
public:
    Singleton (const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton (Singleton &&) = delete;
    Singleton& operator=(Singleton &&) = delete;
    
    static T& instance()
    {
        static T value;
        return value;
    }
private:
    Singleton() = default;
    ~Singleton() = default;
}
}
#endif /* Singleton_h */
