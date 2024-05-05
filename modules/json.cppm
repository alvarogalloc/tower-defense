module;
// push ignore to suppress warnings about the use of deprecated features
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated"
#include <nlohmann/json.hpp>
#pragma clang diagnostic pop

export module json;

export namespace nlohmann {
using ::nlohmann::json;
}
