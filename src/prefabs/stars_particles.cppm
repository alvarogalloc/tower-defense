export module prefabs.stars_particles;
import raylib;
import std;
export namespace prefabs
{

struct star
{
    Vector2 position;
    float speed;
    Color color;
};

class stars_particles
{
  public:
    constexpr static auto default_count = 200;
    stars_particles(int count = default_count);

    void update(float delta_time);
    void draw() const;

  private:
    star make_star(std::minstd_rand &rng);

    std::vector<star> m_stars;
};
} // namespace prefabs
