#pragma once
#include "../map/channel.hpp"
#include <string>
class discord_websocket
{
public:
	discord_websocket();
	~discord_websocket();
	void send_api(std::string msg);
	int run(Channel* ingame_chan);
};

