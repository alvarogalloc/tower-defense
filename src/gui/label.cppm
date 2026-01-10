export module gui:label;
import std;
import raylib;
import :node;
export namespace gui::v2 {

struct label : node {
  label(std::string_view str, Color color, Vector2 size);
  label(std::string_view str, Color color, float font_size, Vector2 padding);
  void set_size(Vector2 size) override;
  [[nodiscard]] Vector2 get_size() const override;
  void draw(Vector2 parent_pos) override;

  std::string str;
  Color color;
  float font_size;
  Vector2 padding;
};
}  // namespace gui::v2
