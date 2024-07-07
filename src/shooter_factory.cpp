module shooter_factory;


namespace {

constexpr components::shooter_type to_shooter_type(std::uint8_t type)
{
  return static_cast<components::shooter_type>(type);
}

shooter_factory::shooter_info load_shooter_info(const nlohmann::json &node)
{
  shooter_factory::shooter_info info;
  info.name = node.at("name").get<std::string_view>();
  info.damage = node.at("damage").get<std::uint8_t>();
  info.type = to_shooter_type(node.at("type").get<std::uint8_t>());

  // WIP ONLY IDLE FRAMES FOR NOW
  for (const auto &frame : node.at("frames").at("idle"))
  {
    auto rect = sf::IntRect(frame[0], frame[1], frame[2], frame[3]);
    info.frames.push_back(rect);
  }
  return info;
}
}// namespace

shooter_factory::shooter_factory(my_assets &manager, const nlohmann::json &json)
{
  for (const auto &node : json)
  {
    auto info = load_shooter_info(node);
    m_shooter_data[info.type] = info;
  }
}


shooter_factory::ent_id shooter_factory::spawn_shooter(ginseng::database &db,
  components::shooter_type type,
  sf::Vector2f position)
{}
