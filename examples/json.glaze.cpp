import glaze;
import std;

struct config
{
    std::string name;
    int value;
};
const auto src = R"(
{
  "name": "example",
  "value": 42
}
)";

int main()
{

    auto cfg = glz::read_json<config>(src);


    std::print("Name: {}, Value: {}\n", cfg->name, cfg->value);
}
