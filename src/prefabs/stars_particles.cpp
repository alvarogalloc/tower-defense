module prefabs;
import wey;


namespace
{
float random_speed(auto rng)
{
    static std::uniform_real_distribution<float> dist_speed(0.5f, 2.0f);
    return dist_speed(rng);
}

int random_color(auto rng)
{
    static std::uniform_int_distribution<int> dist_color(200, 255);
    return dist_color(rng);
}

int random_twinkle(auto rng)
{
    static std::uniform_int_distribution<int> dist_twinkle(0, 100);
    return dist_twinkle(rng);
}
} // namespace

namespace prefabs
{

stars_particles::stars_particles(int count, context_view ctx) : rng(std::random_device{}())

{
    const auto size = ctx.app_info.game_res;
    m_stars.reserve(std::size_t(count));
    std::uniform_int_distribution<int> dist_x(0, int(size.x));
    std::uniform_int_distribution<int> dist_y(0, int(size.y));

    for (int i = 0; i < count; ++i)
    {
        star s{};
        s.position = {.x = static_cast<float>(dist_x(rng)),
                      .y = static_cast<float>(dist_y(rng))};
        s.speed = random_speed(rng);
        const std::uint8_t alpha = 255;
        s.color = Color{static_cast<std::uint8_t>(random_color(rng)),
                        static_cast<std::uint8_t>(random_color(rng)),
                        static_cast<std::uint8_t>(random_color(rng)), alpha};
        m_stars.push_back(s);
    }
}

void stars_particles::update(float, Rectangle cam)
{
    // const auto size = game::get().get_app_info().size;
    // make dist_x and dist_y uniform distributions
    std::uniform_int_distribution<int> dist_x(int(cam.x), int(cam.x+cam.width));
    std::uniform_int_distribution<int> dist_y(int(cam.y),
                                              int(cam.y+cam.height));

    for (auto &s : m_stars)
    {
        s.position.y += s.speed;
        // is the star out of bounds?
        // use raylib's CheckCollisionPointRec
        if (!CheckCollisionPointRec(s.position, cam))
        {
            // reset the star to the top of the screen
            s.position.y = float(dist_y(rng));
            s.position.x = float(dist_x(rng));
            s.speed = random_speed(rng);
            s.color = Color{static_cast<std::uint8_t>(random_color(rng)),
                            static_cast<std::uint8_t>(random_color(rng)),
                            static_cast<std::uint8_t>(random_color(rng)), 255};
        }
        if (random_twinkle(rng) < 2)
        {
            s.color.a = static_cast<unsigned char>(random_color(rng));
        }
    }
}

void stars_particles::draw() const
{
    for (const auto &s : m_stars)
    {
        DrawPixelV(s.position, s.color);
    }
}
} // namespace prefabs
