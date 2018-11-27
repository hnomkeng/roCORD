#include "discord_log.hpp"
#ifdef TESTING
#include "showmsg_testing.hpp"
#else
#include "../../common/showmsg.hpp"
#endif

#include <iostream>

namespace rocord {
log::log()
{
}

log::~log()
{
}

void log::print(std::string message, log_type ltype)
{
  // lock
  // need types, src and so on

  // HARDCODED logging level
  int level = 0x03;

  if (level & ltype) {
    std::shared_ptr<log_entry> entry(new log_entry(message, ltype));
    std::lock_guard<std::mutex> lock(m);
    print_queue.push(entry);
  }
}

void log::handle_print()
{
  if (!m.try_lock())
    return;

  if (print_queue.empty()) {
    m.unlock();
    return;
  }

  auto entry = print_queue.front();
  switch(entry->get_type()) {
    case log_type::WARNING:
      ShowWarning(entry->get_message().c_str());
      break;
    case log_type::INFO:
      ShowInfo(entry->get_message().c_str());
      break;
    case log_type::STATUS:
      ShowStatus(entry->get_message().c_str());
      break;
    case log_type::ERROR:
      ShowError(entry->get_message().c_str());
      break;
    default:
      ShowError("[roCORD]: error in handle print");
  }
  print_queue.pop();
  m.unlock();
}

void log::welcome()
{
  ShowStatus("Loading roCORD by norm\n");
}
} // namespace rocord
