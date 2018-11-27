//
//  discord_log_entry.hpp
//  roCORD
//
//  Created by Norman Ziebal on 21.08.18.
//  Copyright © 2018 Norman Ziebal. All rights reserved.
//

#ifndef discord_log_entry_hpp
#define discord_log_entry_hpp

#include <string>

namespace rocord {

enum log_type {
    DEBUG   = 0x01,
    INFO    = 0x02,
    STATUS  = 0x04,
    WARNING = 0x08,
    ERROR   = 0x0C
};

class log_entry 
{
public:
  log_entry(std::string message, log_type ltype);
  virtual ~log_entry();

  std::string get_message();
  log_type get_type();

private:
  // time
  std::string message;
  log_type ltype;
};
}
#endif /* discord_log_entry_hpp*/
