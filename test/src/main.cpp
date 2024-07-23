import stdbridge;
import debug;
import fmt;
import tests;

int main(int argc, char **argv)
{
  const auto tests{ get_tests() };
  if (argc == 1)
  {
    for (const auto &[name, test] : tests)
    {
      fmt::print(info, "\tRunning test: {}\n", name);
      test();
    }
  } else
  {
    for (auto i : std::ranges::iota_view{ 1, argc })
    {
      if (auto it = tests.find(argv[i]); it != tests.end())
      {
        fmt::print(info, "\tRunning test: {}\n", it->first);
        it->second();
        break;
      } else
      {
        fmt::print(info, "Test {} not found", argv[i]);
      }
    }
  }
}
