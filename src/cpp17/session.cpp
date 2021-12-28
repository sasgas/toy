#include "session.h"

#include <iostream>

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include "fmt/format.h"
#include "fmt/ostream.h"

#include "listener.h"
#include "fmt_custom.h"

namespace beast = boost::beast;
namespace http = beast::http;
namespace asio = boost::asio;
namespace sys = boost::system;
namespace pt = boost::posix_time;

using tcp = boost::asio::ip::tcp;

////////////////////////////////////////////////////////////////////////////////

ServiceSession::ServiceSession(boost::beast::tcp_stream stream)
    : stream_(std::move(stream)) {}

void ServiceSession::Start() {
  asio::spawn(stream_.get_executor(),
              std::bind(&ServiceSession::Run, shared_from_this(),
                        std::placeholders::_1));
}

void ServiceSession::Run(boost::asio::yield_context yield) {
  auto close = false;
  sys::error_code ec;

  while (true) {
    try {
      http::request<http::empty_body> req;
      http::async_read(stream_, buffer_, req, yield[ec]);
      if (ec == http::error::end_of_stream ||
          ec == asio::error::connection_reset || ec == beast::error::timeout) {
        break;
      }
      if (ec) {
        fmt::print(std::cerr, "[{}] failed to read, {}\n", transaction_id_, ec);
        break;
      }

      // fmt::print(std::cerr, "[{}] {}\n", transaction_id_, req);

      // Send the response
      auto req_utc = pt::microsec_clock::universal_time();
      HandleRequest(req_utc, std::move(req), close, ec, yield);
      if (ec) {
        fmt::print(std::cerr, "[{}] failed to handle request, {}\n",
                   transaction_id_, ec);
        break;
      }
      if (close) {
        // This means we should close the connection, usually because
        // the response indicated the "Connection: close" semantic.
        break;
      }
    } catch (std::exception& e) {
      fmt::print(std::cerr, "[{}] exception caused, {}\n", transaction_id_, e);
      break;
    }
  }

  // At this point the connection is closed gracefully
  // fmt::print(std::cerr, "[{}] connection closed, {}", transaction_id_, ec);

  // Send a TCP shutdown
  stream_.socket().shutdown(tcp::socket::shutdown_send, ec);
}

void ServiceSession::HandleRequest(
    boost::posix_time::ptime req_utc,
    boost::beast::http::request<boost::beast::http::empty_body>&& req,
    bool& close, boost::system::error_code& ec,
    const boost::asio::yield_context& yield) {
  http::response<http::string_body> res{http::status::ok, req.version()};
  res.body() = "Hello, World";
  res.prepare_payload();

  http::response_serializer<http::string_body> sr{res};
  http::async_write(stream_, sr, yield[ec]);
  if (ec) {
    fmt::print(std::cerr, "[{}] write error, {}\n", transaction_id_, ec);
  }

  // fmt::print(std::cerr, "[{}] {}\n", transaction_id_, res);
}
