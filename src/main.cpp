import sfml;
import stdbridge;
import assets;
import utils;
import json;
import tilemap;
import say;
import random;
import components;
import EntityFactory;
import ShooterSelector;
import LevelManager;


using namespace components;

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
        LevelManager level_manager {get_resource_path() + "assets/levels.json"};


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
