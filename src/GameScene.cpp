module GameScene;
import utils;
import imgui;
import sfml;
import json;
import EnemySystem;
import BulletSystem;

#if 0
using namespace components;

void GameScene::on_start()
{
  m_factory = std::make_unique<EntityFactory>(
    *m_manager, m_manager->get_path() + "frames.json");

  m_levels = std::make_unique<LevelManager>(
    m_manager->get_path() + levels_file.data(), *m_factory);

  m_curent_map =
    m_manager->get<Tilemap>(m_levels->get_current_level().m_tilemap_name);

  m_spawn_points = get_points(m_curent_map->get_map()->ly_head, "spawnpoints");

  m_factory->set_enemy_turning_points(
    get_points(m_curent_map->get_map()->ly_head, "turningpoints"));
  m_shooters =
    std::make_unique<ShooterSelector>(m_manager->get<sf::Font>("monogram.ttf"));

  m_shooters->add_option(sf::Color::Blue,
    shooter_type::wizard,
    *m_manager->get<sf::Texture>("characters.png"),
    m_factory->get_wizard_frames().back());
  m_shaded_tile.setSize({ 32, 64 });
}


auto get_distance_from_spawn_point(const sf::Vector2f &point,
  const sf::Vector2f &spawn_point)
{
  return std::sqrt(std::pow(point.x - spawn_point.x, 2)
                   + std::pow(point.y - spawn_point.y, 2));
}

void GameScene::on_update(float delta)
{
  m_world->visit([&](components::Animation &anim) { anim.update(delta); });
  enemy_system(delta);
  bullet_system(delta);
  auto type = m_levels->update(delta);
  if (type != components::enemy_type::none)
  {
    m_factory->spawn_enemy(*m_world, type);
  }
}
void GameScene::on_event(const sf::Event &e)
{
  auto [x, y] = sf::Mouse::getPosition(*m_win);
  auto mouse_pos = sf::Vector2f{ static_cast<float>(x), static_cast<float>(y) };
  if (e.type == sf::Event::KeyPressed)
  {
    if (e.key.code == sf::Keyboard::Escape)
    {
      m_should_exit = true;
    } else if (e.key.code == sf::Keyboard::S)
    {
      m_factory->spawn_zombie(*m_world);
    } else if (e.key.code == sf::Keyboard::D)
    {
      m_factory->spawn_demon(*m_world);
    } else if (e.key.code == sf::Keyboard::Space)
    {
      m_world->visit([&](components::Animation &anim, components::player_tag) {
        auto bullet_id = m_world->create_entity();
        components::Projectile projectile;
        // calculate the direction of the bullet with mouse
        // position
        projectile.direction = normalize(mouse_pos - anim.getPosition());
        projectile.speed = 500;
        projectile.damage = 20;
        projectile.setSize({ 10, 10 });
        // rotate the bullet by a fixed amount
        projectile.setPosition(anim.getPosition());
        projectile.setOrigin(sprite_center(projectile));
        m_world->add_component(bullet_id, projectile);
      });
    }
  } else if (e.type == sf::Event::MouseButtonPressed
             && e.mouseButton.button == sf::Mouse::Left)
  {
    auto spawn_info = m_shooters->choose_or_place(mouse_pos);
    if (spawn_info)
    {
     fmt::print("spawning shooter: {} at {}",
        to_string(spawn_info->type),
        to_string(spawn_info->pos));
      m_factory->spawn_shooter(
        *m_world, spawn_info->type, m_shaded_tile.getPosition());
      m_shooters->deselect();
    }
  } else if (e.type == sf::Event::MouseMoved)
  {
    // m_shaded_tile.setSize({ 32, 32 });
    // put in the corner of the tile
    // m_shaded_tile.setPosition({ static_cast<float>(e.mouseMove.x / 32)
    // * 32.f,
    //   static_cast<float>(e.mouseMove.y / 32) * 32.f });

    for (const auto &point : m_spawn_points)
    {
      auto old_pos = m_shaded_tile.getPosition();
      m_shaded_tile.setPosition(point);
      if (m_shaded_tile.getGlobalBounds().contains(mouse_pos))
      {
        m_shaded_tile.setFillColor(sf::Color::Red);
        break;
      }
      m_shaded_tile.setPosition(old_pos);
    }
  }
}

void GameScene::enemy_system(float delta)
{
  m_world->visit([&](ginseng::database::ent_id id,
                   components::Animation &anim,
                   components::EnemyPath &path,
                   const components::Velocity &vel,
                   components::Health &health) {
    if (health.health <= 0)
    {
      m_world->destroy_entity(id);
    } else
    {
      const auto &directions = m_factory->get_enemy_turning_points();
      bool should_delete = false;
      // move the anim to the next point

      // get unit vector pointing to the next point
      my_assert(path.current_point < directions.size(),
        "current goal point is out of bounds");
      const auto center = sprite_center(anim);
      float distance = std::sqrt(
        std::pow(
          directions[path.current_point].x - center.x - anim.getPosition().x, 2)
        + std::pow(
          directions[path.current_point].y - center.y - anim.getPosition().y,
          2));
      float min_distance_for_next_point = 3;
      if (distance < min_distance_for_next_point)
      {
        if (path.current_point == directions.size() - 1)
        {
          path.current_point = 0;
          should_delete = true;
        } else
        {
          ++path.current_point;
        }
      }
      auto move_vector =
        normalize(directions[path.current_point] - center - anim.getPosition())
        * vel.magnitude;
      anim.move(move_vector * delta);
      if (should_delete) m_world->destroy_entity(id);
    }
  });
}
void GameScene::bullet_system(float delta)
{
  m_world->visit([&](ginseng::database::ent_id id,
                   components::Projectile &projectile) {
    if (projectile.getPosition().x < 0
        || projectile.getPosition().x > win_size.x
        || projectile.getPosition().y < 0
        || projectile.getPosition().y > win_size.y)
    {
      m_world->destroy_entity(id);
    } else
    {
      projectile.move(projectile.direction * projectile.speed * delta);
      projectile.rotate(delta * 1000);

      m_world->visit([&](components::Animation &anim,
                       components::Health &health) {
        if (m_world->exists(id)
            && projectile.getGlobalBounds().intersects(anim.getGlobalBounds()))
        {
          health.health -= projectile.damage;
          m_world->destroy_entity(id);
        }
      });
    }
  });
}


void GameScene::on_render()
{
  m_win->clear(sf::Color::Red);
  if (m_curent_map) m_win->draw(*m_curent_map);
  // use second font imgui

  auto texture_count = m_manager->size<sf::Texture>();
  ImGui::Text("%zu Textures loaded", texture_count);
  auto tilemap_count = m_manager->size<Tilemap>();
  ImGui::Text("%zu Tilemaps loaded", tilemap_count);
  ImGui::Text("Entities spawned: %zu", m_world->size());

  m_world->visit(
    [&](Animation &anim, ginseng::optional<components::Health> health) {
      m_win->draw(anim);
      if (health) { health->draw(*m_win, anim); }
    });
  m_win->draw(*m_shooters);
  if (m_shooters->get_current_type() != shooter_type::none)
  {
    m_win->draw(m_shaded_tile);
    for (const auto &point : m_spawn_points)
    {
      sf::RectangleShape shape({ 32, 64 });
      shape.setPosition({ point.x, point.y });
      shape.setFillColor(sf::Color::Green);
      shape.setOutlineColor(sf::Color::Black);
      m_win->draw(shape);
    }
  }
  m_world->visit(
    [&](components::Projectile &projectile) { m_win->draw(projectile); });
}

std::unique_ptr<Scene> GameScene::on_exit() { return nullptr; }

#endif


// generate empty implementation

void GameScene::on_start()
{
  auto c_file = [](std::string_view path) {
    return std::unique_ptr<std::FILE, decltype(&std::fclose)>(
      std::fopen(path.data(), "r"), &std::fclose);
  };

  auto levels_json = nlohmann::json::parse(
    c_file(m_manager->get_path() + levels_file.data()).get());
  auto enemies_json = nlohmann::json::parse(
    c_file(m_manager->get_path() + enemies_file.data()).get());


  m_levels =
    std::make_unique<LevelManager>(levels_json, enemies_json, *m_manager);
  m_levels->set_draw_debug(true);
  m_enemy_system = make_enemy_system(*m_world);
  m_bullet_system =
    make_bullet_system(*m_world, sf::Vector2f{ m_win->getSize() });
}
void GameScene::on_update(float delta)
{
  m_levels->update(*m_world, delta);
  // this should be passed as turning points vary with level
  m_enemy_system(delta, m_levels->get_current_level().m_enemies_turning_points);
  m_world->visit([&](components::Animation &anim) { anim.update(delta); });
  m_bullet_system(delta);
}
void GameScene::on_event(const sf::Event &e)
{
  // if n pressed, go to next level
  if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::N)
  {
    m_levels->next_level();
  }


  auto mouse_pos = sf::Vector2f{ sf::Mouse::getPosition(*m_win) };
  if (e.type == sf::Event::KeyPressed)
  {
    if (e.key.code == sf::Keyboard::Escape)
    {
      m_should_exit = true;
    } else if (e.key.code == sf::Keyboard::Space)
    {
      m_world->visit([&](components::player_tag, components::Animation &anim) {
        auto bullet_id = m_world->create_entity();
        components::Projectile projectile;
        // calculate the direction of the bullet with mouse
        // position
        projectile.direction = normalize(mouse_pos - anim.getPosition());
        projectile.speed = 500;
        projectile.damage = 20;
        projectile.setSize({ 10, 10 });
        // rotate the bullet by a fixed amount
        projectile.setPosition(anim.getPosition());
        projectile.setOrigin(sprite_center(projectile));
        m_world->add_component(bullet_id, projectile);
      });
    }
  } else if (e.type == sf::Event::MouseButtonPressed
             && e.mouseButton.button == sf::Mouse::Left)
  {
    // should be:
    // if (in world)
    // if(m_shooters.selected() != none)
    // m_shooters->spawn(m_shooters.selected())
    // m_shooters->set_show_available_spawn_points(true)
    // else
    // m_shooters->deselect()
    // m_shooters->set_show_available_spawn_points(false)
    // else if (in selector)
    // m_shooter->select(mouse_pos)

    /*
    auto spawn_info = m_shooters->choose_or_place(mouse_pos);
    if (spawn_info)
    {
      say::info(fmt::format("spawning shooter: {} at {}",
        to_string(spawn_info->type),
        to_string(spawn_info->pos)));
      m_factory->spawn_shooter(
        *m_world, spawn_info->type, m_shaded_tile.getPosition());
      m_shooters->deselect();
    }
    */
  } else if (e.type == sf::Event::MouseMoved)
  {
    // iterate over all spawn points
    // and check if mouse is near (20px or less) to any of them
    // if it is, set the color of the spawn point to red
    // else set it to green
    // so selected_spawn_point should be a member of ShooterSelector
    // for (const auto &point : m_levels->get_current_level().m_spawn_points)
    // {
    //   // float distance = get_distance(mouse_pos, point);
    // }
  }
}
void GameScene::on_render()
{
  m_win->clear(sf::Color::Red);
  m_win->draw(*m_levels);
  ImGui::Text("Entities spawned: %zu", m_world->size());
  m_world->visit([&](components::Animation &anim) { m_win->draw(anim); });
}
std::unique_ptr<Scene> GameScene::on_exit() { return nullptr; }
