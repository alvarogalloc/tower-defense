export module bullets;
import stdbridge;
import raylib;
export
{
  struct bullet
  {
    Vector2 position;
    bool alive = false;
    float damage = 0.0f;
  };

  template<std::size_t N> class bullets

  {
    // this is the class for the bullets suroounding the player
    // it will be used to draw the bullets and update their position
    // and to take track of the bullets that are still alive and their damage

    bullets(const bullet &b) { m_bullets.fill(b); }

    // position of the player should be provided
    void draw(const Vector2) const;

    void respawn_dead();
    void update(const float delta);

    auto get() const & { return m_bullets; }

    auto count_alive() const -> std::size_t
    {
      return std::ranges::count_if(
        m_bullets, [](const auto &b) { return b.alive; });
    };
    auto count_dead() const -> std::size_t
    {
      return std::ranges::count_if(
        m_bullets, [](const auto &b) { return !b.alive; });
    };


  private:
    std::array<bullet, N> m_bullets;
  };
}
