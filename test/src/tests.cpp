module tests;
import ut;
import ginseng;
import tilemap;
import raylib;

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

  void tilemap_test()
  {
    "tilemap"_test = [] mutable {
      // needs because of calls to rlLoadTexture
      // no really need the window
      // TODO: might be better to put this in runner
      InitWindow(1, 1, "");
      tilemap tp {SRC_DIR "/assets/test_tilemap.tmx"};
      expect(tp.get_textures().size() == 1_ul);
      expect(true_b == (tp.get_textures().find("ground") != tp.get_textures().end()));
      expect(tp.get_target().texture.width == _i(tp.get_map()->width * tp.get_map()->tile_width));
      CloseWindow();
    };
  }
} // namespace

std::unordered_map<std::string, void (*)()> get_tests()
{
  return {{"tilemap_test", tilemap_test}};
}
