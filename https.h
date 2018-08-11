#pragma once
#include <string>
class https
{
public:
	https();
	~https();
	void post(std::string payload, const char* token);
};
