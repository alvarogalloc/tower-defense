export module EnemyFactory.test;
import ut;
import assets;
import sfml;
import stdbridge;
import EnemyFactory;
import json;
import ginseng;
import components;

export void EnemyFactory_test();


module :private;
using namespace nlohmann::literals;
const auto enemies_json = R"(
    [
    {
      "name": "zombie",
      "type": 1,
      "health": 200,
      "speed": 30,
      "frames": {
        "idle": [
          [16,332,32,36 ],
          [48,332,32,36 ],
          [80,332,32,36 ],
          [112,332,32,36]
        ]
      }
    },
    {
      "name": "demon",
      "type": 2,
      "health": 100,
      "speed": 70,
      "frames": {
        "idle": [
        [16, 428, 32, 36],
        [48, 428, 32, 36],
        [80, 428, 32, 36],
        [112, 428, 32, 36]
        ] 
      }   
    }
  ]
  )"_json;


void EnemyFactory_test()
{
  using namespace ut;
  my_assets manager{ SRC_DIR "/assets" };
  manager.set_loader<sf::Texture>([](sf::Texture *, std::string_view path) {});
  ginseng::database db;
  "Enemy Factory Test"_test = [&] {
    EnemyFactory factory{ manager, enemies_json };
    expect(1_ul == manager.size<sf::Texture>());
    factory.spawn_enemy(db, components::enemy_type::zombie, { 0, 0 });
    expect(1_ul == db.size());
    factory.spawn_enemy(db, components::enemy_type::demon, { 0, 0 });
    expect(2_ul == db.size());
    auto zombie_data = factory.get_enemy_data(components::enemy_type::zombie);
    expect(true_b == (components::enemy_type::zombie == zombie_data.type));
    expect((200.0_f == zombie_data.health)(0.1));
    expect((30.0_f == zombie_data.speed)(0.1));
    expect(4_ul == zombie_data.frames.size());
  };
}
