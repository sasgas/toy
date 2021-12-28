#ifndef APP_LISTENER_H_
#define APP_LISTENER_H_

#include <atomic>
#include <memory>

#include <boost/asio/awaitable.hpp>

struct ServiceListener {
  struct AcceptorInfo;

  ServiceListener(boost::asio::any_io_executor exc);
  void Start(boost::system::error_code& ec);
  void Listen(boost::system::error_code& ec);
  void PostStop();
  void Stop();
  bool IsStopped();

  boost::asio::awaitable<void> RunTCP(const AcceptorInfo& ai);

  boost::asio::any_io_executor exc_;
  std::vector<std::shared_ptr<AcceptorInfo>> tcp_acceptors_;

  std::atomic_bool stop_{};
};

#endif  // APP_LISTENER_H_
