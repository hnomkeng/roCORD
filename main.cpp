//
//  main.cpp
//  roCORD
//
//  Created by Norman Ziebal on 15.08.18.
//  Copyright © 2018 Norman Ziebal. All rights reserved.
//

#include <iostream>
#include <future>
#include "discord_bot.hpp"

int main(int argc, const char * argv[]) {
    discord_init();
    int i = 0;
    while(true) {
        discord_handle();
        std::cout << "This simulates rAthena SRC! Round: " << ++i << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    }
    return 0;
}
