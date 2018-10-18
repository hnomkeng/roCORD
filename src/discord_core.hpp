//
//  discord_core.hpp
//  roCORD
//
//  Created by Norman Ziebal on 21.08.18.
//  Copyright © 2018 Norman Ziebal. All rights reserved.
//

#ifndef discord_core_hpp
#define discord_core_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <functional>
#include <queue>
#include <thread>
#include <chrono>
#include "discord_websocket.hpp"
#include "discord_http.hpp"

enum State {
    OFF,
    CONNECTING,
    ON
};

class discord_core {
    
public:
    	discord_core(std::string display_name_, std::string token_, std::string presence_, int debug_, std::shared_ptr<std::vector<std::pair<std::string, std::string>>> channel_mapping_, std::unique_ptr<discord_websocket> dwss_, std::unique_ptr<discord_http> dhttps_);
    	virtual ~discord_core();
    	void info();
    	int to_discord(std::string& msg, const std::string& channel, std::string* name);
    	void set_display_name(const std::string& display_name);
    	void handle_events();
    	State get_state();
    
private:
    	friend class discord_websocket;

	// Discord Api handling
	void handle_cmd_info(const std::string& channel_id);
    	void handle_ready(const std::string& guild_id);
    	void handle_hello(int heartbeat_interval);
    	void handle_guild_create();
    	void handle_close();
    	void handle_cmd_uptime(const std::string& channel_id);
    	void handle_message_create(const std::string& author, const std::string& nick, std::string& content, const std::string& d_channel);
	
	void convert_latin1(std::string& content);
    	void convert_utf8(std::string& content);
    	bool check_ISO8859_1(const std::string& content);
   	
	// Private varibales
	int debug;
    	State state = OFF;
    	std::string display_name;
	std::string token;
    	std::string presence;
	std::string guild_id = "";
	std::chrono::time_point<std::chrono::system_clock> start_time;
    	std::unique_ptr<discord_websocket> dwss;
	std::unique_ptr<discord_http> dhttps;
    	std::shared_ptr<std::vector<std::pair<std::string, std::string>>> channel_mapping; // TODO: has to be shared???
    
};

#endif /* discord_core_hpp */
