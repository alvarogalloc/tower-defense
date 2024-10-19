import ut;
import raylib;
import tilemap;

using namespace ut;

void CustomLog(int msgType, const char* text, auto args)
{
  return;
}
int main()
{
  "tilemap"_test = [] mutable {
    // needs because of calls to rlLoadTexture
    // no really need the window
    SetTraceLogCallback(CustomLog);
    InitWindow(1, 1, "");
    tilemap tp {SRC_DIR "/assets/test_tilemap.tmx"};
    expect(tp.get_textures().size() == 1_ul);
    expect(true_b == (tp.get_textures().find("ground") != tp.get_textures().end()));
    expect(tp.get_target().texture.width == _i(tp.get_map()->width * tp.get_map()->tile_width));
    CloseWindow();
  };
}
