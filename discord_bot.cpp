#include "discord_bot.h"


bot::bot()
{
}

bot::bot(std::string token, Channel* ingame_chan)
{
	this->token = token;
	this->ingame_chn = ingame_chan;
}


bot::~bot()
{
}

int bot::set_sequence(int value) {
	if (value <= sequence) {
		std::cerr << "sequence value can only grow!" << std::endl;
		return -1;
	}
	this->sequence = value;
	return 0;
}

int bot::get_sequence()
{
	return this->sequence;
}

std::string* bot::get_token()
{
	return &(this->token);
}
