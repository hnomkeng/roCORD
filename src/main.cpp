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
	std::string name = "test_user";
	std::string channel = "general";
	std::string msg = "Test message!"; 

    while(i < 10) {
        discord_handle();
        std::cout << "This simulates rAthena SRC! Round: " << ++i << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        if(i == 3)
			discord_toDiscord(msg.c_str(), channel.c_str(), name.c_str());
    }

    //discord_free();
    return 0;
}
