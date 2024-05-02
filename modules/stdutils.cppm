module;
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <memory>
#include <span>
#include <stdexcept>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

export module stdutils;
export namespace std
{
using ::std::exception;
using ::std::fstream;
using ::std::ifstream;
using ::std::make_shared;
using ::std::make_unique;
using ::std::move;
using ::std::ofstream;
using ::std::pow;
using ::std::runtime_error;
using ::std::shared_ptr;
using ::std::size_t;
using ::std::span;
using ::std::sqrt;
using ::std::string;
using ::std::string_view;
using ::std::thread;
using ::std::uint16_t;
using ::std::uint32_t;
using ::std::uint8_t;
using ::std::unique_ptr;
using ::std::unordered_map;
using ::std::vector;
} // namespace std
