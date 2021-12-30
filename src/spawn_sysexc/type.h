#ifndef APP_TYPE_H_
#define APP_TYPE_H_

#include <boost/asio/io_context.hpp>
#include <boost/asio/basic_socket_acceptor.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/system_executor.hpp>
#include <boost/beast/core/basic_stream.hpp>

using executor_type = boost::asio::strand<boost::asio::system_executor>;
using tcp_acceptor_type =
    boost::asio::basic_socket_acceptor<boost::asio::ip::tcp, executor_type>;
using tcp_stream_type =
    boost::beast::basic_stream<boost::asio::ip::tcp, executor_type>;

#endif  // APP_TYPE_H_
