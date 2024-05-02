module;
#include <random>
export module Random;

export
{
    class Random
    {
      public:
        Random() : m_gen(m_rd())
        {
        }

        template <typename T>
        T get(T min, T max)
            requires std::is_arithmetic_v<T>
        {
            if constexpr (std::is_floating_point<T>::value)
            {
                std::uniform_real_distribution<T> dist(min, max);
                return dist(m_gen);
            }
            else
            {
                std::uniform_int_distribution<T> dist(min, max);
                return dist(m_gen);
            }
        }
        template <typename C, typename T = float> C get_vec2(auto min, auto max)
        {
            return C{get<T>(min, max), get<T>(min, max)};
        }

      private:
        std::random_device m_rd;
        std::mt19937 m_gen;
    };
}
