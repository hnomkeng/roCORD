//
//  discord_bot.cpp
//  roCORD
//
//  Created by Norman Ziebal on 21.08.18.
//  Copyright © 2018 Norman Ziebal. All rights reserved.
//

#include "discord_bot.hpp"
#include "discord_core.hpp"
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>

#ifdef TESTING
    #include "showmsg_testing.hpp"
#else
    #include "../common/showmsg.hpp"
#endif

using namespace nlohmann;

std::unique_ptr<discord_core> dcore;

/*
 * Entry point to hand control to bot.
 */
void discord_handle() {
    dcore->handleEvents();
}

/*
 * Entry point to send a message to discord.
 */
void discord_toDiscord() {
    dcore->toDiscord();
}

/*
 * Inits the core functionality of the bot. With the given config.
 * A config file looks like following:
 *{
 *      "version": 1,                   # Required
 *      "debug": 0,
 *      "display_name": "roCORD",
 *      "token": "<token>",             # Required
 *      "presence": "by Normynator",
 *      "channels" : {
 *         "general": "1234567890"      # Required
 *      }
 *}
 */
void discord_init() {
    std::ifstream ifs("/Users/normanziebal/roCORD/roCORD/roCORD/config.json"); // TODO: fix hardcoded path!
    json data = json::parse(ifs);
    int version, debug;
    std::string display_name, token, presence;
    
    if(data.find("version") != data.end())
        version = data.at("version");
    else {
        ShowError("Version is not defined! Aborting!");
        return;
    }
    
    if(data.find("token") != data.end())
        token = data.at("token");
    else {
        ShowError("Token is not defined! Aborting!");
        return;
    }
    
    if(data.find("display_name") != data.end())
        display_name = data.at("display_name");
    else {
        ShowWarning("No display_name defined using alternative!");
        display_name = "roCORD";
    }
    
    if(data.find("presence") != data.end())
        presence = data.at("presence");
    else {
        ShowWarning("No presence defined using alternative!");
        presence = "by Normynator";
    }
    
    if(data.find("debug") != data.end())
        debug = data.at("debug");
    else {
        debug = 0;
    }
    
    std::shared_ptr<std::vector<std::pair<std::string, std::string>>> channel_mapping = std::make_shared<std::vector<std::pair<std::string, std::string>>>();
    for (auto it = data.at("channels").begin(); it != data.at("channels").end(); ++it)
    {
        
        channel_mapping->push_back(std::make_pair<std::string, std::string> (((json) (it.key())).dump(), ((json) (it.value())).dump()));
    }
    
    if(channel_mapping->empty()) {
        ShowError("No channel mapping found! Aborting!");
        return;
    }
    
    /* TODO:
     *  Validate channel mapping !
     *  Check if the given channels do exist.
     *  Maybe validate somewhere else, since we dont know the Discord Channels yet!
     */
    dcore = std::make_unique<discord_core>(display_name, token, presence, debug, version, channel_mapping);
}
