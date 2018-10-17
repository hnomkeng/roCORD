//
//  discord_websocket.hpp
//  roCORD
//
//  Created by Norman Ziebal on 15.08.18.
//  Copyright © 2018 Norman Ziebal. All rights reserved.
//

#ifndef discord_websocket_hpp
#define discord_websocket_hpp

#include <chrono>
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
    std::mutex c;
public:
    discord_websocket() {};
    discord_websocket(std::string token, std::string uri); //TODO copy by value
    virtual ~discord_websocket();
    void final();
    std::function<void(discord_core*)> getNextEvent();
    void start();
    void sendIdentify(const std::string& token, const std::string& presence);
    void startHeartbeat(int interval);
	std::chrono::time_point<std::chrono::system_clock> getStartTime();
    
private:
    void on_message(websocketpp::client<websocketpp::config::asio_tls_client>* client, websocketpp::connection_hdl hdl, websocketpp::config::asio_tls_client::message_type::ptr msg);
	void on_close(websocketpp::connection_hdl hdl);	
	void on_fail(websocketpp::connection_hdl hdl);
	void on_open(websocketpp::connection_hdl hdl);	
	void on_socket_init(websocketpp::connection_hdl);
	websocketpp::lib::shared_ptr<boost::asio::ssl::context> on_tls_init(websocketpp::connection_hdl);
	void run();
	std::chrono::high_resolution_clock::time_point c_start;
    std::chrono::high_resolution_clock::time_point c_socket_init;
    std::chrono::high_resolution_clock::time_point c_tls_init;
    std::chrono::high_resolution_clock::time_point c_close;
    std::chrono::time_point<std::chrono::system_clock> start_time;
	int sequence_number;
    std::string token;
    std::string uri;
	bool heartbeat_active;
	bool shutdown = false;
	bool started = false;
    void do_heartbeat();
    int interval;
    std::thread heartbeat_thr;
    std::thread socket_thr;
    websocketpp::client<websocketpp::config::asio_tls_client> client;
    std::queue<std::function<void(discord_core*)>> eventQueue;
    websocketpp::client<websocketpp::config::asio_tls_client>::connection_ptr connection;
};

#endif /* discord_websocket_hpp */
