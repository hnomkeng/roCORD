#pragma once
#include <iostream>
#include <nlohmann/json.hpp>
#include <vector>

using namespace std;

struct channel {
	int bitrate; // if voice
	string id;
	string last_message_id;
	string name;
	// permission overwrites -> unknown type, dont know what for
	int position;
	// topic -> unknown type, dont know what for
	int type; // voice or chat?
	int user_limit; // if voice
};

struct role {
	int color;
	bool hoist;
	string id;
	bool managed;
	bool mentionable;
	string name;
	uint32_t permissions;
	int position;
};

struct user {
	string avatar;
	bool bot;
	string discriminator;
	string id;
	string username;
};

struct member {
	bool deaf;
	string joined_at;
	bool mute;
	string nick;
	std::vector<std::weak_ptr<role>> roles;
	std::shared_ptr<user> user;
};

class discord_guild {
public:
	discord_guild();
	discord_guild(nlohmann::json init_data);
	~discord_guild();

	int init(nlohmann::json init_data);

	//private:
	std::vector<std::shared_ptr<channel>> channels;
	string afk_channel_id;
	int afk_timeout;
	string application_id;
	int default_message_notifications;
	// emojis -> unknown type
	int explicit_content_filter;
	// features -> unknwon type
	string icon;
	string id;
	string joined_at;
	bool large;
	bool lazy;
	int member_count;
	std::vector<std::shared_ptr<member>> members; // vector?
	int mfa_level;
	string name;
	string owner_id;
	// presences -> need a type
	/*
	"presences":[
	{
	"game":{
	"name":"bit.ly/mathbot",
	"type":0
	},
	"status":"online",
	"user":{
	"id":"134073775925886976"
	}
	},
	{
	"game":null,
	"status":"online",
	"user":{
	"id":"209332603076083712"
	}
	},
	{
	"game":null,
	"status":"dnd",
	"user":{
	"id":"468536152618696708"
	}
	}
	],
	*/
	string region;
	std::vector<std::shared_ptr<role>> roles;
	// splash -> unknown type
	// system_channel_id -> unknown type
	bool unavailable;
	int verification_level;
	// voice states -> unknown type
};
