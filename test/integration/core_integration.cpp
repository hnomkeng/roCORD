#include "../src/discord_core.hpp"
#include "gmock/gmock-matchers.h"
#include "gmock/gmock-more-matchers.h"
#include "gmock/gmock.h"
#include "gtest/gtest-spi.h"
#include "gtest/gtest.h"

namespace testing {
namespace gmock_matchers_test {

class core_integration : public ::testing::Test {
protected:
  // You can remove any or all of the following functions if its body
  // is empty.
  core_integration()
  {
    // You can do set-up work for each test here.
  }

  virtual ~core_integration()
  {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  virtual void SetUp()
  {
    // Code here will be called immediately after the constructor (right
    // before each test).
	/*	
	std::unique_ptr<fake_websocket> dwss(new fake_websocket());
	dcore = std::unique_ptr<rocord::core>(
			      new rocord::core("display_name", "token", "presence", "debug", "channel_mapping",
					  std::move(dwss), null);
  	*/
  }

  virtual void TearDown()
  {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }

  // Objects declared here can be used by all tests in the test case for
  // CoreTest.
  //std::unique_ptr<rocord::core> dcore;
};


TEST_F(core_integration, simple_recv) {
	
}
}
}
