#include <limits.h>
#include <stdio.h>
#include "../src/discord_core.hpp"
#include "fake_http.hpp"
#include "fake_websocket.hpp"
#include "gtest/gtest.h"
#include "gtest/gtest-spi.h"
#include "gmock/gmock.h"
#include "gmock/gmock-matchers.h"
#include "gmock/gmock-more-matchers.h"
#include "fake_api.cpp"

namespace testing {
namespace gmock_matchers_test {

class CoreTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.
  CoreTest() {
    // You can do set-up work for each test here.
  }

  virtual ~CoreTest() {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  virtual void SetUp() {
    // Code here will be called immediately after the constructor (right
    // before each test).
	auto channel_mapping = std::make_shared<std::vector<std::pair<std::string, std::string>>>();
	channel_mapping->push_back(std::make_pair<std::string, std::string>("Name", "ID"));
	
	std::unique_ptr<discord_websocket> dwss(new fake_websocket("12345678", "wss://gateway.discord.gg/?v=6&encoding=json"));
	std::unique_ptr<discord_http> dhttps(new fake_http());	

	dcore = std::unique_ptr<discord_core>(new discord_core("Test Name", "12345678", "Test Presence", 0, channel_mapping, std::move(dwss), std::move(dhttps)));
	
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }
	
  // Objects declared here can be used by all tests in the test case for CoreTest.
  std::unique_ptr<discord_core> dcore;
};

/*TEST(CoreTest, True){
	EXPECT_TRUE(true);
}*/

TEST_F(CoreTest, Constructor) {
	//std::unique_ptr<discord_core> dcore = CoreTest::create_core();
	
	EXPECT_TRUE(dcore != nullptr);	

}

/*TEST(CoreTestAdv, Destructor) {
	auto channel_mapping = std::make_shared<std::vector<std::pair<std::string, std::string>>>();
	channel_mapping->push_back(std::make_pair<std::string, std::string>("Name", "ID"));
	
	std::shared_ptr<discord_websocket> dwss(new fake_websocket("12345678", "wss://gateway.discord.gg/?v=6&encoding=json"));
	std::unique_ptr<discord_http> dhttps(new fake_http());	

	{
		std::unique_ptr<discord_core> dcore = std::unique_ptr<discord_core>(new discord_core("Test Name", "12345678", "Test Presence", 0, 1, channel_mapping, dwss, std::move(dhttps)));
	}
	
	EXPECT_EQ(dwss.use_count(), 1);
}*/

TEST(CoreTestAdv, reconnect) {
	run();
}

/*TEST(CoreTest, channelName) {
	
}

TEST(CoreTest, toDiscordSimple) {
	// Send a simple message
}

TEST(CoreTest, toDiscordSpecialMessage) {
	// Send a message with "äöü"
}

TEST(CoreTest, toDiscordSpecialName) {
	// Send a message with name containing "äüö"
}

TEST(CoreTest, handleMessageEventSimple) {
	
}

TEST(CoreTest, handleMessageEventSpecialMessage) {
	// Message contains "äöü"
}*/

}
}
