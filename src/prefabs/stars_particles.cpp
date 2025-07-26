module prefabs.stars_particles;
import game;

namespace
{
static std::minstd_rand rng;
float random_speed()
{
    static std::uniform_real_distribution<float> dist_speed(0.5f, 2.0f);
    return dist_speed(rng);
}

int random_color()
{
    static std::uniform_int_distribution<int> dist_color(200, 255);
    return dist_color(rng);
}

int random_twinkle()
{
    static std::uniform_int_distribution<int> dist_twinkle(0, 100);
    return dist_twinkle(rng);
}
} // namespace

namespace prefabs
{

stars_particles::stars_particles(int count)

{
    rng = std::minstd_rand{std::random_device{}()};
    const auto size = game::get().get_spec().size;
    m_stars.reserve(std::size_t(count));
    std::uniform_int_distribution<int> dist_x(0, int(size.x));
    std::uniform_int_distribution<int> dist_y(0, int(size.y));

    for (int i = 0; i < count; ++i)
    {
        star s{};
        s.position = {.x = static_cast<float>(dist_x(rng)),
                      .y = static_cast<float>(dist_y(rng))};
        s.speed = random_speed();
        const std::uint8_t alpha = 255;
        s.color = Color{static_cast<std::uint8_t>(random_color()),
                        static_cast<std::uint8_t>(random_color()),
                        static_cast<std::uint8_t>(random_color()), alpha};
        m_stars.push_back(s);
    }
}

void stars_particles::update(float)
{
    const auto size = game::get().get_spec().size;
    std::uniform_int_distribution<int> dist_x(0, int(size.x));

    for (auto &s : m_stars)
    {
        s.position.y += s.speed;
        if (s.position.y > size.y)
        {
            s.position.y = 0;
            s.position.x = static_cast<float>(dist_x(rng));
        }
        if (random_twinkle() < 2)
        {
            s.color.a = static_cast<unsigned char>(random_color());
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
