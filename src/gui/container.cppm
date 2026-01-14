export module gui:container;
import std;
import raylib;
import :node;

template <class... Ts>
struct overloaded : Ts... {
  using Ts::operator()...;
};
export namespace gui::v2 {
enum class layout_flags {
  no_space,
  fill_space,
  space_between,
  space_around,
};

// when v_alignment == bottom, we have to render in reverse, as
enum class v_alignment {
  top,
  bottom,
  center,
};
// if rendering an horizontal, we calculate the total width of the container
// and start rendering it from pos+parent.w/2-container.w/2 for center
// for right is draw in reverse from pos+parent.w and before rendering, make
// pos.x-=widget.w
enum class h_alignment {
  left,
  right,
  center,
};
constexpr layout_flags parse_layout_flags(std::string_view str) {
  if (str == "no_space") return layout_flags::no_space;
  if (str == "fill_space") return layout_flags::fill_space;
  if (str == "space_between") return layout_flags::space_between;
  if (str == "space_around") return layout_flags::space_around;
  throw std::invalid_argument("Invalid layout_flags string");
}

constexpr h_alignment parse_h_alignment(std::string_view str) {
  if (str == "left") return h_alignment::left;
  if (str == "right") return h_alignment::right;
  if (str == "center") return h_alignment::center;
  throw std::invalid_argument("Invalid h_alignment string");
}

constexpr std::string_view to_string(layout_flags flag) {
  switch (flag) {
    case layout_flags::no_space:
      return "no_space";
    case layout_flags::fill_space:
      return "fill_space";
    case layout_flags::space_between:
      return "space_between";
    case layout_flags::space_around:
      return "space_around";
    default:
      return "unknown";
  }
}

constexpr std::string_view to_string(h_alignment flag) {
  switch (flag) {
    case h_alignment::left:
      return "left";
    case h_alignment::right:
      return "right";
    case h_alignment::center:
      return "center";
    default:
      return "unknown";
  }
}

struct container {
  enum class direction { horizontal, vertical };
  using widget_t = std::shared_ptr<node>;
  using container_t = std::shared_ptr<container>;
  using child_t = std::variant<widget_t, container_t>;

  Vector2 position{0.f, 0.f};
  float gap = 0.f;
  direction dir{direction::vertical};
  std::vector<child_t> children{};
  layout_flags lyflags = layout_flags::no_space;
  h_alignment alflags = h_alignment::left;

  auto add_child(auto &&child) {
    auto ret = std::make_shared<std::decay_t<decltype(child)>>(
        std::forward<decltype(child)>(child));
    this->children.emplace_back(ret);
    if (ret->get_size().y > get_size().y) {
      std::puts("WARN: element is bigger than the container\n");
    }

    return ret;
  }

  template <typename T, typename... Args>
    requires std::derived_from<T, node>
  auto add_child(Args &&...args) {
    return add_child(T(std::forward<Args>(args)...));
  }

  void add_children(auto &&...appended) {
    (add_child(std::forward<decltype(appended)>(appended)), ...);
  }
  [[nodiscard]] Vector2 get_size() const {
    return std::accumulate(std::begin(children), std::end(children),
                           Vector2{0, 0}, [](auto acc, auto &w) {
                             return acc + std::visit(overloaded{[&](auto w) {
                                                       return w->get_size();
                                                     }

                                                     },
                                                     w);
                           });
  }

  void draw() const;
  // maybe make private as it is only used inside its parent
  void draw(Vector2 parent_end) const;
  // enum class draw_debug {all, boxes, widget};
  // void draw(Vector2 parent_end, draw_debug what_to_draw) const;
  void draw_box_model() const;
  void draw_box_model(Vector2 parent_end) const;
};

}  // namespace gui::v2
