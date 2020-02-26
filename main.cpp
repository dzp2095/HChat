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
#include "ThreadPool.h"

using namespace HChat;
using namespace std;
int main(int argc, const char * argv[]) {
    vector<future<int>> res(10);

    ThreadPool pool(9);
    for (int i=0;i<=10;i++){

        res.emplace_back(
                pool.submit([i]{
                    cout<<"Hello, World by "<<i<<endl;
                    return i*i;
                }));

    }
}
