module;
#include <vector>
#include <span>
#include <cstdint>
#include <cstddef>
#include <unordered_map>
#include <memory>
#include <string>
#include <thread>
#include <cmath>
#include <stdexcept>

export module stdutils;
export namespace std {
  using ::std::vector;
  using ::std::uint32_t;
  using ::std::size_t;
  using ::std::uint8_t;
  using ::std::uint16_t;
  using ::std::span;
  using ::std::unordered_map;
  using ::std::string;
  using ::std::string_view;
  using ::std::shared_ptr;
  using ::std::make_shared;
  using ::std::unique_ptr;
  using ::std::make_unique;
  using ::std::thread;
  using ::std::sqrt;
  using ::std::pow;
  using ::std::exception;
  using ::std::runtime_error;
  using ::std::move;
}
