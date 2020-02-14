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
#include <thread>
#include "AsnyLogger.hpp"
using namespace HChat;
using namespace std;
int main(int argc, const char * argv[]) {
    // insert code here...
    cout<<thread::hardware_concurrency();
    return 0;
}
