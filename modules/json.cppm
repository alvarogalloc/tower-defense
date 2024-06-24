module;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated"
#include <nlohmann/json.hpp>
#pragma clang diagnostic pop
export module json;
export NLOHMANN_JSON_NAMESPACE_BEGIN
  // The main class for the JSON library
    using nlohmann::json;
    using nlohmann::basic_json;


    namespace detail {
        // Exception types
        using nlohmann::detail::parse_error;
        using nlohmann::detail::invalid_iterator;
        using nlohmann::detail::type_error;
        using nlohmann::detail::out_of_range;
        using nlohmann::detail::other_error;

        // Serializer and deserializer classes
        using nlohmann::detail::binary_reader;
        using nlohmann::detail::binary_writer;
        using nlohmann::detail::json_sax_dom_parser;
        using nlohmann::detail::json_sax_dom_callback_parser;

        // Lexer and parser classes
        using nlohmann::detail::lexer;
        using nlohmann::detail::parser;
        using nlohmann::detail::wide_string_input_helper;
        using nlohmann::detail::iterator_input_adapter;
    } // namespace detail
    inline namespace literals {
        // JSON Pointer functions
        using nlohmann::literals::operator""_json_pointer;
        using nlohmann::literals::operator""_json;
    } // namespace literals
NLOHMANN_JSON_NAMESPACE_END
