module tests;
import ut;
import ginseng;

using namespace ut;

namespace {

struct resources
{
  ginseng::database db;
};
auto& get_shared_resources()
{
  static resources res;
  return res;
}

void tilemap_test()
{
  "tilemap"_test = [] mutable{
    auto& res  = get_shared_resources();
    res.db.create_entity();
    res.db.create_entity();
    res.db.create_entity();
    res.db.create_entity();
    res.db.create_entity();
    expect(5_ul == res.db.size()); 
  };
}
}// namespace

std::unordered_map<std::string, void (*)()> get_tests()
{
  return { { "tilemap_test", tilemap_test } };
}
