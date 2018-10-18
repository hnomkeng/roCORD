//
//  discord_bot.cpp
//  roCORD
//
//  Created by Norman Ziebal on 21.08.18.
//  Copyright © 2018 Norman Ziebal. All rights reserved.
//

#include "discord_bot.hpp"
#include "discord_core.hpp"
#include "discord_websocket.hpp"
#include "discord_http.hpp"
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>

#ifdef TESTING
    #include "showmsg_testing.hpp"
#else
    #include "../../common/showmsg.hpp"
	#include "../../common/timer.hpp"
#endif

using namespace nlohmann;

std::unique_ptr<discord_core> dcore;

#ifdef TESTING
void discord_handle() {
	dcore->handle_events();
}
#else
/*
 * Entry point to hand control to bot.
 */
TIMER_FUNC(discord_handle) {
    	dcore->handle_events();
	add_timer(gettick()+100, discord_handle, 0, 0);
	return 0;
}
#endif

/*
 * Entrypoint to send a message to discord.
 */
void discord_send(const char* msg, const char* channel, const char* name) {
	std::string msg_s = msg;
	std::string channel_s = channel;
	std::string name_s = name;
        dcore->to_discord(msg_s, channel_s, &name_s);
}

/*
 * Entrypoint for script command.
 */
int discord_script(const char* msg, const char* channel) {
	std::string msg_s = msg;
	std::string channel_s = channel;
    	return dcore->to_discord(msg_s, channel_s, nullptr);
}

/*
 * Entrypoint to announce mobdrops.
 */
void discord_announce_drop(const char* msg) {
	std::string channel = "drop_announce";
	std::string msg_s = msg;
	dcore->to_discord(msg_s, channel, nullptr);
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
int discord_init() {
	ShowStatus("Loading roCORD by norm\n");
#ifdef TESTING
    std::ifstream ifs("../config/config.json"); // TODO: fix hardcoded path!
#else
	std::ifstream ifs("conf/discord/config.json");
#endif
    json data;
    int debug;
    std::shared_ptr<std::vector<std::pair<std::string, std::string>>> channel_mapping;
    std::string display_name, token, presence;
    if (ifs.fail()) {
        ShowError("[roCORD] Failed to open config.json!\n");
        return -1;
    }

    try {
        data = json::parse(ifs);
        if (data.find("token") != data.end())
            token = data.at("token");
        else {
            ShowError("[roCORD] Token is not defined! Aborting!\n");
            return -1;
        }

        if (data.find("display_name") != data.end())
            display_name = data.at("display_name");
        else {
            ShowInfo("[roCORD] No display_name defined using alternative!\n");
            display_name = "roCORD";
        }

        if (data.find("presence") != data.end())
            presence = data.at("presence");
        else {
            ShowInfo("[roCORD] No presence defined using alternative!\n");
            presence = "by Normynator";
        }

        if (data.find("debug") != data.end())
            debug = data.at("debug");
        else {
            debug = 0;
        }

        channel_mapping = std::make_shared<std::vector<std::pair<std::string, std::string>>>(); // TODO leak?
        for (auto it = data.at("channels").begin(); it != data.at("channels").end(); ++it) {

            channel_mapping->push_back(
                    std::make_pair<std::string, std::string>(((json) (it.key())), ((json) (it.value()))));
        }

        if (channel_mapping->empty()) {
            ShowError("[roCORD] No channel mapping found! Aborting!\n");
            return -1;
        }
    } catch (json::parse_error &e)
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    
    std::unique_ptr<discord_websocket> dwss(new discord_websocket(token, "wss://gateway.discord.gg/?v=6&encoding=json")); //TODO use factory pattern
    std::unique_ptr<discord_http> dhttps(new discord_http(token));
    
    /* TODO:
     *  Validate channel mapping !
     *  Check if the given channels do exist.
     *  Maybe validate somewhere else, since we dont know the Discord Channels yet!
     */
    dcore = std::unique_ptr<discord_core>(new discord_core(display_name, token, presence, debug, channel_mapping, std::move(dwss), std::move(dhttps)));
#ifndef TESTING
	add_timer_func_list(discord_handle, "discord_handle");
	add_timer_interval(gettick()+100, discord_handle, 0, 0, 1000); //start in 1s each 1sec
#endif
	return 0;
}

/* 
 * currently not needed because of unique ptr destruct!
	void discord_free() {
		dcore.reset();
	}
 */
