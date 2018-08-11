
#include <nlohmann/json.hpp>
#include "discord_guild.h"
#include <iostream>
#include <vector>
#include <boost/bind.hpp>

using namespace nlohmann;

discord_guild::discord_guild()
{
}

discord_guild::discord_guild(json init_data)
{
	this->init(init_data);
}

discord_guild::~discord_guild()
{
}

int discord_guild::init(json init_data)
{
	// generates a object of the current guild state
	std::cout << std::string(init_data.dump()) << std::endl;
	try {
		json d = init_data.at("d");
		this->afk_channel_id = d.at("afk_channel_id").dump();
		this->afk_timeout = d.at("afk_timeout");
		this->application_id = d.at("application_id").dump();

		// parse roles
		std::cout << std::string(d.at("roles").dump()) << std::endl;
		auto roles = d.at("roles").get<std::vector<json>>();
		std::cout << "Roles found: " << roles.size() << std::endl;

		for (int i = 0; i < roles.size(); i++) {
			std::cout << roles.at(i) << std::endl;
			auto jrole = roles.at(i);
			std::shared_ptr<role> role_(new role);
			role_->color = jrole.at("color");
			role_->hoist = jrole.at("hoist");
			role_->id = jrole.at("id").dump();
			role_->managed = jrole.at("managed");
			role_->mentionable = jrole.at("mentionable");
			role_->name = jrole.at("name").dump();
			role_->position = jrole.at("position");
			role_->permissions = jrole.at("permissions");
			this->roles.push_back(role_);
		}

		// parse channels
		std::cout << std::string(d.at("channels").dump()) << std::endl;
		auto channels = d.at("channels").get<std::vector<json>>();
		std::cout << "Channels found: %d" << channels.size() << std::endl;

		for (int i = 0; i < channels.size(); i++) {
			std::cout << channels.at(i) << std::endl;
			auto jchan = channels.at(i);
			std::shared_ptr<channel> chan(new channel);
			chan->id = jchan.at("id").dump();
			chan->type = jchan.at("type");

			if (chan->type == 2) {
				chan->user_limit = jchan.at("user_limit");
				chan->bitrate = jchan.at("bitrate");
			}
			else
				chan->last_message_id = jchan.at("last_message_id").dump();

			chan->name = jchan.at("name").dump();
			chan->position = jchan.at("position");
			this->channels.push_back(chan);
		}

		this->default_message_notifications = d.at("default_message_notifications");
		this->explicit_content_filter = d.at("explicit_content_filter");
		this->icon = d.at("icon").dump();
		this->id = d.at("id").dump();
		this->joined_at = d.at("joined_at").dump();
		this->large = d.at("large");
		this->lazy = d.at("lazy");
		this->member_count = d.at("member_count");

		// parse members
		std::cout << std::string(d.at("members").dump()) << std::endl;
		auto members = d.at("members").get<std::vector<json>>();
		std::cout << "Members found: " << members.size() << std::endl;

		if (this->member_count != members.size()) {
			std::cout << "Member count is not equal to member size!" << std::endl;
			return -1;
		}

		for (int i = 0; i < members.size(); i++) {
			std::cout << members.at(i) << std::endl;
			auto jmemb = members.at(i);
			std::shared_ptr<member> memb(new member);
			memb->deaf = jmemb.at("deaf");
			memb->joined_at = jmemb.at("joined_at").dump();
			memb->mute = jmemb.at("mute");
			if (jmemb.find("nick") != jmemb.end())
				memb->nick = jmemb.at("nick").dump();

			// parse user
			auto juser = jmemb.at("user");
			std::shared_ptr<user> user_(new user);
			user_->avatar = juser.at("avatar").dump();
			if (juser.find("bot") != juser.end())
				user_->bot = juser.at("bot");
			else
				user_->bot = false;
			user_->discriminator = juser.at("discriminator").dump();
			user_->id = juser.at("id").dump();
			user_->username = juser.at("username").dump();

			memb->user = user_;

			// assign roles
			auto m_roles = jmemb.at("roles").get<std::vector<json>>();
			for (int i = 0; i < m_roles.size(); i++) {
				json role_ = m_roles.at(i);
				std::cout << "Found role: " << role_.dump() << std::endl;
				std::vector<std::shared_ptr<role>>::iterator it;
				for (it = this->roles.begin(); it != this->roles.end(); ++it)
					if ((*it)->id == role_.dump()) {
						std::cout << "Role for user " << memb->user->username << " is " << (*it)->id << std::endl;
						memb->roles.push_back(*it);
					}
			}
			this->members.push_back(memb);
		}


		this->mfa_level = d.at("mfa_level");
		this->name = d.at("name").dump();
		this->owner_id = d.at("owner_id").dump();

		json presences = d.at("presences");
		// TODO

		this->region = d.at("region").dump();
		this->unavailable = d.at("unavailable");
		this->verification_level = d.at("verification_level");

	}
	catch (json::type_error& e)
	{
		std::cout << e.what() << '\n';
	}

	return 0;
}
