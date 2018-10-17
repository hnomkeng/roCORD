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
    discord_core(std::string display_name_, std::string token_, std::string presence_, int debug_, int version_, std::shared_ptr<std::vector<std::pair<std::string, std::string>>> channel_mapping_, std::unique_ptr<discord_websocket> dwss_, std::unique_ptr<discord_http> dhttps_);
    virtual ~discord_core();
    void info();
    int toDiscord(std::string& msg, const std::string& channel, std::string* name);
    void setDisplayName(const std::string& display_name);
    void handleEvents();
    State getState();
    
private:
    friend class discord_websocket;
	void handleCmdInfo(const std::string& channel_id);
    void handleReady(const std::string& guild_id);
    void handleHello(int heartbeat_interval);
    void handleGuildCreate();
    void handleClose();
    void handleCmdUptime(const std::string& channel_id);
	void convert_latin1(std::string& content);
    void convert_utf8(std::string& content);
    bool check_ISO8859_1(const std::string& content);
    void handleMessageCreate(const std::string& author, const std::string& nick, std::string& content, const std::string& d_channel);
    std::chrono::time_point<std::chrono::system_clock> start_time;
    std::string display_name;
	std::string token;
    std::string presence;
	int debug;
	int config_version;
    std::string guild_id;
    State state;
    std::shared_ptr<std::vector<std::pair<std::string, std::string>>> channel_mapping; // TODO: has to be shared???
    std::unique_ptr<discord_websocket> dwss;
	std::unique_ptr<discord_http> dhttps;
    
};

#endif /* discord_core_hpp */
