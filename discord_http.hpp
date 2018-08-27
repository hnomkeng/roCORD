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
    discord_http(std::string* token);
    ~discord_http();
    void send(std::string payload);
    void setDisplayName(std::string display_name, std::string guild_id);
private:
    CURL *curl;
    const char* token;
    void request(struct curl_slist *header, std::string *request_type, std::string *url, std::string *content);
};

#endif /* discord_http_hpp */
