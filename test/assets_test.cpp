import stdbridge;
import boost.ut;

auto times2(std::uint64_t n) { return n * 2; }


int main(int argc, char *argv[])
{
    using namespace boost::ut;
    using namespace boost::ut::spec;

    // use spec
    describe("vector") = [] {
        std::vector<int> v(5);
        expect(fatal(5_ul == std::size(v)));

        it("should resize bigger") = [v] {
            mut(v).resize(10);
            expect(10_ul == std::size(v));
        };
    };
}
