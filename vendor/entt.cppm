module;
#include <cpptrace/cpptrace.hpp>
#define ENTT_ASSERT(condition, msg)                          \
  if (!(condition)) {                                        \
    cpptrace::generate_trace().print();                       \
    throw std::runtime_error(std::string("ENTT ASSERTION FAILED: ") + (msg)); \
  }

#include <entt/entity/registry.hpp>

export module entt;

export namespace entt {
using registry = entt::registry;
using entity = entt::entity;
}
