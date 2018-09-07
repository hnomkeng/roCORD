//
//  discord_bot.hpp
//  roCORD
//
//  Created by Norman Ziebal on 21.08.18.
//  Copyright © 2018 Norman Ziebal. All rights reserved.
//

#ifndef discord_bot_hpp
#define discord_bot_hpp

#include <string>
#include <stdio.h>
#include <iostream>
#include "../../common/timer.hpp"

void discord_toDiscord(const char* msg, const char* channel, const char* name);
TIMER_FUNC(discord_handle);
int discord_init();
void discord_free();
int discord_script(const char* msg, const char* channel);

#endif /* discord_bot_hpp */
