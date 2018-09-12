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
    int err = discord_init();
    if (err == -1)
        return 0;
    int i = 0;
    while(i < 100) {
        discord_handle();
        std::cout << "This simulates rAthena SRC! Round: " << ++i << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        //discord_toDiscord("Test message!");
    }

    //discord_free();
    return 0;
}
