#ifndef NAPI_BRIDGE_H
#define NAPI_BRIDGE_H

#include "napi/native_api.h"

template <typename T>
T j2c(napi_env env, napi_value jval);

template <typename T>
auto c2j(napi_env env, T cval);

template <typename... Args>
struct Apply;

template <>
struct Apply<> {
  template <typename F, typename... Args>
  static auto fun(F f, napi_env env, napi_value* napi_args, Args... args) {
    return f(args...);
  }
};

template <typename Head, typename... Tail>
struct Apply<Head, Tail...> {
  template <typename F, typename... Args>
  static auto fun(F f, napi_env env, napi_value* napi_args, Args... args) {
    return Apply<Tail...>::template fun(f, env, napi_args + 1, args...,
                                        j2c<Head>(env, *napi_args));
  }
};

template <typename F>
struct Wrap;

template <typename Ret, typename... Args>
struct Wrap<Ret (*)(Args...)> {
  template <Ret f(Args...)>
  static napi_value fun(napi_env env, napi_callback_info info) {
    size_t argc = sizeof...(Args);
    napi_value args[sizeof...(Args)] = {};
    napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    return c2j(env, Apply<Args...>::fun(f, env, args));
  }
};

template <typename... Args>
struct Wrap<void (*)(Args...)> {
  template <void f(Args...)>
  static napi_value fun(napi_env env, napi_callback_info info) {
    size_t argc = sizeof...(Args);
    napi_value args[sizeof...(Args)] = {};
    napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    Apply<Args...>::template fun(f, env, args);
    return nullptr;
  }
};

template <typename F, F f>
auto wrap() {
  return Wrap<F>::template fun<f>;
}

#define NAPI_BRIDGE(f) wrap<decltype(&f), f>()
#define NAPI_J(v) c2j(env, v)

#endif
