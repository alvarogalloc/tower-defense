import ut;
import raylib;
import target_manager;
import std;

using namespace ut;
int main()
{
  "default constructor"_test = [] mutable {
    target_manager manager;
    expect(true_b == manager.get_targets().empty());
  };

  "constructor with empty json array"_test = [] mutable {
    target_manager manager("[]");
    expect(true_b == manager.get_targets().empty());
  };

  "constructor with valid JSON"_test = [] mutable {
    std::string json = R"(
            [
                {"pos": [0.0,  20.5], "radius": 10.0, "color": [220, 10, 20, 55], "health": 100.0, "max_health": 100.0},
                {"pos": [20.0, 20.5], "radius": 20.0, "color": [0, 255, 0, 255], "health": 200.0, "max_health": 200.0}
            ]
        )";
    target_manager manager(json);
    expect(2_ul == manager.get_targets().size());
  };

  "load_targets_from_file"_test = [] mutable {
    std::string filename = "targets.json";
    std::ofstream file(filename);
    file << R"(
            [
                {"pos": [10.0, 20.5], "radius": 10.0, "color": [255, 0, 0, 255], "health": 100.0, "max_health": 100.0},
                {"pos": [10.0, 20.5], "radius": 20.0, "color": [0, 255, 0, 255], "health": 200.0, "max_health": 200.0}
            ]
        )";
    file.close();

    target_manager manager(std::ifstream {filename});
    expect(2_ul == manager.get_targets().size());

    std::filesystem::remove(filename);
  };

  "spawn"_test = [] mutable {
    target_manager manager;
    target t({100.f, 10.f}, 10.0f, {255, 0, 0, 255}, 100.0f, 100.0f);
    manager.spawn(t);
    expect(1_ul == manager.get_targets().size());
    int n_calls = 2300;
    for (int _ : std::views::iota(0, n_calls)) {
      manager.spawn(t);
    }
    expect(_ul(n_calls + 1) == manager.get_targets().size());
  };
}
