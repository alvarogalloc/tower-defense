module;
#include <cassert>
module tests;
import ut;
import ginseng;
import tilemap;
import raylib;
import bullets;
import debug;

using namespace ut;

namespace {

  struct resources {
    ginseng::database db;
  };
  auto& get_shared_resources()
  {
    static resources res;
    return res;
  }
  void CustomLog(int msgType, const char* text, auto args)
  {
    return;
  }

  void tilemap_test()
  {
    "tilemap"_test = [] mutable {
      // needs because of calls to rlLoadTexture
      // no really need the window
      // TODO: might be better to put this in runner
      SetTraceLogCallback(CustomLog);
      InitWindow(1, 1, "");
      tilemap tp {SRC_DIR "/assets/test_tilemap.tmx"};
      expect(tp.get_textures().size() == 1_ul);
      expect(true_b == (tp.get_textures().find("ground") != tp.get_textures().end()));
      expect(tp.get_target().texture.width == _i(tp.get_map()->width * tp.get_map()->tile_width));
      CloseWindow();
    };
  }

  void deserialize_bullet_groups()
  {
    "deserialize bullet groups test"_test = [] mutable {
      auto b_info = bullet_group_info::load(SRC_DIR "/assets/test_bullet_group.txt");
      expect((5.0_f == b_info.radius)(0.1f));
      expect((10.0_f == b_info.damage)(0.1f));
      expect((20.0_f == b_info.speed)(0.1f));
      expect(5_ul == b_info.count);
      expect(0xaaff00ff_i == ColorToInt(b_info.color));
      expect((3._f == b_info.ellipse_radius.x)(0.1f));
      expect((4._f == b_info.ellipse_radius.y)(0.1f));
    };
  }
} // namespace

std::unordered_map<std::string, void (*)()> get_tests()
{
  return {
    {"tilemap test", tilemap_test},
    {"deserialize bullet groups test", deserialize_bullet_groups},
  };
}
