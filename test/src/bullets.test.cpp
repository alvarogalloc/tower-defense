import ut;
import raylib;
import bullets;
import std;
import raylib_utils;

using namespace ut;

// clang-format off
constexpr bullets::info b_ring_info_valid {
  .damage = 10.0f,
  .radius = 5.0f,
  .speed = 20.0f,
  .color ={},
  .count = 5,
  .ellipse_radius {3.0f, 4.0f},
};
// clang-format on

int main()
{
  "deserialize bullet groups test"_test = [] {
    auto b_info = bullets::info::load(SRC_DIR "/assets/test_bullet_group.txt");
    expect((5.0_f == b_info.radius)(0.1f));
    expect((10.0_f == b_info.damage)(0.1f));
    expect((20.0_f == b_info.speed)(0.1f));
    expect(5_ul == b_info.count);
    // gives weird error
    /*expect(0xaaff00ff_i == ColorToInt(b_info.color));*/
    expect((3._f == b_info.ellipse_radius.x)(0.1f));
    expect((4._f == b_info.ellipse_radius.y)(0.1f));
  };
  // i can be sure that from here b_info is valid
  "common bullet operations"_test = [&] {
    bullets b_ring {b_ring_info_valid};
    detached_bullet detached_default {};
    expect(true_b != detached_default.valid());
    expect(b_ring.get().size() == _ul(b_ring.count_alive()));

    detached_bullet dt_bullet = b_ring.detach_bullet({0, 0});
    expect(b_ring.get().size() - 1 == _ul(b_ring.count_alive()));
    expect(neq(dt_bullet.info, detached_default.info));
    /*target target_dummy{*/
    /*      .pos{0,0},*/
    /*      .radius = 5.f,*/
    /*      .color{colors::white},*/
    /*      .health = 10.f,*/
    /*      .max_health = 10.f*/
    /*};*/
    // updating a few times, should increase acc_time which moves the bullets
    const auto alive_bullet = std::ranges::find_if(b_ring.get(), [](const auto& el) {
      return el.alive;
    });
    float before_updating = alive_bullet->acc_time;
    for (int _ : std::views::iota(0, 10)) b_ring.update(0.1f);
    expect(lt(before_updating, alive_bullet->acc_time));
    // anything more than 2pi
    b_ring.update(10.f);
    expect(eq(0.f, alive_bullet->acc_time)(0.1));

    b_ring.respawn_dead();
    expect(b_ring.get().size() == _ul(b_ring.count_alive()));
  };
}
