//
//  discord_core.cpp
//  roCORD
//
//  Created by Norman Ziebal on 21.08.18.
//  Copyright © 2018 Norman Ziebal. All rights reserved.
//

#include "discord_core.hpp"
#include <iostream>
#include <string>
#include <thread>
#include <boost/locale.hpp>
#include <sstream>
#include <future>

#include "discord_error.hpp"

#ifdef TESTING
    #include "showmsg_testing.hpp"
#else
    #include "../../common/showmsg.hpp"
    #include "../channel.hpp"
	#include "../clif.hpp"
#endif

discord_core::discord_core(std::string display_name_, std::string token_, std::string presence_, int debug_, int version_, std::shared_ptr<std::vector<std::pair<std::string, std::string>>> channel_mapping_) : display_name{display_name_}, token{token_}, presence{presence_}, debug{debug_}, config_version{version_}, channel_mapping{channel_mapping_} {
    this->guild_id = "";
    this->state = OFF;
    this->dwss = std::make_shared<discord_websocket>(this->token);
    this->dwss_thr = std::thread(&discord_websocket::run, dwss);
    //this->dhttps = std::make_unique<discord_http>(&this->token);
    this->dhttps = discord_http(&this->token); //TODO should be const
    // heartbeat thread
    this->info();
};


discord_core::~discord_core() {
    this->dwss->final();
    this->dwss_thr.join();
    std::cout << "Pointer to dwss left: " << dwss.use_count() << std::endl;
    std::cout << "Core is shutting down now!" << std::endl;
}

/*
 * Public
 * Sends a message from rAthena SRC to Discord
 * @param name, can be nullptr if message not from player
 */
int discord_core::toDiscord(std::string& msg, const std::string& channel, std::string* name) {
	/*
	  if (!name)
		*name = ""; // if webhooks are used, this should be the bot name;
	*/
	std::string channel_ = "#";
	channel_.append(channel);
	std::string channel_id;
	for(auto it = channel_mapping->begin(); it != channel_mapping->end(); it++) {
		if (it->first == channel_) {
			channel_id = it->second;
			break;
		}
	}

	if(channel_id.empty()) {
		ShowWarning("Discord channel has no mapping!");
		return -1;
	}
#ifndef __linux__ 
	if (name)
		msg.erase(0,3); // TODO: what happens when executing script cmd on linux, same as osx?
#endif
	convert_latin1(msg); 
	std::stringstream ss;
	if (name)
		// TODO: convert_latin1(name) requires template 
		ss << "<" << *name << "> " << msg;// << " | " << channel_id;
	else
		ss << msg;
    this->dhttps.send(ss.str(), channel_id);
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
void discord_core::setDisplayName(const std::string& display_name) {
    if (this->display_name == display_name)
        return;
    this->dhttps.setDisplayName(display_name, this->guild_id);
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
	std::stringstream ss;
	std::string msg;

    ss << "Core loaded with config:\n";
    ss << " - Bot name: " << this->display_name << "\n"; 
	ss << " - Token: " << this->token << "\n";
	ss << " - Presence: " << this->presence << "\n";
	ss << " - Channel mapping: \n";
    for(auto it = this->channel_mapping->begin(); it != this->channel_mapping->end(); it++) {
        ss << "\t\t" << it->first << " <-> " << it->second << std::endl; //TODO
    }
	ShowInfo(ss.str().c_str());
}

/*
 * Private
 * Handles the Ready Event from Discord API.
 */
void discord_core::handleReady(const std::string& guild_id) {
    ShowInfo("Discord: Ready Event!");
    this->guild_id = guild_id; // TODO
    this->dhttps.setDisplayName(this->display_name, this->guild_id); // init set of display_name
    std::string payload = " * We launched into outer space * "; // DEBUG VALUE
    this->dhttps.send(payload, channel_mapping->begin()->second);
    this->state = ON;
}

/*
 * Private
 * Handles a message from Discord server.
 */
void discord_core::handleMessageCreate(const std::string& author, const std::string& nick, std::string& content, const std::string& d_channel) {
    ShowInfo("Discord: Message Event!");
    if (author == this->display_name)
        return;
    
	if (content.length() > 150)
		return;

    std::string channel;
    for(auto it = channel_mapping->begin(); it != channel_mapping->end(); it++) {
        if (it->second == d_channel) {
            channel = it->first;
            break;
        }
    }
    
    if(channel.empty()) {
        ShowWarning("Discord channel has no mapping!");
        return;
    }

    //discord_core::convert_utf8(content);
	Channel* r_channel = channel_name2channel((char*)channel.c_str(), NULL, 0);
	if (!r_channel){
		ShowError("[roCORD] Channel was not found!");
		return;
	}
    std::string msg = r_channel->alias;
    msg.append("<");
    if (!nick.empty() && check_ISO8859_1(nick))
        msg.append(nick);
    else if (check_ISO8859_1(author))
        msg.append(author);
    else
        return;
    msg.append(">: ");
    msg.append(content);
	
	convert_utf8(msg);

#ifdef TESTING
    ShowInfo(msg.c_str());
#else
	clif_channel_msg(r_channel, msg.c_str(), r_channel->color);
#endif
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
    this->dwss->heartbeat_active = true;
    this->dwss->startHeartbeat(heartbeat_interval / 2); // TODO configable
    this->dwss->sendIdentify(this->token, this->presence);
    this->state = CONNECTING;
}

bool discord_core::check_ISO8859_1(const std::string &content) {
    try {
        boost::locale::conv::from_utf(content, "ISO-8859-1", boost::locale::conv::method_type::stop);
    } catch (const boost::locale::conv::conversion_error& e) {
        return false;
    }
    return true;
}

void discord_core::convert_utf8(std::string& content) { 
  	content = boost::locale::conv::from_utf(content, "ISO-8859-1");
}

void discord_core::convert_latin1(std::string& content) {
	std::string latin1 = "ISO-8859-1";
	content = boost::locale::conv::to_utf<char>(content, "ISO-8859-1");
}
