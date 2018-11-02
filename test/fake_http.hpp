#ifndef fake_http_hpp
#define fake_http_hpp

#include "../src/discord_http.hpp"

class fake_http : public rocord::http {

public:
  fake_http(std::string token) : rocord::http(token) {}

};

#endif /* fake_http_hpp */
