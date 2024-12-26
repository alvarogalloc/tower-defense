import ut;
import raylib;
import target_manager;
import std;

using namespace ut;

auto expect_throw = [](auto&& f) {
  try {
    /*f();*/
    expect(false_b == true) << "Expected a exception here exception";
  } catch (...) {
    expect(true_b == true);
  }
};

int main()
{
  "default constructor"_test = [] {
    target_manager manager;
    expect(true_b == manager.get_targets().empty());
  };

  "constructor with empty json array"_test = [] {
    target_manager manager("[]");
    expect(true_b == manager.get_targets().empty());
  };

  "constructor with valid JSON"_test = [] {
    std::string json = R"(
            [
                {"pos": [0.0,  20.5], "radius": 10.0, "color": [220, 10, 20, 55], "health": 100.0, "max_health": 100.0},
                {"pos": [20.0, 20.5], "radius": 20.0, "color": [0, 255, 0, 255], "health": 200.0, "max_health": 200.0}
            ]
        )";
    target_manager manager(json);
    expect(2_ul == manager.get_targets().size());
  };

  "load_targets_from_file"_test = [] {
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
  "closest to position"_test = [] {
    target_manager manager;
    target t1(10.0f, 100.0f, 100.0f, {10.f, 10.f}, {255, 0, 0, 255});
    target t2(10.0f, 100.0f, 100.0f, {10.f, 10.f}, {255, 0, 10, 255});
    target t3(10.0f, 100.0f, 100.0f, {30.f, 30.f}, {255, 10, 0, 255});
    manager.spawn(t1);
    manager.spawn(t2);
    manager.spawn(t3);
    // make an assertion like this but separate for each condition
    /*
       pos.x == rhs.pos.x && pos.y == rhs.pos.y && radius == rhs.radius
        && color.r == rhs.color.r && color.g == rhs.color.g && color.b == rhs.color.b
        && health == rhs.health && max_health == rhs.max_health
      */
    auto cmp_target = [](target l, target r) {
      // all the left hand side values are equal to the right hand side values
      // but the left floats, should be constructed as _f(value)

      expect((_f(r.pos.x) == l.pos.x)(0.01));
      expect((_f(r.pos.y) == l.pos.y)(0.01));
      expect((_f(r.radius) == l.radius)(0.01));
      expect(_uc(r.color.r) == l.color.r);
      expect(_uc(r.color.g) == l.color.g);
      expect(_uc(r.color.b) == l.color.b);
      expect((_f(r.health) == l.health)(0.01));
      expect((_f(r.max_health) == l.max_health)(0.01));
    };
    auto closest = manager.closest_to({0.f, 0.f});
    cmp_target(t1, closest);
    closest = manager.closest_to({20.f, 20.f});
    // all three are at the same distance, so the first one should be returned
    cmp_target(t1, closest);
    manager.get_targets().at(1).pos = t2.pos = {20.f, 20.f};
    closest = manager.closest_to({20.f, 20.f});
    cmp_target(t2, closest);
  };

  "spawn"_test = [] {
    target_manager manager;
    target t(10.0f, 100.0f, 100.0f, {30.f, 30.f}, {255, 10, 0, 255});
    manager.spawn(t);
    expect(1_ul == manager.get_targets().size());
    int n_calls = 2300;
    for (int _ : std::views::iota(0, n_calls)) {
      manager.spawn(t);
    }
    expect(_ul(n_calls + 1) == manager.get_targets().size());
  };
}
