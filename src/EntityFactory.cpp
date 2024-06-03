module EntityFactory;
import utils;
import json;
import fmt;
import ginseng;

using ent_id = ginseng::database::ent_id;

EntityFactory::EntityFactory(my_assets &manager, std::string_view frames_path)
  : manager(&manager)
{
  std::ifstream anim_frames_file{ frames_path.data() };
  my_assert(anim_frames_file.is_open(), "failed to open frames file");

  auto frames_json = nlohmann::json::parse(anim_frames_file);

  demon_frames = load_animation_frames_json(frames_json, "big_demon_idle");
  zombie_frames = load_animation_frames_json(frames_json, "big_zombie_idle");
  wizard_frames = load_animation_frames_json(frames_json, "wizzard_idle");
  knight_frames = load_animation_frames_json(frames_json, "knight_idle");

  {
    my_assert(frames_json.contains("wizzard_idle_origin")
              && frames_json["wizzard_idle_origin"].is_array());
    std::vector<float> vecarr = frames_json["wizzard_idle_origin"];
    wizard_origin.x = vecarr[0];
    wizard_origin.y = vecarr[1];
  }

  texture = manager.get<sf::Texture>("../characters.png");
}
void EntityFactory::set_enemy_turning_points(std::vector<sf::Vector2f> &&points)
{
  enemies_turning_points = std::move(points);
}


ent_id EntityFactory::spawn_enemy(ginseng::database &db,
  components::enemy_type type)
{
  switch (type)
  {
  case components::enemy_type::zombie:
    fmt::print("spawning zombie\n");
    return spawn_zombie(db);
  case components::enemy_type::demon:
    return spawn_demon(db);
  default:
    my_assert(false, "unknown enemy type");
  }
  std::unreachable();
}

ent_id EntityFactory::spawn_zombie(ginseng::database &db)
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

ent_id EntityFactory::spawn_demon(ginseng::database &db)
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

ent_id EntityFactory::spawn_wizard(ginseng::database &db, sf::Vector2f position)
{
  auto id = db.create_entity();
  components::Animation anim{ &wizard_frames, *texture, 0.1f };
  anim.setOrigin(wizard_origin);
  anim.setScale(2.f, 2.f);
  anim.setPosition(position);
  db.add_component(id, anim);

  db.add_component(id, components::player_tag{});
  return id;
}


ent_id EntityFactory::spawn_knight(ginseng::database &db, sf::Vector2f position)
{
  auto id = db.create_entity();
  components::Animation anim{ &knight_frames, *texture, 0.1f };
  // anim.setOrigin(wizard_origin);
  anim.setScale(2.f, 2.f);
  anim.setPosition(position);
  db.add_component(id, anim);

  db.add_component(id, components::player_tag{});
  return id;
}
