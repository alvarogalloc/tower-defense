export module test;
export import ut;
import std;

export namespace utils {

  auto should_throw(auto fn)
  {
    using namespace boost::ut;
    try {
      fn();
      std::print("should throw but didn't\n");
      expect(false == not "true"_b);
    } catch (const std::exception& e) {
      expect(true == "true"_b);
    }
  };

} // namespace utils
