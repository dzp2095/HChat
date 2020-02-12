//
//  main.cpp
//  HChat
//
//  Created by 邓志鹏 on 29/01/2020.
//  Copyright © 2020 邓志鹏. All rights reserved.
//

#include <iostream>
#include "unistd.h"
#include "Logging.hpp"

using namespace HChat;
int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << getppid();
    HChat::LOG<<1;
    return 0;
}
