#ifndef APP_LISTENER_H_
#define APP_LISTENER_H_

#include <atomic>
#include <memory>

#include <boost/asio/spawn.hpp>

struct ServiceListener {
  struct AcceptorInfo;

  ServiceListener(std::size_t thread_count, boost::asio::any_io_executor exc)
      : thread_count_(thread_count), exc_(exc) {}
  void Start(boost::system::error_code& ec);
  void Listen(boost::system::error_code& ec);
  void PostStop();
  void Stop();
  bool IsStopped();

  void RunTCP(const AcceptorInfo& ai, boost::asio::yield_context yield);

  std::size_t thread_count_{};
  boost::asio::any_io_executor exc_;
  std::vector<std::shared_ptr<AcceptorInfo>> tcp_acceptors_;

  std::atomic_bool stop_{};
};

#endif  // APP_LISTENER_H_
