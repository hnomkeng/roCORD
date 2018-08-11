#include "discord_websocket.h"
// DiscordWSS_test_stackoverflow1.cpp: Definiert den Einstiegspunkt für die Konsolenanwendung.
// src: https://stackoverflow.com/questions/39678143/error-while-decoding-payload-after-trying-to-send-second-payload
//Standard C++:
#include <string>
//JSON Header (nlohmann's library):
#include <nlohmann/json.hpp>
//Networking Headers:
#include <websocketpp/client.hpp>
#include <websocketpp/config/asio_client.hpp>
#include <future>
#include "discord_bot.h"
#include "../map/channel.hpp"
#include "https.h"
#include "../map/clif.hpp"

#define WEBSOCKETPP_STRICT_MASKING

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#define ECHO

bot* state;
https* https_test;


discord_websocket::discord_websocket()
{
	
}


discord_websocket::~discord_websocket()
{
}

template <typename Verifier> class verbose_verification
{
public:
	verbose_verification(Verifier verifier): verifier_(verifier)
	{}

	bool operator()(bool preverified, boost::asio::ssl::verify_context& ctx)
	{
		char subject_name[256];
		X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
		X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);
		bool verified = verifier_(preverified, ctx);
		std::cout << "Verifying: " << subject_name << "\n" << "Verified: " << verified << std::endl;
		return verified;
	}
private:
	Verifier verifier_;
};

///@brief Auxiliary function to make verbose_verification objects.
template <typename Verifier> verbose_verification<Verifier> make_verbose_verification(Verifier verifier)
{
	return verbose_verification<Verifier>(verifier);
}


static void heartbeat(int interval, websocketpp::client<websocketpp::config::asio_tls_client>* client, websocketpp::connection_hdl hdl) {
	websocketpp::lib::error_code errorCode;
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds{ interval / 2 });
		nlohmann::json heartb =
		{
			{ "op", 1 },
		{ "d", state->get_sequence() }
		};
		std::cout << std::string(heartb.dump()) << std::endl;
		client->send(hdl, std::string(heartb.dump()), websocketpp::frame::opcode::text, errorCode);
		if (errorCode) { std::cerr << "Heartbeat failed because " << errorCode.message() << std::endl; }
		std::cout << "heartbeat!" << std::endl;
		// For testing
		//std::cout << "Test! " << state->c_guild->members.at(0)->user->username << std::endl;
	}
}

// Doing the SSL stuff!
static websocketpp::lib::shared_ptr<boost::asio::ssl::context> on_tls_init(websocketpp::connection_hdl)
{
	websocketpp::lib::shared_ptr<boost::asio::ssl::context> ctx = websocketpp::lib::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::sslv23);

	ctx->set_options(boost::asio::ssl::context::default_workarounds |
		boost::asio::ssl::context::no_sslv2 |
		boost::asio::ssl::context::no_sslv3 |
		boost::asio::ssl::context::single_dh_use);

	ctx->load_verify_file("path/to/cacert.pem");
	ctx->set_verify_mode(boost::asio::ssl::verify_peer);
	ctx->set_verify_callback(make_verbose_verification(boost::asio::ssl::rfc2818_verification("gateway.discord.gg")));

	return ctx;
}

void onMessage(websocketpp::client<websocketpp::config::asio_tls_client>* client, websocketpp::connection_hdl hdl, websocketpp::config::asio_tls_client::message_type::ptr msg)
{
	nlohmann::json identify;
	nlohmann::json t_value;
	nlohmann::json s_value;
	nlohmann::json d_value;
	websocketpp::lib::error_code errorCode;

	// Get the payload
	nlohmann::json payload = nlohmann::json::parse(msg->get_payload());

	// Print s and save it to state
	s_value = payload.at("s");
	std::cout << "sequence: " << std::string(s_value.dump()) << std::endl;
	if (s_value.dump() != "null")
		state->set_sequence(s_value);

	// If the op code is 'hello'
	int op_code = (int)payload.at("op");
	switch (op_code) {
	case 10:
		std::cout << "Received: " << std::string(payload.dump()) << std::endl;
		state->heartbeat = std::thread(heartbeat, payload.at("d").at("heartbeat_interval"), client, hdl);
		// Create the identify JSON
		identify =
		{
			{ "op", 2 },
		{ "d",{
			{ "token", *state->get_token() },
		{ "properties",{
#ifdef __linux__ 
			{ "$os", "linux" },
#endif
#ifdef _WIN32
		{ "$os", "windows" },
#endif
		{ "$browser", "roCORD" },
		{ "$device", "roCORD" },
		{ "$referrer", "" },
		{ "$referring_domain", "" }
		} },
		{ "compress", false },
		{ "large_threshold", 250 },
		{ "shard",{ 0, 1 } },
		{ "presence",{
			{ "game",{
				{ "name", "by Normynator" },
		{ "type", 0 },
		} },
		{ "status", "dnd" },
		{ "since", NULL },
		{ "afk", false },
		} },
		} }
		};
		std::cout << std::string(identify.dump()) << std::endl;
		// Send the identity JSON
		client->send(hdl, std::string(identify.dump()), websocketpp::frame::opcode::text, errorCode);
		// If the request was invalid
		if (errorCode) { std::cerr << "Identify handshake failed because " << errorCode.message() << std::endl; }
		break;
	case 0:
		t_value = payload.at("t");
		if (t_value == "READY") {
			std::cout << "READY needs to be handled!" << std::endl;
		}
		else if (t_value == "GUILD_CREATE") {
			std::shared_ptr<discord_guild> guild_(new discord_guild(payload));
			state->c_guild = guild_;
		}
		else if (t_value == "MESSAGE_CREATE") {
			std::cout << "Message created: " << std::string(payload.dump()) << std::endl;
			d_value = payload.at("d");
			std::string author = d_value.at("author").at("username").dump();
			std::string content = d_value.at("content").dump();
			std::cout << author << ": " << content << std::endl;
			if (author != "\"roCORD\"") { //todo fix me, name should not be hardcoded!
#ifdef ECHO
				https_test->post(content, state->get_token()->c_str());
#endif // !ECHO
				char send_string[150];
				snprintf(send_string, 150, " %s%s", state->ingame_chn->alias, content.c_str());
				clif_channel_msg(state->ingame_chn, send_string, state->ingame_chn->color);
			}


		}
		else
			std::cout << "Unhandled t value: " << t_value << std::endl;
		break;
	case 11:
		std::cout << "Heartbeat ACK: " << std::string(payload.dump()) << std::endl;
		break;
	default:
		std::cout << "Unhandled OP code! " << op_code << std::endl;
	}

}

int discord_websocket::run(Channel* ingame_chan)
{
	state = new bot("<token>", ingame_chan);
	https_test = new https();

	// Create the client
	websocketpp::client<websocketpp::config::asio_tls_client> client;
	client.set_tls_init_handler(on_tls_init);
	client.init_asio();

	client.set_access_channels(websocketpp::log::alevel::all); // change this to change console debug logs

	client.set_message_handler(websocketpp::lib::bind(&onMessage, &client, websocketpp::lib::placeholders::_1, websocketpp::lib::placeholders::_2));

	// Create an error object
	websocketpp::lib::error_code errorCode;
	// Get the connection from the gateway (usually you'd use GET for the URI, but I'm hardcoding it for simplicity)
	websocketpp::client<websocketpp::config::asio_tls_client>::connection_ptr connection = client.get_connection("wss://gateway.discord.gg/?v=6&encoding=json", errorCode);
	// Check for errors
	if (errorCode)
	{
		std::cout << "Could not create an connection because " << errorCode.message() << std::endl;
	}

	// Connect
	client.connect(connection);

	// Run it!
	client.run();
	return 0;
}

void discord_websocket::send_api(std::string msg)
{
	https_test->post(msg, state->get_token()->c_str());
}
