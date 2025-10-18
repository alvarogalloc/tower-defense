module;
#include <glaze/glaze_exceptions.hpp>
#include <glaze/json.hpp>

export module glaze;

export namespace glz
{
using glz::format_error;
using glz::read_file_json;
using glz::read_json;
using glz::write_file_json;
using glz::write_json;
namespace ex
{
using glz::ex::read_file_json;
using glz::ex::read_json;
using glz::ex::write_file_json;
using glz::ex::write_json;
} // namespace ex
} // namespace glz
