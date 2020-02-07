//
//  Copyable.hpp
//  HChat
//
//  Created by 邓志鹏 on 07/02/2020.
//  Copyright © 2020 邓志鹏. All rights reserved.
//

/*
Use CRTP technique to allow for Empty Base Optimization with multiple inheritance
https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Non-copyable_Mixin
*/
#ifndef HChat_Copyable_h
#define HChat_Copyable_h

namespace HChat
{

template <class T>
class Copyable
{
protected:
    Copyable() = default;
    ~Copyable() = default;
};

}

#endif /* Copyable_h */
