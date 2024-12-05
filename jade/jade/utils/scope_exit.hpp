#pragma once

#include <algorithm>

namespace jade {

  /**
   * @brief A helper class that guarantees a function is executed when the scope exits.
   *
   * This class takes a callable object (a function, lambda, etc.) as a parameter and ensures
   * that it is executed when the object goes out of scope. This is useful for performing
   * cleanup operations or ensuring that resources are released at the end of a scope.
   *
   * @tparam FunctionType The type of the callable object.
   */
  template<typename FunctionType>
  class ScopeExit
  {
  public:
    explicit ScopeExit(FunctionType&& function)
      : function(std::forward<FunctionType>(function))
    {
    }

    ~ScopeExit()
    {
      function();
    }

    ScopeExit(const ScopeExit&) = delete;
    ScopeExit& operator=(const ScopeExit&) = delete;

    ScopeExit(ScopeExit&&) = delete;
    ScopeExit& operator=(ScopeExit&&) = delete;

  private:
    FunctionType function;
  };

} // namespace jade
