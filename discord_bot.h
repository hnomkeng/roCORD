#pragma once
#include <iostream>
#include <future>
#include "discord_guild.h"
#include "../map/channel.hpp"
class bot
{
	int sequence;
	std::string token;
	
public:
	bot();
	bot(std::string token, Channel* ingame_chan);
	~bot();
	int set_sequence(int value);
	int get_sequence();
	std::string* get_token();
	std::shared_ptr<discord_guild> c_guild;
	std::thread heartbeat;
	Channel* ingame_chn;
	
};

/*
Required Tests:
- What happens if a role is deleted, is member losing the roll or null/dangling pointer?
- Use valgrind to find memory leaks and dangling pointers.

*/

/*
TODO:
- clean up
- op code switch, call bot functions
- seperate bot and wss logic
*/

