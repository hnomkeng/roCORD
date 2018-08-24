//
//  discord_websocket.cpp
//  roCORD
//
//  Created by Norman Ziebal on 15.08.18.
//  Copyright © 2018 Norman Ziebal. All rights reserved.
//

#include <mutex>

#include "discord_websocket.hpp"
#include "discord_core.hpp"

discord_websocket::discord_websocket(std::string token) {
    this->token = token;
}

template <typename Verifier> class verbose_verification
{
public:
    verbose_verification(Verifier verifier, bool debug): verifier_(verifier)
    { this->debug = debug;}
    
    bool operator()(bool preverified, boost::asio::ssl::verify_context& ctx)
    {
        char subject_name[256];
        X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
        X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);
        bool verified = verifier_(preverified, ctx);
        if (debug)
            std::cout << "Verifying: " << subject_name << "\n" << "Verified: " << verified << std::endl;
        return verified;
    }
private:
    bool debug;
    Verifier verifier_;
};

template <typename Verifier> verbose_verification<Verifier> make_verbose_verification(Verifier verifier)
{
    bool debug = false; // TODO make it dynamic
    return verbose_verification<Verifier>(verifier, debug);
}

static websocketpp::lib::shared_ptr<boost::asio::ssl::context> on_tls_init(websocketpp::connection_hdl)
{
    websocketpp::lib::shared_ptr<boost::asio::ssl::context> ctx = websocketpp::lib::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::sslv23);
    
    ctx->set_options(boost::asio::ssl::context::default_workarounds |
                     boost::asio::ssl::context::no_sslv2 |
                     boost::asio::ssl::context::no_sslv3 |
                     boost::asio::ssl::context::single_dh_use);
    
    ctx->load_verify_file("/Users/normanziebal/roCORD.v2/cacert.pem"); // TODO catch exception
    ctx->set_verify_mode(boost::asio::ssl::verify_peer);
    ctx->set_verify_callback(make_verbose_verification(boost::asio::ssl::rfc2818_verification("gateway.discord.gg")));
    
    return ctx;
}

void discord_websocket::onMessage(websocketpp::client<websocketpp::config::asio_tls_client>* client, websocketpp::connection_hdl hdl, websocketpp::config::asio_tls_client::message_type::ptr msg)
{
    json s, t, d;
    int op = -1;
    std::function<void(discord_core*)> event_ptr;
    websocketpp::lib::error_code errorCode;

    json payload = json::parse(msg->get_payload());
    
    s = payload.at("s");
    if (s.dump() != "null") {
        this->sequence_number = (int)s;
    }
    
    op = (int)payload.at("op");
    switch (op) {
        case 0:
            t = payload.at("t");
            if (t == "READY") {
                event_ptr = std::bind(&discord_core::handleReady, std::placeholders::_1);
                // TODO
            }
            else if (t == "GUILD_CREATE") {
                // TODO
            }
            else if (t == "MESSAGE_CREATE") {
                d = payload.at("d");
                std::string author = d.at("author").at("username").dump();
                std::string content = d.at("content").dump();
                
            }
            else
                std::cout << "Unhandled t value: " << t << std::endl;
            break;
        case 10:
            event_ptr = std::bind(&discord_core::handleHello, std::placeholders::_1);
            break;
        case 11:
            std::cout << "Heartbeat ACK: " << std::string(payload.dump()) << std::endl;
            break;
        default:
            std::cout << "Unhandled OP code! " << op << std::endl;
    }
    std::lock_guard<std::mutex> lock(m);
    eventQueue.push(event_ptr);
}

std::function<void(discord_core*)> discord_websocket::getNextEvent() {
    if(!m.try_lock())
        return nullptr;
    
    if (eventQueue.empty()) {
        m.unlock();
        return nullptr;
    }
    
    std::function<void(discord_core*)> ret = eventQueue.front();
    eventQueue.pop();
    m.unlock();
    return ret;
}

void discord_websocket::run() {
    client.set_tls_init_handler(on_tls_init);
    client.init_asio();
    
    client.set_access_channels(websocketpp::log::alevel::all); // TODO debug settings
    
    client.set_message_handler(websocketpp::lib::bind(&discord_websocket::onMessage, this, &client, websocketpp::lib::placeholders::_1, websocketpp::lib::placeholders::_2));
    
    websocketpp::lib::error_code errorCode;
    websocketpp::client<websocketpp::config::asio_tls_client>::connection_ptr connection = client.get_connection("wss://gateway.discord.gg/?v=6&encoding=json", errorCode);
    if (errorCode)
    {
        std::cout << "Could not create an connection because " << errorCode.message() << std::endl;
    }
    this->hdl = connection->get_handle();
    
    client.connect(connection);
    client.run();
}

void discord_websocket::sendIdentify(std::string token, std::string display_name, std::string presence) {
    websocketpp::lib::error_code errorCode;
    json identify =
    {
        { "op", 2 },
        { "d",{
            { "token", "NDY4NTM2MTUyNjE4Njk2NzA4.DmDBFw.4rs4EbJ_p0Klxz7lqnzzy8qwLYQ" },
            { "properties",{
#ifdef __linux__
                { "$os", "linux" },
#elseif _WIN32
                { "$os", "windows" },
#else
                { "$os", "osx" },
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
                    { "name", presence },
                    { "type", 0 },
                } },
                { "status", "dnd" },
                { "since", NULL },
                { "afk", false },
            } },
        } }
    };
    client.send(this->hdl, identify.dump(), websocketpp::frame::opcode::text, errorCode);
    if (errorCode) {
        std::cerr << errorCode.message() << std::endl;
    }
}

discord_websocket::~discord_websocket() {
    
}


