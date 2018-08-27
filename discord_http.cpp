//
//  discord_http.cpp
//  roCORD
//
//  Created by Norman Ziebal on 22.08.18.
//  Copyright © 2018 Norman Ziebal. All rights reserved.
//

#include "discord_http.hpp"
#include <nlohmann/json.hpp>

using namespace nlohmann;

discord_http::discord_http(std::string* token) {
    this->token = token->c_str();
    
    curl_global_init(CURL_GLOBAL_DEFAULT);
    //this->curl = curl_easy_init();
}

discord_http::~discord_http() {
    curl_global_cleanup();
}

void discord_http::send(std::string payload) {
    struct curl_slist *header = NULL;
    std::string url, content;
    std::string type = "POST";

    url = "channels/280871866669989888/messages"; //TODO fix channel!
    
    content = "content=";
    content.append(payload);
    
    this->request(header, &type, &url, &content);
}

void discord_http::setDisplayName(std::string display_name, std::string guild_id) {
    struct curl_slist *header = NULL;
    std::string url, content;
    std::string type = "PATCH";
    
    if(guild_id == "" || display_name == "")
        return;
    
    header = curl_slist_append(header, "Content-Type:application/json");
    url = "guilds/";
    url.append(guild_id);
    url.append("/members/@me/nick");
    content = ((json) {
        { "username", display_name }
    }).dump();
    
    this->request(header, &type, &url, &content);

}

void discord_http::request(struct curl_slist *header, std::string *request_type, std::string *url, std::string *content) {
    CURL *curl = curl_easy_init();
    if (curl) {
        CURLcode res;
        char auth[256]; //TODO: fix me
        strcpy(auth, "Authorization:Bot ");
        strcat(auth, token);
        header = curl_slist_append(header, auth);
        header = curl_slist_append(header, "User-Agent:roCORD (https://github.com/Normynator/Ragnarok, v1)");
        res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, request_type->c_str());
        std::string api_url = "https://discordapp.com/api/v6/";
        api_url.append(*url);
        curl_easy_setopt(curl, CURLOPT_URL, api_url.c_str()); //TODO fixme
        
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, content->c_str());
        curl_easy_setopt(curl, CURLOPT_CAINFO, "/Users/normanziebal/roCORD.v2/cacert.pem");
        
#ifdef SKIP_PEER_VERIFICATION
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif
        
#ifdef SKIP_HOSTNAME_VERIFICATION
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif
        
        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl); // TODO handle curl_easy_perfrom with: https://curl.haxx.se/libcurl/c/CURLOPT_WRITEFUNCTION.html
        /* Check for errors */
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        
        /* always cleanup */
        curl_easy_cleanup(curl);
    }
}
