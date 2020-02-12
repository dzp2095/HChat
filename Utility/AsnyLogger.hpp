//
//  AsnyLogger.hpp
//  HChat
//
//  Created by 邓志鹏 on 12/02/2020.
//  Copyright © 2020 邓志鹏. All rights reserved.
//

#ifndef AsnyLogger_hpp
#define AsnyLogger_hpp

#include <stdio.h>
#include <thread>
#include "Noncopyable.hpp"

namespace HChat {

class AsnyLogger:public Noncopyable<AsnyLogger> {

public:
    void append(const char * logline,size_t len);
    
    void start();
    
    void stop();
    
private:
    
};

}

#endif /* AsnyLogger_hpp */
