export module test;
export import ut;
import std;

export namespace utils {

  auto should_throw(auto fn)
  {
    using namespace ut;
    try {
      fn();
      std::print("should throw but didn't\n");
      expect(false == true_b);
    } catch (const std::exception& e) {
      expect(true == true_b);
    }
  };

} // namespace utils
