#include <cstdlib>
#include <filesystem>
#include <string>
#include <thread>
#include <iostream>

#include <boost/asio.hpp>
#include <boost/config.hpp>
#include <boost/exception/all.hpp>
#include <boost/program_options.hpp>
#include <boost/stacktrace.hpp>
#include <boost/version.hpp>

#include "listener.h"

namespace po = boost::program_options;
namespace sys = boost::system;
namespace asio = boost::asio;
namespace pt = boost::posix_time;
namespace fs = std::filesystem;
namespace strace = boost::stacktrace;

using traced = boost::error_info<struct tag_stacktrace, strace::stacktrace>;
using tcp = boost::asio::ip::tcp;

int main(int argc, char *argv[]) {
  std::size_t thread_cnt = std::thread::hardware_concurrency() * 2;
  asio::io_context ioc{static_cast<int>(thread_cnt)};
  std::vector<std::thread> v;

  ServiceListener sl(thread_cnt, ioc.get_executor());

  sys::error_code ec;
  sl.Start(ec);

  if (ec) {
    std::cerr << "failed to start service listener, " << ec.message() << "\n\n";
    return EXIT_FAILURE;
  }

  asio::signal_set signals(ioc, SIGINT, SIGTERM);
  signals.async_wait([&](const sys::error_code& err, int signal) {
    if (err) {
      std::cerr << err.message() << "\n";
    }
    if (signal) {
      std::cerr << "signal = " << strsignal(signal) << "\n";

      ioc.stop();
    }
  });

  for (auto i = thread_cnt - 1; i > 0; --i) {
    v.emplace_back([&ioc] {
      // CILOG(debug, "ioc is running on tid[{}]", syscall(SYS_gettid));
      ioc.run();
    });
  }

  // CILOG(debug, "ioc is running on tid[{}]", syscall(SYS_gettid));
  ioc.run();

  for (auto& th : v) th.join();
  v.clear();

  return EXIT_SUCCESS;
}
