//
// Created by 邓志鹏 on 26/02/2020.
//

#include "ThreadPool.h"
#include <iostream>
#include <vector>
#include <chrono>

using namespace std;
using namespace HChat;
int main(){

    vector<future<int>> res;

    ThreadPool pool(9);
    for (int i=0;i<=1000;i++){
//        std::this_thread::sleep_for(std::chrono::seconds(1));
        res.emplace_back(
        pool.submit([i]{
//                   cout<<"Hello, World by "<<i<<endl;
                   return i*i;
       }));
    }
    std::this_thread::sleep_for(std::chrono::seconds(2));

    for(auto && result: res)
        cout << result.get() << ' ';
    std::cout << std::endl;
}