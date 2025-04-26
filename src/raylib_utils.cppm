export module raylib_utils;
import raylib;
import std;

export {
    constexpr auto to_string(const auto &v)
        requires requires {
            { v.x } -> std::convertible_to<float>;
            { v.y } -> std::convertible_to<float>;
        }
    {
        return std::format("({:.2f},{:.2f})", v.x, v.y);
    }

    constexpr auto list_to_datastr(std::string_view name,
                                   std::ranges::range auto data,
                                   auto transformer)
    {
        std::string datastr;
        std::size_t i = 0;
        for (auto &current : data)
        {
            datastr += std::format("{}{}: {};", name, i, transformer(current));
            i++;
        }
        datastr.pop_back(); // remove last ';'
        return datastr;
    }
}
