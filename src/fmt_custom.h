#ifndef COMMON_FMT_CUSTOM_H_
#define COMMON_FMT_CUSTOM_H_

#include <chrono>
#include <filesystem>
#include <system_error>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/local/stream_protocol.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/date_time/posix_time/time_formatters.hpp>
#include <boost/system/error_code.hpp>
#include "urdl/url.hpp"
#include "fmt/format.h"

template <>
struct fmt::formatter<std::filesystem::path> {
  constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

  template <typename FormatContext>
  auto format(const std::filesystem::path& p, FormatContext& ctx) {
    return format_to(ctx.out(), "{}", p.string());
  }
};

template <>
struct fmt::formatter<boost::asio::ip::tcp::endpoint> {
  constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

  template <typename FormatContext>
  auto format(const boost::asio::ip::tcp::endpoint& ep, FormatContext& ctx) {
    auto addr = ep.address();
    if (addr.is_v6() && addr.to_v6().is_v4_mapped()) {
      auto v4 = boost::asio::ip::make_address_v4(boost::asio::ip::v4_mapped,
                                                 addr.to_v6());
      return format_to(ctx.out(), "{}:{}", v4.to_string(), ep.port());
    }
    return format_to(ctx.out(), "{}:{}", addr.to_string(), ep.port());
  }
};

template <>
struct fmt::formatter<boost::asio::ip::udp::endpoint> {
  constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

  template <typename FormatContext>
  auto format(const boost::asio::ip::udp::endpoint& ep, FormatContext& ctx) {
    return format_to(ctx.out(), "{}:{}", ep.address().to_string(), ep.port());
  }
};

template <>
struct fmt::formatter<boost::asio::local::stream_protocol::endpoint> {
  constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

  template <typename FormatContext>
  auto format(const boost::asio::local::stream_protocol::endpoint& ep,
              FormatContext& ctx) {
    return format_to(ctx.out(), "{}", ep.path());
  }
};

template <>
struct fmt::formatter<boost::posix_time::ptime> {
  constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

  template <typename FormatContext>
  auto format(const boost::posix_time::ptime& tm, FormatContext& ctx) {
    return format_to(ctx.out(), "{}",
                     boost::posix_time::to_iso_extended_string(tm));
  }
};

template <>
struct fmt::formatter<boost::posix_time::time_duration> {
  constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

  template <typename FormatContext>
  auto format(const boost::posix_time::time_duration& td, FormatContext& ctx) {
    return format_to(ctx.out(), "{}", boost::posix_time::to_simple_string(td));
  }
};

template <>
struct fmt::formatter<boost::posix_time::microseconds> {
  constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

  template <typename FormatContext>
  auto format(const boost::posix_time::microseconds& td, FormatContext& ctx) {
    return format_to(ctx.out(), "{}", boost::posix_time::to_simple_string(td));
  }
};

template <>
struct fmt::formatter<boost::posix_time::milliseconds> {
  constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

  template <typename FormatContext>
  auto format(const boost::posix_time::milliseconds& td, FormatContext& ctx) {
    return format_to(ctx.out(), "{}", boost::posix_time::to_simple_string(td));
  }
};

template <>
struct fmt::formatter<boost::posix_time::seconds> {
  constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

  template <typename FormatContext>
  auto format(const boost::posix_time::seconds& td, FormatContext& ctx) {
    return format_to(ctx.out(), "{}", boost::posix_time::to_simple_string(td));
  }
};

template <>
struct fmt::formatter<std::chrono::duration<double>> {
  constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

  template <typename FormatContext>
  auto format(const std::chrono::duration<double>& td, FormatContext& ctx) {
    return format_to(ctx.out(), "{:.3f}", td.count());
  }
};

template <>
template <class Body>
struct fmt::formatter<boost::beast::http::request<Body>> {
  constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

  template <typename FormatContext>
  auto format(const boost::beast::http::request<Body>& req,
              FormatContext& ctx) {
    return format_to(ctx.out(), "{}", req.base());
  }
};

template <>
struct fmt::formatter<boost::beast::http::header<true>> {
  constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

  template <typename FormatContext>
  auto format(const boost::beast::http::header<true>& req, FormatContext& ctx) {
    format_to(ctx.out(), "{} {}", req.method_string(), req.target());
    for (auto& f : req) {
      format_to(ctx.out(), ", {}[{}]", f.name_string(), f.value());
    }
    return ctx.out();
  }
};

template <>
template <class Body>
struct fmt::formatter<boost::beast::http::response<Body>> {
  constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

  template <typename FormatContext>
  auto format(const boost::beast::http::response<Body>& res,
              FormatContext& ctx) {
    return format_to(ctx.out(), "{}", res.base());
  }
};

template <>
struct fmt::formatter<boost::beast::http::header<false>> {
  constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

  template <typename FormatContext>
  auto format(const boost::beast::http::header<false>& res,
              FormatContext& ctx) {
    format_to(ctx.out(), "{} {}", res.result_int(), res.reason());
    for (auto& f : res) {
      format_to(ctx.out(), ", {}[{}]", f.name_string(), f.value());
    }
    return ctx.out();
  }
};

template <>
struct fmt::formatter<boost::system::error_code> {
  constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

  template <typename FormatContext>
  auto format(const boost::system::error_code& ec, FormatContext& ctx) {
    return format_to(ctx.out(), "{}", ec.message());
  }
};

template <>
struct fmt::formatter<std::error_code> {
  constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

  template <typename FormatContext>
  auto format(const std::error_code& ec, FormatContext& ctx) {
    return format_to(ctx.out(), "{}", ec.message());
  }
};

template <>
struct fmt::formatter<std::exception> {
  constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

  template <typename FormatContext>
  auto format(const std::exception& e, FormatContext& ctx) {
    return format_to(ctx.out(), "{}", e.what());
  }
};

template <>
struct fmt::formatter<urdl::url> {
  constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

  template <typename FormatContext>
  auto format(const urdl::url& url, FormatContext& ctx) {
    return format_to(ctx.out(), "{}", url.to_string());
  }
};

#endif  // COMMON_FMT_CUSTOM_H_
