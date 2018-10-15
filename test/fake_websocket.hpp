#ifndef fake_websocket_hpp
#define fake_websocket_hpp

#include "../src/discord_websocket.hpp"

class fake_websocket : public discord_websocket {

public:
	fake_websocket(std::string token, std::string uri) : discord_websocket(token, uri) {}
};

#endif /*fake_websocket_hpp*/
