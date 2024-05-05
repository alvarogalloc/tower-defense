import sfml;
import fmt;
import stdutils;
import tmx;
import assets;
import stringables;
import tilemap;
import json;
import ginseng;

void my_assert(bool condition,
  std::string_view message = "",
  std::source_location loc = std::source_location::current())
{
    if (!condition)
    {

        auto msg = fmt::format("assertion failed: {} (at {}:{})",
          message,
          loc.file_name(),
          loc.line());
        throw std::runtime_error(msg);
    }
}
namespace components {

struct Animation : public sf::Sprite
{
    float frame_duration_seconds;
    std::size_t current_frame = 0;
    float acc_time_seconds = 0;
    std::vector<sf::IntRect> *frames;
    Animation(std::vector<sf::IntRect> *frames,
      sf::Texture &text,
      float frame_duration)
      : frame_duration_seconds(frame_duration), frames(frames)
    {
        this->setTexture(text);
        this->setTextureRect(frames->at(current_frame));
    }
    void update(const float delta)
    {
        acc_time_seconds += delta;

        if (acc_time_seconds >= frame_duration_seconds)
        {
            if (current_frame == frames->size() - 1)
            {
                current_frame = 0;
            } else
            {
                // intentional, is more readable
                ++current_frame;
            }
            this->setTextureRect(frames->at(current_frame));
            acc_time_seconds = 0;
        }
    }
};

struct Health
{
    float max_health;
    float health;
};

struct Velocity
{
    // velocity is the same always in xy
    float magnitude;
};

struct BoundingBox : public sf::RectangleShape
{
    BoundingBox(sf::Sprite &sprite)
    {
        setSize(
          { sprite.getGlobalBounds().width, sprite.getGlobalBounds().height });
        setPosition(sprite.getPosition());
        setFillColor(sf::Color::Transparent);
        setOutlineColor(sf::Color::Red);
        setOutlineThickness(2);
    }
};


struct EnemyPath
{
    std::size_t current_point = 0;
};

struct Projectile : public sf::RectangleShape
{
    sf::Vector2f direction;
    float speed;
    float damage;
};
using player_tag = ginseng::tag<struct player_tag_t>;
}// namespace components


using namespace components;

const sf::Vector2u win_size{ 640, 640 };

constexpr auto font_path = "/System/Library/Fonts/Times.ttc";


auto sprite_center(const sf::Sprite &sprite)
{
    return sf::Vector2f{ sprite.getGlobalBounds().width / 2,
        sprite.getGlobalBounds().height / 2 };
}

auto load_animation_frames_json(const nlohmann::json &parent_node,
  std::string_view name)
{
    my_assert(parent_node.contains(name),
      fmt::format("no suggested key \"{}\" found in the json", name));
    my_assert(parent_node[name].is_array(), "frames must be an array");
    std::vector<sf::IntRect> frames;
    for (auto &&rect : parent_node[name])
    {
        frames.push_back({ rect[0], rect[1], rect[2], rect[3] });
    }
    return frames;
}

constexpr std::vector<sf::Vector2f> get_turning_points(tmx_layer *layer_head)
{
    my_assert(layer_head != nullptr, "no layers found");
    while (layer_head)
    {
        if (layer_head->type == L_OBJGR) { break; }
        layer_head = layer_head->next;
    }
    my_assert(layer_head != nullptr, "no object layer found");

    std::vector<sf::Vector2f> points;
    auto current_object = layer_head->content.objgr->head;
    while (current_object)
    {
        points.push_back({ sf::Vector2f{ static_cast<float>(current_object->x),
          static_cast<float>(current_object->y) } });
        current_object = current_object->next;
    }
    std::reverse(points.begin(), points.end());
    return points;
}

auto normalize(sf::Vector2f vec) -> sf::Vector2f
{
    float length = std::sqrt(std::pow(vec.x, 2) + std::pow(vec.y, 2));
    return { vec.x / length, vec.y / length };
};

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


class EntityFactory
{
  public:
    EntityFactory(assets &manager, std::string_view frames_path)
      : manager(&manager)
    {
        std::ifstream anim_frames_file{ frames_path.data() };
        my_assert(anim_frames_file.is_open(), "failed to open frames file");

        auto frames_json = nlohmann::json::parse(anim_frames_file);

        demon_frames =
          load_animation_frames_json(frames_json, "big_demon_idle_anim");
        zombie_frames =
          load_animation_frames_json(frames_json, "big_zombie_idle_anim");
        wizard_frames =
          load_animation_frames_json(frames_json, "wizzard_m_idle_anim");
        texture = manager.get<sf::Texture>("../characters.png");
    }

    void set_enemy_turning_points(std::vector<sf::Vector2f> &&points)
    {
        enemies_turning_points = std::move(points);
    }

    auto spawn_zombie(ginseng::database &db)
    {
        auto id = db.create_entity();

        components::Animation anim{ &zombie_frames, *texture, 0.1f };
        anim.setScale(1.5, 1.5f);
        anim.setPosition(enemies_turning_points.front() - sprite_center(anim));
        db.add_component(id, anim);
        db.add_component(id, components::Velocity{ 100.f });
        db.add_component(id, components::Health{ 100.f, 100.f });
        db.add_component(id, components::EnemyPath{});
        return id;
    }

    auto spawn_demon(ginseng::database &db)
    {
        auto id = db.create_entity();
        components::Animation anim{ &demon_frames, *texture, 0.1f };
        anim.setScale(1.5, 1.5f);
        anim.setPosition(enemies_turning_points.front() - sprite_center(anim));
        db.add_component(id, anim);

        db.add_component(id, components::Velocity{ 200.f });
        db.add_component(id, components::Health{ 70.f, 70.f });
        db.add_component(id, components::EnemyPath{});
        return id;
    }

    auto spawn_wizard(ginseng::database &db)
    {
        auto id = db.create_entity();
        components::Animation anim{ &wizard_frames, *texture, 0.1f };
        anim.setScale(2.f, 2.f);
        anim.setPosition(
          enemies_turning_points.front() - sf::Vector2f{ 0, 100 });
        anim.setOrigin(sprite_center(anim));
        db.add_component(id, anim);

        db.add_component(id, components::player_tag{});
        return id;
    }

    auto get_zombie_frames() const { return zombie_frames; }
    auto get_demon_frames() const { return demon_frames; }
    auto get_enemy_turning_points() const { return enemies_turning_points; }

  private:
    assets *manager;
    std::shared_ptr<sf::Texture> texture;

    // frames of different kinds of enemies, right now only demon and zombie
    std::vector<sf::IntRect> demon_frames;
    std::vector<sf::IntRect> wizard_frames;
    std::vector<sf::IntRect> zombie_frames;
    std::vector<sf::Vector2f> enemies_turning_points;
};


int main(int argc, char *argv[])
{

    try
    {
        sf::RenderWindow win{ sf::VideoMode{ win_size.x, win_size.y },
            "Tower Defense in the works" };
        const auto middle_screen = sf::VideoMode::getDesktopMode().width / 4;
        win.setPosition({ static_cast<int>(middle_screen), 0 });
        win.setFramerateLimit(60);
        win.setKeyRepeatEnabled(false);

        assets manager{ "./assets/tiled" };

        Tilemap map{ "./assets/tiled/1.tmx", manager };


        EntityFactory factory{ manager, "./assets/frames.json" };
        factory.set_enemy_turning_points(
          get_turning_points(map.get_map()->ly_head));


        // ecs
        ginseng::database db;
        factory.spawn_wizard(db);


        sf::Font f;
        if (!f.loadFromFile(font_path))
            throw std::runtime_error{ "failed to load font" };

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
                    db.visit([&](components::Animation &anim, components::player_tag) {
                        anim.setRotation(
                          std::atan2(mouse_pos.y - anim.getPosition().y,
                            mouse_pos.x - anim.getPosition().x)
                          * 180 / 3.14159265);
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
                    } else if (ev.key.code == sf::Keyboard::Space)
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
                            projectile.damage = 10;
                            projectile.setSize({ 5, 5 });
                            projectile.setPosition(anim.getPosition());
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
                      vel))
                {
                    db.destroy_entity(id);
                }

                if (health.health <= 0) { db.destroy_entity(id); }
            });
            db.visit([&](ginseng::database::ent_id id,
                       components::Projectile &projectile) {
                projectile.move(projectile.direction * projectile.speed * delta);
                db.visit([&](components::Animation &anim,
                           components::Health &health) {
                    if (projectile.getGlobalBounds().intersects(anim.getGlobalBounds()))
                    {
                        health.health -= projectile.damage;
                        db.destroy_entity(id);
                    }
                });
            });


            win.clear();
            win.draw(map);
            db.visit([&](Animation &anim,
                       ginseng::optional<components::Health> health) {
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

            win.display();

        }
    } catch (const std::exception &e)
    {
        fmt::println(
          "exception caught when running the game: \n\t{}", e.what());
        return 1;
    }
}
