#ifndef APP_LISTENER_H_
#define APP_LISTENER_H_

#include <atomic>
#include <memory>

#include <boost/asio/spawn.hpp>

#include "type.h"

struct ServiceListener {
  struct AcceptorInfo;

  ServiceListener(std::size_t acceptor_count, executor_type exc)
      : acceptor_count_(acceptor_count), exc_(exc) {}
  void Start(boost::system::error_code& ec);
  void Listen(boost::system::error_code& ec);
  void PostStop();
  void Stop();
  bool IsStopped();

  void RunTCP(const AcceptorInfo& ai, boost::asio::yield_context yield);

  std::size_t acceptor_count_{};
  executor_type exc_;
  std::vector<std::shared_ptr<AcceptorInfo>> tcp_acceptors_;

  std::atomic_bool stop_{};
};

#endif  // APP_LISTENER_H_
