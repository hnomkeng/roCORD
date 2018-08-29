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
#include "discord_websocket.hpp"
#include "discord_http.hpp"

enum State {
    OFF,
    CONNECTING,
    ON
};

class discord_core {
    
public:
    discord_core();
    discord_core(std::string display_name, std::string token, std::string presence, int debug, int version, std::shared_ptr<std::vector<std::pair<std::string, std::string>>> channel_mapping);
    ~discord_core();
    void info();
    int toDiscord(std::string msg);
    void setDisplayName(std::string display_name);
    void handleEvents();
    State getState();
    
private:
    friend class discord_websocket;
    void handleReady(std::string guild_id);
    void handleHello(int heartbeat_interval);
    void handleGuildCreate();
    void handleMessageCreate(std::string author, std::string content, std::string d_channel);
    std::string token;
    std::string display_name;
    std::string presence;
    std::string guild_id;
    State state;
    int config_version;
    int debug;
    std::shared_ptr<std::vector<std::pair<std::string, std::string>>> channel_mapping;
    std::shared_ptr<discord_websocket> dwss;
    std::thread dwss_thr;
    std::unique_ptr<discord_http> dhttps;
    
};

#endif /* discord_core_hpp */
