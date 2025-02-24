export module ut;
import std;
#define NTEST
#define UT_RUN_TIME_ONLY
export namespace ut::inline v2_1_1 {
  namespace type_traits {
    template <class, class>
    inline constexpr auto is_same_v = false;
    template <class T>
    inline constexpr auto is_same_v<T, T> = true;
    template <class T>
    inline constexpr auto is_floating_point_v = false;
    template <>
    inline constexpr auto is_floating_point_v<float> = true;
    template <>
    inline constexpr auto is_floating_point_v<double> = true;
    template <>
    inline constexpr auto is_floating_point_v<long double> = true;
    template <class>
    inline constexpr auto is_mutable_lambda_v = false;
    template <class R, class B, class... Ts>
    inline constexpr auto is_mutable_lambda_v<R (B::*)(Ts...)> = true;
    template <class Fn>
    inline constexpr auto has_capture_lambda_v = sizeof(Fn) > 1ul;
  } // namespace type_traits
  namespace utility {
    template <class T>
    T&& declval();
    template <class T, class...>
    struct type_identity {
      using type = T;
    };
    template <unsigned Size>
    struct fixed_string {
      constexpr fixed_string(const char (&str)[Size])
      {
        for (auto i = 0u; i < Size; ++i) {
          storage[i] = str[i];
        }
      }
      [[nodiscard]] constexpr auto operator[](const auto i) const
      {
        return storage[i];
      }
      [[nodiscard]] constexpr auto data() const
      {
        return storage;
      }
      [[nodiscard]] static constexpr auto size()
      {
        return Size;
      }
      constexpr friend auto operator<<(auto& os, const fixed_string& fs) -> decltype(auto)
      {
        return os << fs.storage;
      }
      char storage[Size] {};
    };
  } // namespace utility
  namespace events {
    enum class mode { run_time, compile_time };
    template <mode Mode>
    struct test_begin {
      const char* file_name {};
      int line {};
      const char* name {};
    };
    template <mode Mode>
    struct test_end {
      const char* file_name {};
      int line {};
      const char* name {};
      enum { FAILED, PASSED, COMPILE_TIME } result {};
    };
    template <class TExpr>
    struct assert_pass {
      const char* file_name {};
      int line {};
      TExpr expr {};
    };
    template <class TExpr>
    struct assert_fail {
      const char* file_name {};
      int line {};
      TExpr expr {};
    };
    struct fatal {};
    template <class TMsg>
    struct log {
      const TMsg& msg;
      bool result {};
    };
    struct summary {
      enum { FAILED, PASSED, COMPILE_TIME };
      unsigned asserts[2] {}; /* FAILED, PASSED */
      unsigned tests[3] {};   /* FAILED, PASSED, COMPILE_TIME */
    };
  } // namespace events

  template <class TOStream>
  class outputter {
public:
    template <events::mode Mode>
    constexpr auto on(const events::test_begin<Mode>&)
    {}
    constexpr auto on(const events::test_begin<events::mode::run_time>& event)
    {
      current_test = event;
    }
    template <events::mode Mode>
    constexpr auto on(const events::test_end<Mode>&)
    {}
    template <class TExpr>
    constexpr auto on(const events::assert_pass<TExpr>&)
    {}
    template <class TExpr>
    constexpr auto on(const events::assert_fail<TExpr>& event)
    {
      if (!__builtin_is_constant_evaluated()) {
        if (initial_new_line == '\n') {
          os << initial_new_line;
        } else {
          initial_new_line = '\n';
        }
        os << event.file_name << ':' << event.line << ':' << "FAILED:" << '\"' << current_test.name
           << "\": " << event.expr;
      }
    }
    constexpr auto on(const events::fatal&) {}
    template <class TMsg>
    constexpr auto on(const events::log<TMsg>& event)
    {
      if (!__builtin_is_constant_evaluated() && !event.result) {
        os << ' ' << event.msg;
      }
    }
    constexpr auto on(const events::summary& event)
    {
      if (!__builtin_is_constant_evaluated()) {
        if (event.asserts[events::summary::FAILED] || event.tests[events::summary::FAILED]) {
          os << "\nFAILED: ";
        } else {
          os << "PASSED: ";
        }
        os << "tests: "
           << (event.tests[events::summary::PASSED] + event.tests[events::summary::FAILED]) << " ("
           << event.tests[events::summary::PASSED] << " passed, "
           << event.tests[events::summary::FAILED] << " failed, "
           << event.tests[events::summary::COMPILE_TIME] << " compile-time), "
           << "asserts: "
           << (event.asserts[events::summary::PASSED] + event.asserts[events::summary::FAILED])
           << " (" << event.asserts[events::summary::PASSED] << " passed, "
           << event.asserts[events::summary::FAILED] << " failed)\n";
      }
    }

    TOStream& os;
    events::test_begin<events::mode::run_time> current_test {};
    char initial_new_line {};
  };

  template <class TOutputter, auto MaxDepth = 16u>
  struct reporter {
    constexpr auto on(const events::test_begin<events::mode::run_time>& event)
    {
      asserts_failed[current++] = summary.asserts[events::summary::FAILED];
      outputter.on(event);
    }
    constexpr auto on(const events::test_end<events::mode::run_time>& event)
    {
      const auto result = summary.asserts[events::summary::FAILED] == asserts_failed[--current];
      ++summary.tests[result];
      events::test_end<events::mode::run_time> te {event};
      te.result = static_cast<decltype(te.result)>(result);
      outputter.on(te);
    }
    constexpr auto on(const events::test_begin<events::mode::compile_time>&)
    {
      ++summary.tests[events::summary::COMPILE_TIME];
    }
    constexpr auto on(const events::test_end<events::mode::compile_time>&) {}
    template <class TExpr>
    constexpr auto on(const events::assert_pass<TExpr>& event)
    {
      ++summary.asserts[events::summary::PASSED];
      outputter.on(event);
    }
    template <class TExpr>
    constexpr auto on(const events::assert_fail<TExpr>& event)
    {
      ++summary.asserts[events::summary::FAILED];
      outputter.on(event);
    }
    constexpr auto on(const events::fatal& event)
    {
      ++summary.tests[events::summary::FAILED];
      outputter.on(event);
      outputter.on(summary);
      __builtin_abort();
    }

#if !defined(UT_COMPILE_TIME_ONLY)
    ~reporter()
    { // non constexpr
      outputter.on(summary);
      if (summary.asserts[events::summary::FAILED]) {
        __builtin_abort();
      }
    }
#endif

    TOutputter& outputter;
    events::summary summary {};
    unsigned asserts_failed[MaxDepth] {};
    unsigned current {};
  };

  template <class TReporter>
  struct runner {
    template <class Test>
    constexpr auto on(Test test, const char* file_name, int line, const char* name) -> bool
    {
      if (__builtin_is_constant_evaluated()) {
        if constexpr (!type_traits::is_mutable_lambda_v<decltype(&Test::operator())>) {
          test();
          return true;
        } else {
          return false;
        }
      } else {
#if !defined(UT_RUN_TIME_ONLY)
        if constexpr (
          !type_traits::is_mutable_lambda_v<decltype(&Test::operator())>
          && !type_traits::has_capture_lambda_v<Test>) {
          reporter.on(events::test_begin<events::mode::compile_time> {file_name, line, name});
          static_assert((test(), "[FAILED]"));
          reporter.on(events::test_end<events::mode::compile_time> {file_name, line, name});
        }
#endif

#if !defined(UT_COMPILE_TIME_ONLY)
        reporter.on(events::test_begin<events::mode::run_time> {file_name, line, name});
        test();
        reporter.on(events::test_end<events::mode::run_time> {file_name, line, name});
#endif
      }
      return true;
    }

    TReporter& reporter;
  };
} // namespace ut::inline v2_1_1

export namespace ut::inline v2_1_1 {
  template <class...>
  struct default_cfg {
    struct {
      friend constexpr decltype(auto)
      operator<<([[maybe_unused]] auto& os, [[maybe_unused]] const auto& t)
      {
#if defined(UT_COMPILE_TIME_ONLY)
        return os;
#else
        static_assert(
          requires { std::clog << t; },
          "[ERROR] No output supported: Consider #include <iostream> | "
          "ut::cfg<ut::override> = custom_cfg{} | #define UT_COMPILE_TIME_ONLY");
        return (std::clog << t);
#endif
      }
    } stream;
    ut::outputter<decltype(stream)> outputter {stream};
    ut::reporter<decltype(outputter)> reporter {outputter};
    ut::runner<decltype(reporter)> runner {reporter};
  };

  struct override {};

  /**
 * Customization point to override the default configuration
 *
 * @code
 * template<class... Ts> auto ut::cfg<ut::override, Ts...> = my_config{};
 * @endcode
 */
  template <class... Ts>
  inline default_cfg<Ts...> cfg {};

  namespace detail {
    template <class... Ts>
    [[nodiscard]] constexpr auto& cfg(Ts&&...)
    {
      return ut::cfg<typename utility::type_identity<override, Ts...>::type>;
    }
    void failed(); /// fail in constexpr context
  } // namespace detail

  inline constexpr struct {
    constexpr auto operator()(
      auto expr, const char* file_name = __builtin_FILE(), int line = __builtin_LINE()) const
    {
      if constexpr (constexpr auto supported = type_traits::is_same_v<bool, decltype(expr)>
                      || !requires { static_cast<bool>(expr); };
                    supported) {
        static_assert(!supported, "[ERROR] Expression required: expect(42_i == 42)");
      } else {
        bool result {};
        if (result = static_cast<bool>(expr); __builtin_is_constant_evaluated()) {
          if (!result) {
            detail::failed();
          }
        } else if (result) {
          detail::cfg(expr).reporter.on(events::assert_pass {file_name, line, expr});
        } else {
          detail::cfg(expr).reporter.on(events::assert_fail {file_name, line, expr});
        }
        return log {result};
      }
    }

    struct fatal_expr {
      constexpr fatal_expr(
        auto expr, const char* file_name = __builtin_FILE(), int line = __builtin_LINE())
      {
        if constexpr (constexpr auto supported = type_traits::is_same_v<bool, decltype(expr)>
                        || !requires { static_cast<bool>(expr); };
                      supported) {
          static_assert(!supported, "[ERROR] Expression required: expect[42_i == 42]");
        } else {
          if (result = static_cast<bool>(expr); __builtin_is_constant_evaluated()) {
            if (!result) {
              detail::failed();
            }
          } else if (result) {
            detail::cfg(expr).reporter.on(events::assert_pass {file_name, line, expr});
          } else {
            detail::cfg(expr).reporter.on(events::assert_fail {file_name, line, expr});
            detail::cfg(expr).reporter.on(events::fatal {});
          }
        }
      }
      bool result {};
    };
    constexpr auto operator[](fatal_expr e) const
    {
      return log {e.result};
    } /// multiple and/or default parameters requires C++23

private:
    struct log {
      template <class TMsg>
      constexpr const auto& operator<<(const TMsg& msg) const
      {
        detail::cfg(msg).outputter.on(events::log<TMsg> {msg, result});
        return *this;
      }
      bool result {};
    };
  } expect {};

  struct suite {
    template <class Test>
    suite(Test test)
    {
      test();
    } /// not constexpr
  };

  namespace detail {
    template <utility::fixed_string Name>
    struct test {
      struct run {
        template <class T>
        constexpr run(T test, const char* file_name = __builtin_FILE(), int line = __builtin_LINE())
            : result {cfg(test).runner.on(test, file_name, line, Name.data())}
        {}
        bool result {};
      };
      constexpr auto operator=(run test) const
      {
        return test.result;
      }
    };
  } // namespace detail

  template <utility::fixed_string Str>
  [[nodiscard]] constexpr auto operator""_test()
  {
    return detail::test<Str> {};
  }

  template <auto Expr>
  inline constexpr auto constant = Expr;
  template <class T>
  [[nodiscard]] constexpr auto& mut(const T& t)
  {
    return const_cast<T&>(t);
  }

  template <class TLhs, class TRhs>
  struct eq {
    constexpr eq(const TLhs& lhs, const TRhs& rhs)
        : lhs {lhs}
        , rhs {rhs}
    {}
    static_assert(
      type_traits::is_same_v<TLhs, TRhs>, "[ERROR] Comparision of different types is not allowed");
    [[nodiscard]] constexpr explicit operator bool() const
    {
      return false;
    }
    const TLhs& lhs;
    const TRhs& rhs;
  };
  template <class T>
  struct eq<T, T> {
    constexpr eq(const T& lhs, const T& rhs)
        : lhs {lhs}
        , rhs {rhs}
        , result {lhs == rhs}
    {}
    constexpr friend auto operator<<(auto& os, const eq& expr) -> decltype(auto)
    {
      return (os << expr.lhs << " == " << expr.rhs);
    }
    [[nodiscard]] constexpr explicit operator bool() const
    {
      return result;
    }
    T lhs;
    T rhs;
    bool result {};
  };
  template <class T>
    requires type_traits::is_floating_point_v<T>
  struct eq<T, T> {
    constexpr eq(const T& lhs, const T& rhs)
        : lhs {lhs}
        , rhs {rhs}
    {}
    constexpr friend auto operator<<(auto& os, const eq& expr) -> decltype(auto)
    {
      return (os << expr.lhs << " == " << expr.rhs);
    }
    [[nodiscard]] constexpr explicit operator bool() const
    {
      static_assert(
        !type_traits::is_floating_point_v<T>,
        "[ERROR] Epsilon is required: expect((4.2 == 4.2_d)(.01))");
      return {};
    }
    struct epsilon {
      constexpr epsilon(const T& lhs, const T& rhs, const T e)
          : lhs {lhs}
          , rhs {rhs}
          , e {e}
          , result {(lhs < rhs ? rhs - lhs : lhs - rhs) < e}
      {}
      [[nodiscard]] constexpr explicit operator bool() const
      {
        return result;
      }
      constexpr friend auto operator<<(auto& os, const epsilon& expr) -> decltype(auto)
      {
        return (os << "(" << expr.lhs << " == " << expr.rhs << ")(" << expr.e << ")");
      }
      T lhs;
      T rhs;
      T e;
      bool result {};
    };
    [[nodiscard]] constexpr auto operator()(T e) const
    {
      return epsilon {lhs, rhs, e};
    }
    T lhs;
    T rhs;
  };
  template <class TLhs, class TRhs>
    requires requires(TLhs lhs, TRhs rhs) {
      lhs[0];
      rhs[0];
      lhs.size();
      rhs.size();
    }
  struct eq<TLhs, TRhs> {
    static_assert(
      type_traits::is_same_v<
        decltype(utility::declval<TLhs>()[0]), decltype(utility::declval<TLhs>()[0])>,
      "[ERROR] Comparision of different underlying types is not allowed");
    constexpr eq(const TLhs& lhs, const TRhs& rhs)
        : lhs {lhs}
        , rhs {rhs}
        , result {[](const auto& lhs, const auto& rhs) {
          if (lhs.size() != rhs.size()) {
            return false;
          }
          for (decltype(lhs.size()) i {}; i < lhs.size(); ++i) {
            if (lhs[i] != rhs[i]) {
              return false;
            }
          }
          return true;
        }(lhs, rhs)}
    {}
    constexpr friend auto operator<<(auto& os, const eq& expr) -> decltype(auto)
    {
      return (os << expr.lhs << " == " << expr.rhs);
    }
    [[nodiscard]] constexpr explicit operator bool() const
    {
      return result;
    }
    TLhs lhs;
    TRhs rhs;
    bool result {};
  };

  template <class TLhs, class TRhs>
  struct neq {
    constexpr neq(const TLhs& lhs, const TRhs& rhs)
        : lhs {lhs}
        , rhs {rhs}
    {}
    static_assert(
      type_traits::is_same_v<TLhs, TRhs>, "[ERROR] Comparision of different types is not allowed");
    [[nodiscard]] constexpr explicit operator bool() const
    {
      return false;
    }
    const TLhs& lhs;
    const TRhs& rhs;
  };
  template <class T>
  struct neq<T, T> {
    constexpr neq(const T& lhs, const T& rhs)
        : lhs {lhs}
        , rhs {rhs}
        , result {lhs != rhs}
    {}
    constexpr friend auto operator<<(auto& os, const neq& expr) -> decltype(auto)
    {
      return (os << expr.lhs << " != " << expr.rhs);
    }
    [[nodiscard]] constexpr explicit operator bool() const
    {
      return result;
    }
    T lhs;
    T rhs;
    bool result {};
  };
  template <class T>
    requires type_traits::is_floating_point_v<T>
  struct neq<T, T> {
    constexpr neq(const T& lhs, const T& rhs)
        : lhs {lhs}
        , rhs {rhs}
    {}
    constexpr friend auto operator<<(auto& os, const neq& expr) -> decltype(auto)
    {
      return (os << expr.lhs << " != " << expr.rhs);
    }
    [[nodiscard]] constexpr explicit operator bool() const
    {
      static_assert(
        !type_traits::is_floating_point_v<T>,
        "[ERROR] Epsilon is required: expect((4.2 != 4.2_d)(.1))");
      return {};
    }
    struct epsilon {
      constexpr epsilon(const T& lhs, const T& rhs, const T e)
          : lhs {lhs}
          , rhs {rhs}
          , e {e}
          , result {(lhs < rhs ? rhs - lhs : lhs - rhs) >= e}
      {}
      [[nodiscard]] constexpr explicit operator bool() const
      {
        return result;
      }
      constexpr friend auto operator<<(auto& os, const epsilon& expr) -> decltype(auto)
      {
        return (os << "(" << expr.lhs << " != " << expr.rhs << ")(" << expr.e << ")");
      }
      T lhs;
      T rhs;
      T e;
      bool result {};
    };
    [[nodiscard]] constexpr auto operator()(T e) const
    {
      return epsilon {lhs, rhs, e};
    }
    T lhs;
    T rhs;
  };

  template <class TLhs, class TRhs>
  struct gt {
    constexpr gt(const TLhs& lhs, const TRhs& rhs)
        : lhs {lhs}
        , rhs {rhs}
    {}
    static_assert(
      type_traits::is_same_v<TLhs, TRhs>, "[ERROR] Comparision of different types is not allowed");
    [[nodiscard]] constexpr explicit operator bool() const
    {
      return false;
    }
    const TLhs& lhs;
    const TLhs& rhs;
  };
  template <class T>
  struct gt<T, T> {
    constexpr gt(const T& lhs, const T& rhs)
        : lhs {lhs}
        , rhs {rhs}
        , result {lhs > rhs}
    {}
    constexpr friend auto operator<<(auto& os, const gt& expr) -> decltype(auto)
    {
      return (os << expr.lhs << " > " << expr.rhs);
    }
    [[nodiscard]] constexpr explicit operator bool() const
    {
      return result;
    }
    T lhs;
    T rhs;
    bool result {};
  };

  template <class TLhs, class TRhs>
  struct ge {
    constexpr ge(const TLhs& lhs, const TRhs& rhs)
        : lhs {lhs}
        , rhs {rhs}
    {}
    static_assert(
      type_traits::is_same_v<TLhs, TRhs>, "[ERROR] Comparision of different types is not allowed");
    [[nodiscard]] constexpr explicit operator bool() const
    {
      return false;
    }
    const TLhs& lhs;
    const TLhs& rhs;
  };
  template <class T>
  struct ge<T, T> {
    constexpr ge(const T& lhs, const T& rhs)
        : lhs {lhs}
        , rhs {rhs}
        , result {lhs >= rhs}
    {}
    constexpr friend auto operator<<(auto& os, const ge& expr) -> decltype(auto)
    {
      return (os << expr.lhs << " >= " << expr.rhs);
    }
    [[nodiscard]] constexpr explicit operator bool() const
    {
      return result;
    }
    T lhs;
    T rhs;
    bool result {};
  };

  template <class TLhs, class TRhs>
  struct lt {
    constexpr lt(const TLhs& lhs, const TRhs& rhs)
        : lhs {lhs}
        , rhs {rhs}
    {}
    static_assert(
      type_traits::is_same_v<TLhs, TRhs>, "[ERROR] Comparision of different types is not allowed");
    [[nodiscard]] constexpr explicit operator bool() const
    {
      return false;
    }
    const TLhs& lhs;
    const TLhs& rhs;
  };
  template <class T>
  struct lt<T, T> {
    constexpr lt(const T& lhs, const T& rhs)
        : lhs {lhs}
        , rhs {rhs}
        , result {lhs < rhs}
    {}
    constexpr friend auto operator<<(auto& os, const lt& expr) -> decltype(auto)
    {
      return (os << expr.lhs << " < " << expr.rhs);
    }
    [[nodiscard]] constexpr explicit operator bool() const
    {
      return result;
    }
    T lhs;
    T rhs;
    bool result {};
  };

  template <class TLhs, class TRhs>
  struct le {
    constexpr le(const TLhs& lhs, const TRhs& rhs)
        : lhs {lhs}
        , rhs {rhs}
    {}
    static_assert(
      type_traits::is_same_v<TLhs, TRhs>, "[ERROR] Comparision of different types is not allowed");
    [[nodiscard]] constexpr explicit operator bool() const
    {
      return false;
    }
    const TLhs& lhs;
    const TLhs& rhs;
  };
  template <class T>
  struct le<T, T> {
    constexpr le(const T& lhs, const T& rhs)
        : lhs {lhs}
        , rhs {rhs}
        , result {lhs <= rhs}
    {}
    constexpr friend auto operator<<(auto& os, const le& expr) -> decltype(auto)
    {
      return (os << expr.lhs << " <= " << expr.rhs);
    }
    [[nodiscard]] constexpr explicit operator bool() const
    {
      return result;
    }
    T lhs;
    T rhs;
    bool result {};
  };

  template <class T>
  struct nt {
    constexpr nt(const T& t)
        : t {t}
        , result {!t}
    {}
    constexpr friend auto operator<<(auto& os, const nt& expr) -> decltype(auto)
    {
      return (os << "!" << expr.t);
    }
    [[nodiscard]] constexpr explicit operator bool() const
    {
      return result;
    }
    T t;
    bool result {};
  };

  namespace detail {
    constexpr decltype(auto) get(const auto& t)
    {
      if constexpr (requires { t.VALUE; }) {
        return t.VALUE;
      } else {
        return t;
      }
    }
  } // namespace detail

  [[nodiscard]] constexpr auto operator==(const auto& lhs, const auto& rhs)
    requires(requires { lhs.VALUE; } || requires { rhs.VALUE; })
  {
    return eq {detail::get(lhs), detail::get(rhs)};
  }
  [[nodiscard]] constexpr auto operator!=(const auto& lhs, const auto& rhs)
    requires(requires { lhs.VALUE; } || requires { rhs.VALUE; })
  {
    return neq {detail::get(lhs), detail::get(rhs)};
  }
  [[nodiscard]] constexpr auto operator>(const auto& lhs, const auto& rhs)
    requires(requires { lhs.VALUE; } || requires { rhs.VALUE; })
  {
    return gt {detail::get(lhs), detail::get(rhs)};
  }
  [[nodiscard]] constexpr auto operator>=(const auto& lhs, const auto& rhs)
    requires(requires { lhs.VALUE; } || requires { rhs.VALUE; })
  {
    return ge {detail::get(lhs), detail::get(rhs)};
  }
  [[nodiscard]] constexpr auto operator<(const auto& lhs, const auto& rhs)
    requires(requires { lhs.VALUE; } || requires { rhs.VALUE; })
  {
    return lt {detail::get(lhs), detail::get(rhs)};
  }
  [[nodiscard]] constexpr auto operator<=(const auto& lhs, const auto& rhs)
    requires(requires { lhs.VALUE; } || requires { rhs.VALUE; })
  {
    return le {detail::get(lhs), detail::get(rhs)};
  }
  template <class T>
  [[nodiscard]] constexpr auto operator!(const T& t)
    requires requires { t.VALUE; }
  {
    return T {!t.VALUE};
  }

  struct _b {
    bool VALUE;
  };
  inline constexpr auto true_b = _b {true};
  inline constexpr auto false_b = _b {false};
  struct _c {
    char VALUE {};
  };
  struct _sc {
    signed char VALUE {};
  };
  struct _s {
    short VALUE {};
    constexpr auto operator-() const
    {
      return _s(-VALUE);
    }
  };
  struct _i {
    int VALUE {};
    constexpr auto operator-() const
    {
      return _i(-VALUE);
    }
  };
  struct _l {
    long VALUE {};
    constexpr auto operator-() const
    {
      return _l(-VALUE);
    }
  };
  struct _ll {
    long long VALUE {};
    constexpr auto operator-() const
    {
      return _ll(-VALUE);
    }
  };
  struct _u {
    unsigned VALUE {};
  };
  struct _uc {
    unsigned char VALUE {};
  };
  struct _us {
    unsigned short VALUE {};
  };
  struct _ul {
    unsigned long VALUE {};
  };
  struct _ull {
    unsigned long long VALUE {};
  };
  struct _f {
    float VALUE {};
    constexpr auto operator-() const
    {
      return _f(-VALUE);
    }
  };
  struct _d {
    double VALUE {};
    constexpr auto operator-() const
    {
      return _d(-VALUE);
    }
  };
  struct _ld {
    long double VALUE {};
    constexpr auto operator-() const
    {
      return _ld(-VALUE);
    }
  };
  struct _i8 {
    __INT8_TYPE__ VALUE {};
    constexpr auto operator-() const
    {
      return _i8(-VALUE);
    }
  };
  struct _i16 {
    __INT16_TYPE__ VALUE {};
    constexpr auto operator-() const
    {
      return _i16(-VALUE);
    }
  };
  struct _i32 {
    __INT32_TYPE__ VALUE {};
    constexpr auto operator-() const
    {
      return _i32(-VALUE);
    }
  };
  struct _i64 {
    __INT64_TYPE__ VALUE {};
    constexpr auto operator-() const
    {
      return _i64(-VALUE);
    }
  };
  struct _u8 {
    __UINT8_TYPE__ VALUE {};
  };
  struct _u16 {
    __UINT16_TYPE__ VALUE {};
  };
  struct _u32 {
    __UINT32_TYPE__ VALUE {};
  };
  struct _u64 {
    __UINT64_TYPE__ VALUE {};
  };
  struct _string {
    struct view {
      [[nodiscard]] constexpr auto operator[](auto i) const
      {
        return data_[i];
      }
      [[nodiscard]] constexpr auto size() const
      {
        return size_;
      }
      constexpr friend auto operator<<(auto& os, const view& v) -> decltype(auto)
      {
        return (os << v.data_);
      }
      [[nodiscard]] constexpr auto operator==(const view& other) const -> bool
      {
        if (size() != other.size()) {
          return false;
        }
        for (decltype(size()) i {}; i < size(); ++i) {
          if ((*this)[i] != other[i]) {
            return false;
          }
        }
        return true;
      }
      const char* data_ {};
      unsigned size_ {};
    } VALUE {};
  };

  [[nodiscard]] constexpr auto operator""_i(unsigned long long int value)
  {
    return _i(value);
  }
  [[nodiscard]] constexpr auto operator""_s(unsigned long long int value)
  {
    return _s(value);
  }
  [[nodiscard]] constexpr auto operator""_c(unsigned long long int value)
  {
    return _c(value);
  }
  [[nodiscard]] constexpr auto operator""_sc(unsigned long long int value)
  {
    return _sc(value);
  }
  [[nodiscard]] constexpr auto operator""_l(unsigned long long int value)
  {
    return _l(value);
  }
  [[nodiscard]] constexpr auto operator""_ll(unsigned long long int value)
  {
    return _ll(value);
  }
  [[nodiscard]] constexpr auto operator""_u(unsigned long long int value)
  {
    return _u(value);
  }
  [[nodiscard]] constexpr auto operator""_uc(unsigned long long int value)
  {
    return _uc(value);
  }
  [[nodiscard]] constexpr auto operator""_us(unsigned long long int value)
  {
    return _us(value);
  }
  [[nodiscard]] constexpr auto operator""_ul(unsigned long long int value)
  {
    return _ul(value);
  }
  [[nodiscard]] constexpr auto operator""_ull(unsigned long long int value)
  {
    return _ull(value);
  }
  [[nodiscard]] constexpr auto operator""_f(long double value)
  {
    return _f(value);
  }
  [[nodiscard]] constexpr auto operator""_d(long double value)
  {
    return _d(value);
  }
  [[nodiscard]] constexpr auto operator""_ld(long double value)
  {
    return _ld(value);
  }
  [[nodiscard]] constexpr auto operator""_i8(unsigned long long int value)
  {
    return _i8(value);
  }
  [[nodiscard]] constexpr auto operator""_i16(unsigned long long int value)
  {
    return _i16(value);
  }
  [[nodiscard]] constexpr auto operator""_i32(unsigned long long int value)
  {
    return _i32(value);
  }
  [[nodiscard]] constexpr auto operator""_i64(unsigned long long int value)
  {
    return _i64(value);
  }
  [[nodiscard]] constexpr auto operator""_u8(unsigned long long int value)
  {
    return _u8(value);
  }
  [[nodiscard]] constexpr auto operator""_u16(unsigned long long int value)
  {
    return _u16(value);
  }
  [[nodiscard]] constexpr auto operator""_u32(unsigned long long int value)
  {
    return _u32(value);
  }
  [[nodiscard]] constexpr auto operator""_u64(unsigned long long int value)
  {
    return _u64(value);
  }
  template <utility::fixed_string Str>
  [[nodiscard]] constexpr auto operator""_s()
  {
    return _string {Str.data(), Str.size() - 1u};
  }
} // namespace ut::inline v2_1_1

using ut::operator""_test;

#if !defined(NTEST)
static_assert((
  [] {
    // ut::type_traits::is_same_v
    {
      static_assert(!ut::type_traits::is_same_v<int, void>);
      static_assert(!ut::type_traits::is_same_v<void, int>);
      static_assert(!ut::type_traits::is_same_v<void*, int>);
      static_assert(!ut::type_traits::is_same_v<int, const int>);
      static_assert(ut::type_traits::is_same_v<void, void>);
      static_assert(ut::type_traits::is_same_v<int, int>);
    }

    // ut::type_traits::is_mutable_lambda_v
    {
      auto l1 = []() {};
      auto l2 = []() constexpr {};
      auto l3 = []() mutable {};
      auto l4 = []() mutable constexpr {};
      static_assert(!ut::type_traits::is_mutable_lambda_v<decltype(&decltype(l1)::operator())>);
      static_assert(!ut::type_traits::is_mutable_lambda_v<decltype(&decltype(l2)::operator())>);
      static_assert(ut::type_traits::is_mutable_lambda_v<decltype(&decltype(l3)::operator())>);
      static_assert(ut::type_traits::is_mutable_lambda_v<decltype(&decltype(l4)::operator())>);
    }

    // ut::type_traits::has_capture_lambda_v
    {
      int i {};
      auto l1 = []() {
        return 42;
      };
      auto l2 = []() constexpr {
        return 42;
      };
      auto l3 = []() mutable {
        return 42;
      };
      auto l4 = [&i]() constexpr {
        return i;
      };
      auto l5 = [i]() mutable {
        return i;
      };
      auto l6 = [=]() mutable constexpr {
        return i;
      };
      static_assert(!ut::type_traits::has_capture_lambda_v<decltype(l1)>);
      static_assert(!ut::type_traits::has_capture_lambda_v<decltype(l2)>);
      static_assert(!ut::type_traits::has_capture_lambda_v<decltype(l3)>);
      static_assert(ut::type_traits::has_capture_lambda_v<decltype(l4)>);
      static_assert(ut::type_traits::has_capture_lambda_v<decltype(l5)>);
      static_assert(ut::type_traits::has_capture_lambda_v<decltype(l6)>);
    }

    {
      static_assert(!ut::type_traits::is_floating_point_v<int>);
      static_assert(!ut::type_traits::is_floating_point_v<bool>);
      static_assert(!ut::type_traits::is_floating_point_v<char>);
      static_assert(!ut::type_traits::is_floating_point_v<void>);
      static_assert(ut::type_traits::is_floating_point_v<float>);
      static_assert(ut::type_traits::is_floating_point_v<double>);
      static_assert(ut::type_traits::is_floating_point_v<long double>);
    }

    // ut::utility::fixed_string
    {
      static_assert(sizeof("") == ut::utility::fixed_string {""}.size());
      static_assert(sizeof("foo") == ut::utility::fixed_string {"foo"}.size());
      static_assert('f' == ut::utility::fixed_string {"foo"}[0]);
      static_assert('o' == ut::utility::fixed_string {"foo"}[1]);
      static_assert('o' == ut::utility::fixed_string {"foo"}[2]);
      static_assert('\0' == ut::utility::fixed_string {"foo"}[3]);
    }

    // ut::detail::get
    {
      static_assert(42 == ut::detail::get(42));
      static_assert(42u == ut::detail::get(ut::_u(42)));
    }

    // ut::eq
    {
      using namespace ut;
      static_assert(eq(42, 42));
      static_assert(!eq(43, 42));
      static_assert(!eq(42, 43));

      static_assert((eq(4.2, 4.2))(.1));
      static_assert((eq(4.24, 4.23))(.01));
      static_assert(!(eq(4.24, 4.23))(.001));

      static_assert(eq("foo"_s, "foo"_s));
      static_assert(!eq("foo"_s, "bar"_s));
      static_assert(!eq(""_s, "foo"_s));
      static_assert(!eq("bar"_s, ""_s));
    }

    // ut::neq
    {
      using namespace ut;
      static_assert(neq(42, 43));
      static_assert(neq(43, 42));
      static_assert(!neq(42, 42));

      static_assert(!(neq(4.2, 4.2))(.1));
      static_assert(!(neq(4.24, 4.23))(.01));
      static_assert((neq(4.24, 4.23))(.001));
    }

    // ut::gt
    {
      using namespace ut;
      static_assert(gt(43, 42));
      static_assert(!gt(42, 43));
      static_assert(!gt(42, 42));
    }

    // ut::ge
    {
      using namespace ut;
      static_assert(ge(43, 42));
      static_assert(ge(43, 43));
      static_assert(!ge(42, 43));
    }

    // ut::lt
    {
      using namespace ut;
      static_assert(lt(42, 43));
      static_assert(!lt(43, 42));
      static_assert(!lt(42, 42));
    }

    // ut::le
    {
      using namespace ut;
      static_assert(le(42, 43));
      static_assert(le(42, 42));
      static_assert(!le(43, 42));
    }

    // ut::nt
    {
      using namespace ut;
      static_assert(nt(false));
      static_assert(!nt(true));
    }

    using sc = signed char;
    using uc = unsigned char;
    using ul = unsigned long;
    using us = unsigned short;
    using ull = unsigned long long;
    using ll = long long;
    using ld = long double;

    // ut::_
    {
      using namespace ut;

      static_assert(_b {true}.VALUE);
      static_assert(!_b {false}.VALUE);
      static_assert(true_b.VALUE);
      static_assert(!false_b.VALUE);
      static_assert(char('0') == _c {'0'}.VALUE);
      static_assert(sc(42) == _sc {42}.VALUE);
      static_assert(short(-42) == -_s {42}.VALUE);
      static_assert(int(-42) == -_i {42}.VALUE);
      static_assert(long(-42) == -_l {42}.VALUE);
      static_assert(ll(-42) == -_ll {42}.VALUE);
      static_assert(unsigned(42) == _u {42}.VALUE);
      static_assert(uc(42) == _uc {42}.VALUE);
      static_assert(us(42) == _us {42}.VALUE);
      static_assert(ul(42) == _ul {42}.VALUE);
      static_assert(ull(42) == _ull {42}.VALUE);
      static_assert(-_f {4.2}.VALUE < float(0));
      static_assert(-_d {4.2}.VALUE < double(0));
      static_assert(-_ld {4.2}.VALUE < ld(0));

      static_assert((__INT8_TYPE__) (-42) == -_i8 {42}.VALUE);
      static_assert((__INT16_TYPE__) (-42) == -_i16 {42}.VALUE);
      static_assert((__INT32_TYPE__) (-42) == -_i32 {42}.VALUE);
      static_assert((__INT64_TYPE__) (-42) == -_i64 {42}.VALUE);
      static_assert((__UINT8_TYPE__) (42) == _u8 {42}.VALUE);
      static_assert((__UINT16_TYPE__) (42) == _u16 {42}.VALUE);
      static_assert((__UINT32_TYPE__) (42) == _u32 {42}.VALUE);
      static_assert((__UINT64_TYPE__) (42) == _u64 {42}.VALUE);

      static_assert(sizeof("foo") == _string::view {"foo", sizeof("foo")}.size());
      static_assert('f' == _string::view {"foo", sizeof("foo")}[0]);
      static_assert('o' == _string::view {"foo", sizeof("foo")}[1]);
      static_assert('o' == _string::view {"foo", sizeof("foo")}[2]);
    }

    // ut::operator
    {
      using namespace ut;
      static_assert(int(-42) == -42_i);
      static_assert(short(-42) == -42_s);
      static_assert(char(0xA) == 0xA_c);
      static_assert(sc(0xA) == 0xA_sc);
      static_assert(long(-42) == -42_l);
      static_assert(ll(-42) == -42_ll);
      static_assert(unsigned(42) == 42_u);
      static_assert(uc(42) == 42_uc);
      static_assert(us(42) == 42_us);
      static_assert(ul(42) == 42_ul);
      static_assert(ull(42) == 42_ull);
      static_assert(-4.2_f < float(0));
      static_assert(-4.2_d < double(0));
      static_assert(-4.2_ld < ld(0));
      static_assert((-4.2f == -4.2_f)(.1));
      static_assert((.1234f == .1234_f)(.0001));
      static_assert((.13f == .12_f)(.1));
      static_assert(!(.13f == .12_f)(.001));
      static_assert((-9.12345678 == -9.12345678_d)(.00001));

      static_assert((__INT8_TYPE__) (-42) == -42_i8);
      static_assert((__INT16_TYPE__) (-42) == -42_i16);
      static_assert((__INT32_TYPE__) (-42) == -42_i32);
      static_assert((__INT64_TYPE__) (-42) == -42_i64);
      static_assert((__UINT8_TYPE__) (42) == 42_u8);
      static_assert((__UINT16_TYPE__) (42) == 42_u16);
      static_assert((__UINT32_TYPE__) (42) == 42_u32);
      static_assert((__UINT64_TYPE__) (42) == 42_u64);

      static_assert(sizeof("") - 1u == ""_s.VALUE.size());
      static_assert(sizeof("foo") - 1u == "foo"_s.VALUE.size());
      static_assert('f' == "foo"_s.VALUE[0]);
      static_assert('o' == "foo"_s.VALUE[1]);
      static_assert('o' == "foo"_s.VALUE[2]);
    }

    // ut::constant
    {
      using namespace ut;
      static_assert(constant < 42 == 42 >);
      static_assert(constant < 42 == 42_i >);
      static_assert(constant < 42_i == 42 >);
    }

    // ut::mut
    {
      static_assert(43 == [] {
        using namespace ut;
        auto i = 42;
        return [=] {
          mut(i) = 43;
          return i;
        }();
      }());
    }
  }(),
  true));
#endif
