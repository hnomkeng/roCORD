//
//  discord_core.cpp
//  roCORD
//
//  Created by Norman Ziebal on 21.08.18.
//  Copyright © 2018 Norman Ziebal. All rights reserved.
//

#include "discord_core.hpp"
#include <iostream>
#include "clif_testing.hpp"
#include "showmsg_testing.hpp"
#include <thread>


discord_core::discord_core() {}

discord_core::discord_core(std::string display_name, std::string token, std::string presence, int debug, int version, std::shared_ptr<std::vector<std::pair<std::string, std::string>>> channel_mapping) {
    this->display_name = display_name;
    this->token = token;
    this->presence = presence;
    this->config_version = version;
    this->debug = debug;
    this->channel_mapping = channel_mapping;
    this->info();
    this->dwss = std::make_shared<discord_websocket>(this->token);
    this->dwss_thr = std::thread(&discord_websocket::run, dwss);
}

discord_core::~discord_core() {
    // TODO
}

/*
 * Sends a message from rAthena SRC to Discord
 */
int discord_core::toDiscord() {
    return 0;
}

/*
 * Handles events from discord API.
 */
void discord_core::handleEvents() {
    // locks
    std::function<void(discord_core*)> event = this->dwss->getNextEvent();
    if(event)
        event(this);
}

/*
 * Handles the Ready Event from Discord API.
 */
void discord_core::handleReady() {
    //TODO
    std::cout << "Ready need to be handled!" << std::endl;
}

/*
 * Handles a message from Discord server.
 */
void discord_core::handleMessageCreate(std::string author, std::string content, std::string d_channel) {
    if (author == this->display_name)
        return;
    
    std::string channel = "";
    for(auto it = channel_mapping->begin(); it != channel_mapping->end(); it++) {
        if (it->first == d_channel) {
            channel = d_channel;
            break;
        }
    }
    
    if(channel == "") {
        ShowError("Discord channel has no mapping!");
        return;
    }
}

/*
 * Handles the content of the Discord server.
 */
void discord_core::handleGuildCreate() {
    
}

/*
 * Handles the init Event from Discord API.
 */
void discord_core::handleHello() {
    this->dwss->sendIdentify(this->token, this->display_name, this->presence);
}


/*
 * Displays an info about the loaded config!
 */
void discord_core::info() {
    std::cout << "Core loaded with config:" << std::endl;
    std::cout << " - Bot name: " << this->display_name << std::endl;
    std::cout << " - Token: " << this->token << std::endl;
    std::cout << " - Presence: " << this->presence << std::endl;
    std::cout << " - Channel mapping: " << std::endl;
    for(auto it = this->channel_mapping->begin(); it != this->channel_mapping->end(); it++) {
        std::cout << "\t\t" << it->first << " <-> " << it->second << std::endl;
    }
}

