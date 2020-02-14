//
//  CountDownLatch.hpp
//  HChat
//
//  Created by 邓志鹏 on 08/02/2020.
//  Copyright © 2020 邓志鹏. All rights reserved.
//

#ifndef CountDownLatch_hpp
#define CountDownLatch_hpp

#include "Noncopyable.hpp"
#include <mutex>

namespace HChat
{

class CountDownLatch: public Noncopyable<CountDownLatch>
{
public:

explicit CountDownLatch(int count);

void wait();
/*If n is greater than the value of the internal counter or is negative, the behavior is undefined.*/
void countDown(int n);

int getCount() const;

private:
    
mutable std::mutex mutex_;
std::condition_variable condition_;
int count_;
};

}

#endif /* CountDownLatch_hpp */
