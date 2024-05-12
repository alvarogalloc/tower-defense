import sfml;
import stdutils;
import assets;
import utils;
import json;
import tilemap;
import say;
import random;
import components;
import EntityFactory;


using namespace components;

const sf::Vector2u win_size{ 640, 704 };
constexpr auto font_path = "/System/Library/Fonts/Times.ttc";


// returns whether the enemy should be deleted from the vector
bool enemy_system(Animation &zombie,
  const std::vector<sf::Vector2f> &directions,
  const float delta,
  components::EnemyPath &current_goal_point,
  components::Velocity velocity)
{
    bool should_delete = false;
    // move the zombie to the next point

    // get unit vector pointing to the next point
    my_assert(current_goal_point.current_point < directions.size(),
      "current goal point is out of bounds");
    const auto center = sprite_center(zombie);
    float distance =
      std::sqrt(std::pow(directions[current_goal_point.current_point].x
                           - center.x - zombie.getPosition().x,
                  2)
                + std::pow(directions[current_goal_point.current_point].y
                             - center.y - zombie.getPosition().y,
                  2));
    if (distance < 3)
    {
        if (current_goal_point.current_point == directions.size() - 1)
        {
            current_goal_point.current_point = 0;
            should_delete = true;
        } else
        {
            ++current_goal_point.current_point;
        }
    }
    auto move_vector = normalize(directions[current_goal_point.current_point]
                                 - center - zombie.getPosition())
                       * velocity.magnitude;
    zombie.move(move_vector * delta);
    return should_delete;
}


enum class shooter_type { none = 0, wizard, big_wizard };


class ShooterSelector : public sf::Drawable
{
    constexpr static float option_size{ 64.f };

    struct shooter_option : public sf::Drawable
    {
        shooter_option(sf::Color color, shooter_type type, sf::Sprite sprite)
          : m_color(color), m_type(type), m_sprite(sprite)
        {}
        void draw(sf::RenderTarget &target,
          sf::RenderStates states) const override
        {
            sf::RectangleShape shooter_selector_bg;
            shooter_selector_bg.setSize(
              { ShooterSelector::option_size, ShooterSelector::option_size });
            shooter_selector_bg.setPosition(m_sprite.getPosition());
            shooter_selector_bg.setFillColor(m_color);
            target.draw(shooter_selector_bg, states);
            target.draw(m_sprite, states);
        }
        sf::Color m_color;
        shooter_type m_type;
        sf::Sprite m_sprite;
    };

  public:
    ShooterSelector(sf::Font *font) : m_font(font) {}
    auto get_current_type() const { return m_current_type; }
    void add_option(sf::Color, shooter_type, sf::Texture *);
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override
    {
        for (auto option : m_options)
        {
            target.draw(option, states);
            if (option.m_type == m_current_type)
            {
                sf::RectangleShape shape;
                shape.setFillColor(sf::Color{ 0x00000055 });
                shape.setPosition(option.m_sprite.getPosition());
                shape.setSize(sf::Vector2f{ option_size, option_size });
                target.draw(shape, states);
            }
        }
    }

    void add_option(sf::Color color,
      shooter_type type,
      const sf::Texture &text,
      const sf::IntRect &rect)
    {
        if (ShooterSelector::option_size * m_options.size() > win_size.x)
        {
            say::warn("the shooter selector is full!");
            return;
        }
        for (const auto &[_, opt_type, _s] : m_options)
        {

            if (type == opt_type)
            {
                say::warn("the shooter is already in the selector!");
                return;
            }
        }
        auto &res = m_options.emplace_back(color, type, sf::Sprite(text, rect));

        sf::Vector2f pos{ option_size * (m_options.size() - 1), 640 };
        res.m_sprite.setPosition(pos);
        auto cal_scale = ShooterSelector::option_size / rect.height;
        res.m_sprite.setScale(cal_scale, cal_scale);
    }
    void choose(sf::Vector2f mouse_pos)
    {
        if (mouse_pos.y < 640.f) { m_current_type = shooter_type::none; }

        for (const auto &option : m_options)
        {
            sf::FloatRect rect{ option.m_sprite.getPosition(),
                sf::Vector2f{ option_size, option_size } };
            if (rect.contains(mouse_pos))
            {
                m_current_type = option.m_type;
                return;
            } else
            {
                m_current_type = shooter_type::none;
            }
        }
    }


  private:
    std::vector<shooter_option> m_options;
    sf::Font *m_font;
    shooter_type m_current_type = shooter_type::none;
};

void flip_sprite(sf::Transformable &target, bool flip_x, bool flip_y)
{
    auto g = target.getScale();
    g.x = std::abs(g.x);
    g.y = std::abs(g.y);
    target.setScale(flip_x ? -g.x : g.x, flip_y ? -g.y : g.y);
}

void draw_select_tile(sf::RenderTarget &target,
  sf::Vector2f mouse_pos,
  sf::Vector2u grid_space_size = { 32, 32 })
{
    sf::RectangleShape grid_tile;
    grid_tile.setFillColor(sf::Color{ 0x32323255 });
    grid_tile.setOutlineColor(sf::Color{ 0x111111ff });
    grid_tile.setOutlineThickness(1);
    grid_tile.setSize(sf::Vector2f{ grid_space_size });
    const auto rows = win_size.y / std::size_t(grid_space_size.y);
    const auto columns = win_size.x / std::size_t(grid_space_size.x);
    for (std::size_t i = 0; i < rows; i++)
    {
        for (std::size_t j = 0; j < columns; j++)
        {
            grid_tile.setPosition(grid_space_size.x * i, grid_space_size.y * j);
            if (grid_tile.getGlobalBounds().contains(mouse_pos))
            {
                target.draw(grid_tile);
                return;
            }
        }
    }
}

enum class enemy_type { zombie, demon };


class Level {
    struct spawn_data {
        enemy_type type;
        std::size_t count;
        float time_for_next_wave;
    };
public:
    static Level from_json(const nlohmann::json& json)
    {
    }


    std::vector<sf::IntRect> m_spawning_zones;
    Tilemap m_map;
    std::vector <spawn_data> m_enemy_queue;
    float timer = 0;
};

class LevelManager {
public:
    static auto get()  {
        static LevelManager instance;
        return instance;
    }
private:
     // get_spawning_zones()
    LevelManager() = default;
};

std::string_view to_string(shooter_type type)
{
    using enum shooter_type;
    switch (type)
    {
    case wizard:
        return "wizard";
    case big_wizard:
        return "big wizard";
    case none:
        return "none";
    }
}


int main(int argc, char *argv[])
{

    try
    {
        fmt::println("path: {}", get_resource_path() + "assets/tiled/");
        sf::RenderWindow win{ sf::VideoMode{ win_size.x, win_size.y },
            "Tower Defense in the works",
            sf::Style::None };
        const auto middle_screen = sf::VideoMode::getDesktopMode().width / 4;
        win.setPosition({ static_cast<int>(middle_screen), 0 });
        win.setFramerateLimit(60);
        win.setKeyRepeatEnabled(false);

        assets manager{ get_resource_path() + "assets/tiled/" };
        Tilemap map{ get_resource_path() + "assets/tiled/1.tmx", manager };


        EntityFactory factory{ manager,
            get_resource_path() + "assets/frames.json" };
        factory.set_enemy_turning_points(
          get_turning_points(map.get_map()->ly_head));

        sf::Font font;
        if (!font.loadFromFile(font_path))
            throw std::runtime_error{ "failed to load font" };

        ShooterSelector selector(&font);

        for (std::size_t i = 0; i < 5; i++)
        {
            selector.add_option(sf::Color::Red,
              shooter_type::wizard,
              *manager.get<sf::Texture>("../characters.png"),
              factory.get_wizard_frames().back());
        }
        selector.add_option(sf::Color::Cyan,
          shooter_type::big_wizard,
          *manager.get<sf::Texture>("../characters.png"),
          factory.get_wizard_frames().back());


        Random rd;
        // ecs
        ginseng::database db;
        factory.spawn_wizard(db, rd.get_vec2<sf::Vector2f>(100, 500));
        factory.spawn_wizard(db, rd.get_vec2<sf::Vector2f>(100, 500));
        factory.spawn_wizard(db, rd.get_vec2<sf::Vector2f>(100, 500));
        factory.spawn_wizard(db, rd.get_vec2<sf::Vector2f>(100, 500));


        sf::Event ev;
        sf::Clock clock;
        float delta = 0;
        auto mouse_pos = sf::Vector2f{ 0, 0 };
        while (win.isOpen())
        {
            while (win.pollEvent(ev))
            {
                if (ev.type == sf::Event::Closed) { win.close(); }
                if (ev.type == sf::Event::MouseMoved)
                {
                    mouse_pos =
                      win.mapPixelToCoords({ ev.mouseMove.x, ev.mouseMove.y });
                    db.visit(
                      [&](components::Animation &anim, components::player_tag) {
                          // rotate around its origin
                          auto direction =
                            normalize(mouse_pos - anim.getPosition());
                          if (direction.x < 0)
                          {
                              flip_sprite(anim, true, false);
                          } else
                          {
                              flip_sprite(anim, false, false);
                          }
                      });
                }
                if (ev.type == sf::Event::KeyPressed)
                {
                    if (ev.key.code == sf::Keyboard::S)
                    {
                        factory.spawn_zombie(db);
                    } else if (ev.key.code == sf::Keyboard::D)
                    {
                        factory.spawn_demon(db);
                    }
                }
                if (ev.type == sf::Event::MouseButtonPressed
                    && ev.mouseButton.button == sf::Mouse::Left)
                {
                    if (ev.mouseButton.y > 640)
                    {
                        // avoid shooting when mouse in menu
                        selector.choose(mouse_pos);
                    } else
                    {

                        db.visit([&](components::Animation &anim,
                                   components::player_tag) {
                            auto bullet_id = db.create_entity();
                            components::Projectile projectile;
                            // calculate the direction of the bullet with mouse
                            // position
                            projectile.direction =
                              normalize(mouse_pos - anim.getPosition());
                            projectile.speed = 500;
                            projectile.damage = 5;
                            projectile.setSize({ 10, 10 });
                            // rotate the bullet by a fixed amount
                            projectile.setPosition(anim.getPosition());
                            projectile.setOrigin(sprite_center(projectile));
                            db.add_component(bullet_id, projectile);
                        });
                    }
                }
            }
            delta = clock.restart().asSeconds();
            db.visit([&](components::Animation &anim) { anim.update(delta); });

            db.visit([&](ginseng::database::ent_id id,
                       components::Animation &anim,
                       components::EnemyPath &path,
                       const components::Velocity &vel,
                       components::Health &health) {
                if (enemy_system(anim,
                      factory.get_enemy_turning_points(),
                      delta,
                      path,
                      vel)
                    || health.health <= 0)
                {
                    db.destroy_entity(id);
                }
            });
            db.visit([&](ginseng::database::ent_id id,
                       components::Projectile &projectile) {
                if (projectile.getPosition().x < 0
                    || projectile.getPosition().x > win_size.x
                    || projectile.getPosition().y < 0
                    || projectile.getPosition().y > win_size.y)
                {
                    db.destroy_entity(id);
                } else
                {
                    projectile.move(
                      projectile.direction * projectile.speed * delta);
                    projectile.rotate(delta * 1000);

                    db.visit([&](components::Animation &anim,
                               components::Health &health) {
                        if (db.exists(id)
                            && projectile.getGlobalBounds().intersects(
                              anim.getGlobalBounds()))
                        {
                            health.health -= projectile.damage;
                            db.destroy_entity(id);
                        }
                    });
                }
            });


            win.clear();
            win.draw(map);
            db.visit(
              [&](Animation &anim,
                ginseng::optional<components::draw_bounding_box> box_tag) {
                  win.draw(anim);
              });
            db.visit([&](Animation &anim, components::Health health) {
                sf::RectangleShape outsiderect;
                // put it above the sprite of the width of the sprite and a
                // height of 10, the width should be in function of the
                // health remaining
                outsiderect.setSize({ anim.getGlobalBounds().width, 10 });
                outsiderect.setPosition(
                  anim.getPosition().x, anim.getPosition().y - 10);
                outsiderect.setFillColor(sf::Color{ 0x323232aa });
                win.draw(outsiderect);
                sf::RectangleShape innerrect;
                innerrect.setSize({ (anim.getGlobalBounds().width - 4)
                                      * (health.health / health.max_health),
                  10 - 4 });
                innerrect.setPosition(
                  anim.getPosition().x + 2, anim.getPosition().y - 8);
                innerrect.setFillColor(sf::Color::Green);
                win.draw(innerrect);
            });
            db.visit([&](components::Projectile &projectile) {
                win.draw(projectile);
            });
            // draw_shooter_select(win, shooter_selectors);
            draw_select_tile(win, mouse_pos);
            win.draw(selector);
            win.display();
        }
    } catch (const std::exception &e)
    {
        fmt::println(
          "exception caught when running the game: \n\t{}", e.what());
        return 1;
    }
}
