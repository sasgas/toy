#ifndef APP_SESSION_H_
#define APP_SESSION_H_

#include <string>

#include <boost/asio/spawn.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/http/empty_body.hpp>
#include <boost/beast/core/tcp_stream.hpp>

struct ServiceSession : public std::enable_shared_from_this<ServiceSession> {
  ServiceSession(boost::beast::tcp_stream stream);
  void Start();
  void Run(boost::asio::yield_context yield);

  void HandleRequest(
      boost::posix_time::ptime req_utc,
      boost::beast::http::request<boost::beast::http::empty_body>&& req,
      bool& close, boost::system::error_code& ec,
      const boost::asio::yield_context& yield);

  boost::beast::flat_buffer buffer_;
  boost::beast::tcp_stream stream_;
  std::string transaction_id_;
};

#endif  // APP_SESSION_H_
