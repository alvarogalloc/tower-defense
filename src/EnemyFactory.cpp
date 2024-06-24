module EnemyFactory;
import utils;

EnemyFactory::EnemyFactory(my_assets &manager,
  const nlohmann::json &frames_json)
{
  m_frames_texture = manager.get<sf::Texture>("characters.png");
  for (auto &enemy : frames_json)
  {
    enemy_data data;
    data.type = enemy["type"].get<components::enemy_type>();
    data.health = enemy["health"];
    data.speed = enemy["speed"];
    for (auto &frame : enemy["frames"]["idle"])
    {
      auto rect = sf::IntRect(frame[0], frame[1], frame[2], frame[3]);
      data.frames.push_back(rect);
    }
    m_enemy_data.emplace(data.type, data);
  }
}

EnemyFactory::ent_id EnemyFactory::spawn_enemy(ginseng::database &db,
  components::enemy_type type,
  sf::Vector2f spawn_point)
{
  auto id = db.create_entity();
  auto target = m_enemy_data.at(type);
  components::Animation anim{ target.frames, *m_frames_texture, 0.1f };
  anim.setScale(1.5, 1.5f);

  anim.setPosition(spawn_point - sprite_center(anim));
  db.add_component(id, anim);
  db.add_component(id, components::Velocity{ target.speed });
  db.add_component(id, components::Health{ target.health });
  db.add_component(id, components::EnemyPath{});
  return id;
}
