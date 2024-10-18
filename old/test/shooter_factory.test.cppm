export module shooter_factory.test;
import stdbridge;
import components;
import ginseng;
import sfml;
import json;
import ut;
import assets;
import stdbridge;
import shooter_factory;

export void shooter_factory_test();

using namespace nlohmann::literals;
const auto shooter_json = R"(
[
  {
    "name": "wizzard",
    "type": 1,
    "damage": 20,
    "frames": {
      "idle": [
      [128,164,16,28],
      [144,164,16,28],
      [160,164,16,28],
      [176,164,16,28]
    ]
  }
},
  {
    "name": "knight",
    "type": 2,
    "damage": 10,
    "frames": {
      "idle": [
        [128, 68, 16, 28],
        [144, 68, 16, 28],
        [160, 68, 16, 28],
        [176, 68, 16, 28]
      ]
    }
  }
]
)"_json;

void shooter_factory_test()
{
  using namespace ut;

  ginseng::database db;
  sf::Image image;
  image.create(2, 2, sf::Color::White);
  sf::Texture text;
  text.loadFromImage(image);

  "shooter factory test"_test = [&] mutable {
    using enum components::shooter_type;
    shooter_factory factory(&text, shooter_json);
    factory.spawn_shooter(db, wizard, { 0, 0 });
    expect(1_ul == db.size());
    expect(2_ul == factory.get_shooter_data().size());

    expect(20_u8 == factory.get_shooter_info(wizard).damage);
    expect(10_u8 == factory.get_shooter_info(knight).damage);

    expect("wizzard"_s == factory.get_shooter_info(wizard).name);
    expect("knight"_s == factory.get_shooter_info(knight).name);

    expect(4_ul == factory.get_shooter_info(wizard).frames.size());
    expect(4_ul == factory.get_shooter_info(knight).frames.size());

    expect(true_b
           == (sf::IntRect(128, 164, 16, 28)
               == factory.get_shooter_info(wizard).frames[0]));
  };
}
