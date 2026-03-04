#pragma once
#include <utility>

namespace utils {
template <typename> class function;

template <typename R, typename... Args> class function<R(Args...)> {

private:
  struct callable_base {
    virtual R invoke(Args... args) = 0;
    virtual callable_base *clone() const = 0;
    virtual ~callable_base() = default;
  };

  template <typename Callable> struct callable_impl : callable_base {
    Callable m_callable_;

    template <typename T>
    callable_impl(T &&c) : m_callable_(std::forward<T>(c)) {}

    R invoke(Args... args) override {
      return m_callable_(std::forward<Args>(args)...);
    }

    callable_base *clone() const override {
      return new callable_impl(m_callable_);
    }
  };

  callable_base *m_callable = nullptr;

public:
  function() noexcept = default;

  template
};
} // namespace utils