//
// Created by Norman Ziebal on 10.07.18.
//

#ifndef RATHENA_DISCORDBOT_H
#define RATHENA_DISCORDBOT_H

#include <stdio.h>
#include <future>
#include <stdint.h>
#include "../map/discord_websocket.h"

#define DISCORD_MESSAGE_LENGTH 128
typedef uint32_t uint32;
typedef int64_t int64;

struct discord_bot_interface {
    int fd;
    bool isOn;
    uint32 ip;
    char* ip_name;
    unsigned short port;
    int64 last_try;
    unsigned char fails;
	int connect_timer_id;
    struct Channel *channel;
	discord_websocket *websocket;
	std::thread bot_thread;


    /**
     * Bot initializer
     */
    void (*init) (void);

    /**
     *  Bot finalizer
     */
    void (*final) (void);

    /**
     * Parser for the Discord bridge connection
     */
    int (*recv_api) (int fd);

	/**
	* Sends the message to Discord bridge
	*/
    void (*send_api) (const char *str, char *name, std::string channel_id);

	/**
	* Parses message from ingame channel
	*/
    void (*recv_chn) (struct map_session_data *sd, const char *msg);

	/**
	* Sends the message from recv_api to ingame channel
	*/
    void (*send_chn) (const char* msg);

	/**
	* Inits a timer to receive messages from Discord bridge
	*/
    int(*connect_timer) (int tid, unsigned int tick, int id, intptr_t data);
};

void discord_bot_hook(struct Channel *channel, struct map_session_data *sd, const char *msg);
void discord_bot_script_hook(const char *msg);
void discord_bot_join_hook(struct Channel *channel, struct map_session_data *sd);
void do_init_discord(void);
void discord_bot_defaults(void);

#endif //RATHENA_DISCORDBOT_H
