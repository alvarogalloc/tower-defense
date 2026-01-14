import wey;
import raylib;
import ut;
import std;

struct test_cfg_json {
  int a = 0;
  std::string b;
  std::vector<float> vec;
  auto operator<=>(const test_cfg_json&) const = default;
};
inline std::ostream& operator<<(std::ostream& os, const test_cfg_json& cfg) {
  os << "test_cfg_json{"
     << "a=" << cfg.a << ", b=\"" << cfg.b << '"' << ", vec=[";

  for (std::size_t i = 0; i < cfg.vec.size(); ++i) {
    if (i != 0) os << ", ";
    os << cfg.vec[i];
  }

  os << "]}";
  return os;
}

int main() {
  using namespace boost::ut;
  using namespace std::filesystem;
  using std::chrono::duration_cast;
  using std::chrono::milliseconds;
  using std::this_thread::sleep_for;
  using timer = std::chrono::system_clock;
  const auto dir = path{__FILE__}.parent_path().parent_path() / "mock_game_dir";

  assets as{dir};
  const test_cfg_json t{
      .a = 100,
      .b{"hola"},
      .vec{1.2f, 3.14f, 9.23f},
  };

  "assets testing"_test = [&] {
    const auto json_data = as.get_config<test_cfg_json>("test_cfg.json");
    expect(eq(json_data, t))
        << "serialized and hardcoded should be the same but they are:"
           "\nhardcoded:"
        << t << "\nserialized" << json_data;

    InitAudioDevice();
    auto start = timer::now();
    const auto _ = as.get<asset_type::sfx>("assets/zap2.ogg");
    auto cnt_milli1 =
        duration_cast<std::chrono::milliseconds>(timer::now() - start);

    start = timer::now();
    const auto _ = as.get<asset_type::sfx>("assets/zap2.ogg");
    auto cnt_milli2 =
        duration_cast<std::chrono::milliseconds>(timer::now() - start);

    expect(lt(cnt_milli2, cnt_milli1)) << "second time should hit cache and be almost instant";
    CloseAudioDevice();
  };
}
