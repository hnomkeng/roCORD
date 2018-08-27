//
//  discord_core.cpp
//  roCORD
//
//  Created by Norman Ziebal on 21.08.18.
//  Copyright © 2018 Norman Ziebal. All rights reserved.
//

#include "discord_core.hpp"
#include <iostream>
#include <thread>

#ifdef TESTING
    #include "showmsg_testing.hpp"
#else
    #include "../common/showmsg.hpp"
#endif


discord_core::discord_core() {}

discord_core::discord_core(std::string display_name, std::string token, std::string presence, int debug, int version, std::shared_ptr<std::vector<std::pair<std::string, std::string>>> channel_mapping) {
    this->display_name = display_name;
    this->token = token;
    this->presence = presence;
    this->config_version = version;
    this->debug = debug;
    this->channel_mapping = channel_mapping;
    this->guild_id = "";
    this->heartbeat_interval = -1;
    this->state = OFF;
    this->dwss = std::make_shared<discord_websocket>(this->token);
    this->dwss_thr = std::thread(&discord_websocket::run, dwss);
    this->dhttps = std::make_unique<discord_http>(&this->token);
    // heartbeat thread
    this->info();
}

discord_core::~discord_core() {
    // TODO
}

/*
 * Public
 * Sends a message from rAthena SRC to Discord
 */
int discord_core::toDiscord(std::string msg) {
    this->dhttps->send(msg);
    return 0;
}

/*
 * Public
 * Return current connection state
 */
State discord_core::getState() {
    return this->state;
}

/*
 * Public
 * A public method to change the display name via discord_bot adapter.
 */
void discord_core::setDisplayName(std::string display_name) {
    if (this->display_name == display_name)
        return;
    this->dhttps->setDisplayName(display_name, this->guild_id);
    this->display_name = display_name;
}

/*
 * Public
 * Handles events from discord API.
 */
void discord_core::handleEvents() {
    std::function<void(discord_core*)> event = this->dwss->getNextEvent();
    if(event)
        event(this);
}

/*
 * Public
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

/*
 * Private
 * Handles the Ready Event from Discord API.
 */
void discord_core::handleReady(std::string guild_id) {
    ShowInfo("Discord: Ready Event!");
    this->guild_id = guild_id;
    this->dhttps->setDisplayName(this->display_name, this->guild_id); // init set of display_name
    this->dhttps->send(" * We launched into outer space * ");
    this->state = ON;
}

/*
 * Private
 * Handles a message from Discord server.
 */
void discord_core::handleMessageCreate(std::string author, std::string content, std::string d_channel) {
    ShowInfo("Discord: Message Event!");
    if (author == this->display_name)
        return;
    
    std::string channel = "";
    for(auto it = channel_mapping->begin(); it != channel_mapping->end(); it++) {
        if (it->second == d_channel) {
            channel = d_channel;
            break;
        }
    }
    
    if(channel == "") {
        ShowError("Discord channel has no mapping!");
        return;
    }
    std::cout << author << ": " << content << std::endl;
}

/*
 * Private
 * Handles the content of the Discord server.
 */
void discord_core::handleGuildCreate() {
    ShowInfo("Discord: GuildCreate Event");
    std::cout << "GuildCreate has to be handled!" << std::endl;
}

/*
 * Private
 * Handles the init Event from Discord API.
 */
void discord_core::handleHello(int heartbeat_interval) {
    ShowInfo("Discord: Hello Event!");
    this->heartbeat_interval = heartbeat_interval / 2; // TODO configable
    this->heartbeat_thr = std::thread(&discord_websocket::startHeartbeat, dwss, this->heartbeat_interval);
    this->dwss->sendIdentify(&token, &presence);
    this->state = CONNECTING;
}

