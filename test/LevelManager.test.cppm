export module LevelManager.test;
import LevelManager;
import ut;
import fmt;
import stdbridge;
import json;
import assets;
export void LevelManager_test();

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


const auto levels_json = R"(
    [
    {
      "name": "my first level",
      "tilemap": "1.tmx",
      "waves": [
        {
          "type": 1,
          "count": 0,
          "delay_each": 0.0,
          "time_for_next_wave": 10.0
        },
        {
          "type": 2,
          "count": 20,
          "delay_each": 0.3,
          "time_for_next_wave": 10.0
        },
        {
          "type": 1,
          "count": 10,
          "delay_each": 2.9,
          "time_for_next_wave": 10.0
        },
        {
          "type": 2,
          "count": 3,
          "delay_each": 3.0,
          "time_for_next_wave": 10.0
        }
      ]
    },
    {
      "name": "my second level",
      "tilemap": "2.tmx",
      "waves": [
        {
          "type": 1,
          "count": 0,
          "delay_each": 0.0,
          "time_for_next_wave": 10.0
        },
        {
          "type": 2,
          "count": 20,
          "delay_each": 0.3,
          "time_for_next_wave": 10.0
        },
        {
          "type": 1,
          "count": 10,
          "delay_each": 2.9,
          "time_for_next_wave": 10.0
        },
        {
          "type": 2,
          "count": 3,
          "delay_each": 3.0,
          "time_for_next_wave": 10.0
        }
      ]
    }
    ]
)"_json;


void LevelManager_test()
{
  using namespace ut;
  my_assets manager{ SRC_DIR "/assets" };

  "Level Manager Test"_test = [&]() mutable {
    LevelManager level_manager(levels_json,
      enemies_json,
      manager);
    auto &level = level_manager.get_current_level();
    expect(level.m_name == "my first level"_s);
    level_manager.next_level();
    auto &level2 = level_manager.get_current_level();
    expect(level2.m_name == "my second level"_s);
  };
}
