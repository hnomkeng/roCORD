//
//  discord_websocket.hpp
//  roCORD
//
//  Created by Norman Ziebal on 15.08.18.
//  Copyright © 2018 Norman Ziebal. All rights reserved.
//

#ifndef discord_websocket_hpp
#define discord_websocket_hpp

#include <stdio.h>
#include <queue>
#include <nlohmann/json.hpp>
#include <websocketpp/client.hpp>
#include <websocketpp/config/asio_client.hpp>
#include <functional>
#include <string>

using namespace nlohmann;

class discord_core;

class discord_websocket {
    std::mutex m;
public:
    discord_websocket() {};
    discord_websocket(std::string token);
    virtual ~discord_websocket();
    void final();
    std::function<void(discord_core*)> getNextEvent();
    void run();
    void sendIdentify(const std::string& token, const std::string& presence);
    void startHeartbeat(int interval);
    bool heartbeat_active;
    
private:
    void onMessage(websocketpp::client<websocketpp::config::asio_tls_client>* client, websocketpp::connection_hdl hdl, websocketpp::config::asio_tls_client::message_type::ptr msg);
    int sequence_number;
    std::string token;
    void do_heartbeat();
    int interval;
    std::thread heartbeat_thr;
    websocketpp::client<websocketpp::config::asio_tls_client> client;
    std::queue<std::function<void(discord_core*)>> eventQueue;
    websocketpp::client<websocketpp::config::asio_tls_client>::connection_ptr connection;
};

#endif /* discord_websocket_hpp */
