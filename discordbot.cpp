//
// Created by Norman Ziebal on 10.07.18.
//
#include "discordbot.h"
// by normy

#include "../common/mmo.hpp"
#include "../common/socket.hpp"
#include "../common/strlib.hpp"
#include "../common/nullpo.hpp"
#include "../common/timer.hpp"
#include "../common/malloc.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../map/clif.hpp"
#include "../map/pc.hpp"
#include "../map/channel.hpp"
#include "../common/showmsg.hpp"
#include "../common/socket.hpp"

#define DEBUG

struct discord_bot_interface discord_bot_s;
struct discord_bot_interface *discord;

char send_string[DISCORD_MESSAGE_LENGTH];


void do_init_discord() {
    discord_bot_defaults();
    discord->init();
	ShowInfo("Successfully loaded Discord Bot\n");
}

void discord_bot_init() {
	struct Channel tmp_chn;
	char* name = "#discord";
	char* alias = "[Discord]";

	memset(&tmp_chn, 0, sizeof(struct Channel));

	if (!(discord->ip = host2ip(discord->ip_name))) {
		ShowError("Unable to resolve %s (discord bridge)!\n", discord->ip_name);
		return;
	}
	
	safestrncpy(tmp_chn.name, name + 1, sizeof(tmp_chn.name));
	safestrncpy(tmp_chn.alias, alias, sizeof(tmp_chn.alias));
	tmp_chn.pass[0] = '\0';
	tmp_chn.type = CHAN_TYPE_PUBLIC;
	tmp_chn.color = channel_getColor("Blue");
	tmp_chn.opt = CHAN_OPT_BASE;
	tmp_chn.msg_delay = 1000;

	discord->channel = channel_create(&tmp_chn);
	if (discord->channel == NULL) {
		ShowError("Discord: Channel creation failed!\n");
	}

	discord->fails = 0;
	discord->fd = 0;
	discord->isOn = false;

	discord->websocket = new discord_websocket();
	discord->bot_thread = std::thread(&discord_websocket::run, discord->websocket, discord->channel);

}

void discord_bot_final() {
	if (discord->isOn) {
		do_close(discord->fd);
	}
	discord->fd = 0;
	discord->isOn = false;
	discord->fails = 0;
}



inline void discord_bot_send_api(const char *str, char *name, std::string channel_id) {
	if (!discord->isOn) {
		discord->websocket->send_api(std::string(str));
	}

}

void discord_bot_recv_channel(struct map_session_data *sd, const char *msg) {
	discord->send_api(msg, sd->status.name, "");
}

void discord_bot_join_hook(struct Channel *channel, struct map_session_data *sd) {
	if (strcmp(channel->name, "discord") != 0) {
		return;
	}
	char output[CHAT_SIZE_MAX];
	snprintf(output, CHAT_SIZE_MAX, "Welcome to %s.\nMessages from discord users are marked with <Username>!", discord->channel->alias);
	clif_displaymessage(sd->fd, output);
}

void discord_bot_script_hook(const char *msg) {
	//discord->send_api(msg, NULL, 1);
	return;
}

void discord_bot_hook(struct Channel *channel, struct map_session_data *sd, const char *msg) {
	if (strcmp(channel->name, "discord") != 0) {
		return;
	}
#ifdef DEBUG
	ShowDebug("Discord: Received: %s , with len %d \n", &msg[3], strlen(msg));
#endif
#ifdef WIN32
	discord->recv_chn(sd, &msg[3]); // because all messages from channels start with |00 for an unknown reason.
#else
	discord->recv_chn(sd, msg);
#endif
}

void discord_bot_defaults(void) {
    discord = &discord_bot_s;

    discord->ip_name = "127.0.0.1";
    discord->port = 1337;
    discord->isOn = false;
    discord->channel = NULL;

    discord->init = discord_bot_init;
    discord->final = discord_bot_final;
    discord->send_api = discord_bot_send_api;
    discord->recv_chn = discord_bot_recv_channel;
	discord->connect_timer_id = INVALID_TIMER;
}
