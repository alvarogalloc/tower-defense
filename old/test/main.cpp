import assets.test;
import EnemyFactory.test;
import LevelManager.test;
import shooter_factory.test;
import ut;
import fmt;
import stdbridge;
import utils;
int main(int argc, char **argv)
{
  const std::unordered_map<std::string, void (*)()> tests{
    { "EnemyFactory", EnemyFactory_test },
    { "assets", assets_test },
    { "LevelManager", LevelManager_test },
    { "shooter_factory", shooter_factory_test }
  };
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
