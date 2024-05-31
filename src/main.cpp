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
import imgui;

using namespace components;

constexpr static auto font_path =
  "/Users/macuser/Library/Fonts/SFMono Regular Nerd Font Complete.otf";


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
    std::sqrt(std::pow(directions[current_goal_point.current_point].x - center.x
                         - zombie.getPosition().x,
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

auto oscilate_cyan(float time)
{
  float t = (std::sin(time) + 1.0f) / 2.0f;// Oscillate between 0 and 1
  return sf::Color(0, t * 255, t * 255, 255);// From black to dark cyan
}

enum class menu_state { none, start, credits, exit };
menu_state main_menu(float time)
{
  auto TextCentered = [](std::string text) {
    auto windowWidth = ImGui::GetWindowSize().x;
    auto textWidth = ImGui::CalcTextSize(text.c_str()).x;

    ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
    ImGui::Text("%s", text.c_str());
  };


  const auto menu_width = win_size.x / 2.f;
  ImGui::SetNextWindowSize(ImVec2(menu_width, win_size.y));
  ImGui::SetNextWindowPos(
    ImVec2((ImGui::GetIO().DisplaySize.x / 2.f) - (menu_width / 2.f),
      win_size.y / 4.f));

  ImGui::Begin("Start Menu",
    nullptr,
    ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize
      | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration
      | ImGuiWindowFlags_NoBackground);

  menu_state state = menu_state::none;
  TextCentered("Tower Defense");
  ImGui::SetCursorPosY(50);
  if (ImGui::Button("Start", ImVec2(menu_width, 50)))
  {
    // Handle start button click
    state = menu_state::start;
  }
  ImGui::SetCursorPosY(120);
  if (ImGui::Button("Credits", ImVec2(menu_width, 50)))
  {
    // Handle credits button click
    state = menu_state::credits;
  }
  ImGui::SetCursorPosY(190);
  if (ImGui::Button("Exit", ImVec2(menu_width, 50)))
  {
    state = menu_state::exit;
  }

  ImGui::End();
  return state;
}


int main(int argc, char *argv[])
{

  try
  {
    const std::string path = get_resource_path() + "../assets/tiled/";
    fmt::println("path: {}", path);
    bool game_started = false;
    float accumulated_time = 0;
    sf::RenderWindow win{ sf::VideoMode{ win_size.x, win_size.y },
      "Tower Defense in the works",
      sf::Style::Default };
    if (!ImGui::SFML::Init(win, false))
    {
      throw std::runtime_error{ "failed to initialize imgui" };
    }
    auto &io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF(font_path, 16.0f);
    my_assert(ImGui::SFML::UpdateFontTexture(), "could not build font atlas");
    const auto middle_screen = sf::VideoMode::getDesktopMode().width / 4;
    win.setPosition({ static_cast<int>(middle_screen), 0 });
    win.setFramerateLimit(60);
    win.setKeyRepeatEnabled(false);

    assets manager{ path };
    // auto music =
    // manager.get<sf::Music>("/Users/macuser/Downloads/bgmusic.mp3");
    // music->play();
    Tilemap map{ path + "1.tmx", manager };


    EntityFactory factory{ manager, path + "../frames.json" };
    factory.set_enemy_turning_points(
      get_turning_points(map.get_map()->ly_head));
    LevelManager level_manager{ path + "../levels.json", factory };

    sf::Font font;
    if (!font.loadFromFile(font_path))
      throw std::runtime_error{ "failed to load font" };

    ShooterSelector selector(&font);

    selector.add_option(sf::Color::Red,
      shooter_type::wizard,
      *manager.get<sf::Texture>("../characters.png"),
      factory.get_wizard_frames().back());

    selector.add_option(sf::Color::Cyan,
      shooter_type::knight,
      *manager.get<sf::Texture>("../characters.png"),
      factory.get_knight_frames().back());


    Random rd;
    // ecs
    ginseng::database db;
    // factory.spawn_wizard(db, rd.get_vec2<sf::Vector2f>(100, 500));
    // factory.spawn_wizard(db, rd.get_vec2<sf::Vector2f>(100, 500));
    // factory.spawn_wizard(db, rd.get_vec2<sf::Vector2f>(100, 500));
    // factory.spawn_wizard(db, rd.get_vec2<sf::Vector2f>(100, 500));


    sf::Event ev;
    sf::Clock clock;
    float delta = 0;
    auto mouse_pos = sf::Vector2f{ 0, 0 };
    const auto max_in_level = level_manager.get_current_level().m_waves.size();
    while (win.isOpen())
    {
      while (win.pollEvent(ev))
      {
        ImGui::SFML::ProcessEvent(ev);
        if (ev.type == sf::Event::Closed) { win.close(); }
        if (ev.type == sf::Event::MouseMoved)
        {
          mouse_pos = win.mapPixelToCoords({ ev.mouseMove.x, ev.mouseMove.y });
          db.visit([&](components::Animation &anim, components::player_tag) {
            // rotate around its origin
            auto direction = normalize(mouse_pos - anim.getPosition());
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

            db.visit([&](components::Animation &anim, components::player_tag) {
              auto bullet_id = db.create_entity();
              components::Projectile projectile;
              // calculate the direction of the bullet with mouse
              // position
              projectile.direction = normalize(mouse_pos - anim.getPosition());
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
      ImGui::SFML::Update(win, clock.getElapsedTime());
      delta = clock.restart().asSeconds();
      accumulated_time += delta;
      if (game_started)
      {

        ImGui::Text("Entities spawned: %zu", db.size());
        auto type = level_manager.update(delta);
        if (type != components::enemy_type::none)
        {
          factory.spawn_enemy(db, type);
        }

        db.visit([&](components::Animation &anim) { anim.update(delta); });

        db.visit([&](ginseng::database::ent_id id,
                   components::Animation &anim,
                   components::EnemyPath &path,
                   const components::Velocity &vel,
                   components::Health &health) {
          if (enemy_system(
                anim, factory.get_enemy_turning_points(), delta, path, vel)
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
            projectile.move(projectile.direction * projectile.speed * delta);
            projectile.rotate(delta * 1000);

            db.visit(
              [&](components::Animation &anim, components::Health &health) {
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
      }
      win.clear();
      if (game_started)
      {

        win.draw(map);
        db.visit([&](Animation &anim,
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
        db.visit(
          [&](components::Projectile &projectile) { win.draw(projectile); });
        // draw_shooter_select(win, shooter_selectors);
        draw_select_tile(win, mouse_pos);
        win.draw(selector);
      } else
      {
        win.clear(oscilate_cyan(accumulated_time));
        switch (main_menu(accumulated_time))
        {
        case menu_state::start:
          game_started = true;
          break;
        case menu_state::credits:
          fmt::println("credits");
          break;
        case menu_state::exit:
          win.close();
          break;
        default:
          break;
        }
      }

      ImGui::EndFrame();
      ImGui::SFML::Render(win);
      win.display();
    }
    ImGui::SFML::Shutdown();
  } catch (const std::exception &e)
  {
    fmt::println("exception caught when running the game: \n\t{}", e.what());
    return 1;
  }
}
