#include "listener.h"

#include <iostream>

#include <boost/asio.hpp>
#include <boost/asio/experimental/as_tuple.hpp>
#include <boost/beast.hpp>
#include "fmt/format.h"
#include "fmt/ostream.h"

#include "fmt_custom.h"
#include "session.h"

namespace asio = boost::asio;
namespace sys = boost::system;
namespace beast = boost::beast;

using tcp = boost::asio::ip::tcp;

struct ServiceListener::AcceptorInfo {
  AcceptorInfo(asio::any_io_executor exc)
      : strand_(exc), acceptor_(std::make_shared<tcp::acceptor>(strand_)) {}

  asio::strand<asio::any_io_executor> strand_;
  std::shared_ptr<tcp::acceptor> acceptor_;
};

void ServiceListener::Listen(boost::system::error_code& ec) {
  tcp::endpoint listen_ep(asio::ip::address::from_string("0.0.0.0"), 18080);
  for (auto i = 0u; i < thread_count_; ++i) {
    auto ai = std::make_shared<AcceptorInfo>(exc_);

    // Open the acceptor
    ai->acceptor_->open(listen_ep.protocol(), ec);
    if (ec) {
      fmt::print(std::cerr, "failed to open socket, {}\n", ec);
      return;
    }

    ai->acceptor_->set_option(asio::socket_base::reuse_address(true));
    {
      ai->acceptor_->set_option(
          asio::detail::socket_option::boolean<SOL_SOCKET, SO_REUSEPORT>(true),
          ec);
      if (ec) {
        fmt::print(std::cerr, "failed to set reuseport, {}\n", ec);
        ec.clear();
      }
    }

    // Bind to the server address
    ai->acceptor_->bind(listen_ep, ec);
    if (ec) {
      fmt::print(std::cerr, "failed to bind, {}\n", ec);
      return;
    }

    // Start listening for connections
    ai->acceptor_->listen(asio::socket_base::max_listen_connections, ec);
    if (ec) {
      fmt::print(std::cerr, "failed to listen, {}\n", ec);
      return;
    }

    tcp_acceptors_.emplace_back(ai);
  }
}

void ServiceListener::Start(boost::system::error_code& ec) {
  Listen(ec);
  if (ec) return;

  for (auto ai : tcp_acceptors_) {
    co_spawn(ai->strand_, std::bind(&ServiceListener::RunTCP, this, *ai),
             asio::detached);
  }
}

void ServiceListener::PostStop() {
  asio::post(exc_, std::bind(&ServiceListener::Stop, this));
}

void ServiceListener::Stop() {
  stop_ = true;

  for (auto ai : tcp_acceptors_) {
    asio::post(ai->strand_, [ai] {
      sys::error_code ec;
      ai->acceptor_->close(ec);
      if (ec) {
        fmt::print(std::cerr, "failed to close acceptor, {}\n", ec);
      } else {
        fmt::print(std::cerr, "success to close acceptor\n");
      }
    });
  }
}

bool ServiceListener::IsStopped() {
  if (stop_ == false) {
    return false;
  }

  if (bool is_open =
          std::any_of(tcp_acceptors_.begin(), tcp_acceptors_.end(),
                      [](auto ai) { return ai->acceptor_->is_open(); });
      is_open) {
    return false;
  }

  return true;
}

boost::asio::awaitable<void> ServiceListener::RunTCP(const AcceptorInfo& ai) {
  fmt::print(std::cerr, "tcp acceptor started\n");

  sys::error_code ec;
  while (stop_ == false) {
    auto stream = std::make_shared<beast::tcp_stream>(
        asio::make_strand(ai.acceptor_->get_executor()));
    ec.clear();
    co_await ai.acceptor_->async_accept(
        stream->socket(), asio::redirect_error(asio::use_awaitable, ec));
    // std::tie(ec) = co_await ai.acceptor_->async_accept(
    //     stream->socket(), asio::experimental::as_tuple(asio::use_awaitable));
    if (ec) {
      continue;
    }

    try {
      auto s = std::make_shared<ServiceSession>(std::move(*stream));
      s->Start();
    } catch (std::exception& e) {
      fmt::print(std::cerr, "exception caused, {}\n", e);
    }
  }

  fmt::print(std::cerr, "tcp acceptor ended, {}\n", ec);
}
