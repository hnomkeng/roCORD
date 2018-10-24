//
//  discord_http.hpp
//  roCORD
//
//  Created by Norman Ziebal on 22.08.18.
//  Copyright © 2018 Norman Ziebal. All rights reserved.
//

#ifndef discord_http_hpp
#define discord_http_hpp

#include <stdio.h>
#include <curl/curl.h>
#include <iostream>
#include <string>

class discord_http {
    
public:
    discord_http() {}
    discord_http(std::string token); //TODO copy by value
    virtual ~discord_http();
    void send(const std::string& payload, const std::string& channel_id);
    void setDisplayName(const std::string& display_name, const std::string& guild_id);
private:
    CURL* curl;
    std::string token; //TODO should be const
    void request(struct curl_slist *header, const std::string& request_type /* TODO ENUM */, const std::string& url, const std::string& content);
};

#endif /* discord_http_hpp */
