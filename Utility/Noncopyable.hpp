//
//  Noncopyable.h
//  HChat
//
//  Created by 邓志鹏 on 06/02/2020.
//  Copyright © 2020 邓志鹏. All rights reserved.
//

/*
Use CRTP technique to allow for Empty Base Optimization with multiple inheritance
https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Non-copyable_Mixin
*/

#ifndef HCHAT_NONCOPYABLE_H
#define HCHAT_NONCOPYABLE_H
namespace HChat
{

template <class T>
class Noncopyable
{
public:
    Noncopyable(const Noncopyable&)=delete;
    T & operator=(const T&)=delete;
protected:
    Noncopyable() = default;
    ~Noncopyable() = default;
};

}

#endif /* Noncopyable_h */
