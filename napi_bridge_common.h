#ifndef NAPI_BRIDGE_COMMON_H
#define NAPI_BRIDGE_COMMON_H

#include <string>
#include "napi_bridge.h"

// js to c
template <>
int j2c<int>(napi_env env, napi_value jval) {
  int32_t result = 0;
  napi_get_value_int32(env, jval, &result);
  return result;
}

template <>
float j2c<float>(napi_env env, napi_value jval) {
  double result = 0;
  napi_get_value_double(env, jval, &result);
  return result;
}

template <>
double j2c<double>(napi_env env, napi_value jval) {
  double result = 0;
  napi_get_value_double(env, jval, &result);
  return result;
}

template <>
bool j2c<bool>(napi_env env, napi_value jval) {
  double result = 0;
  napi_get_value_double(env, jval, &result);
  return result;
}

template <>
std::string j2c<std::string>(napi_env env, napi_value jval) {
  std::string result;
  size_t len;
  napi_get_value_string_utf8(env, jval, nullptr, 0, &len);
  result.resize(len - 1);
  napi_get_value_string_utf8(env, jval, (char*)result.c_str(), len, &len);
  return result;
}

// c to js
template <>
auto c2j(napi_env env, int cval) {
  napi_value val;
  napi_create_int32(env, cval, &val);
  return val;
}

template <>
auto c2j(napi_env env, float cval) {
  napi_value val;
  napi_create_double(env, cval, &val);
  return val;
}

template <>
auto c2j(napi_env env, double cval) {
  napi_value val;
  napi_create_double(env, cval, &val);
  return val;
}

template <>
auto c2j(napi_env env, bool cval) {
  napi_value val;
  napi_create_int32(env, cval, &val);
  return val;
}

template <>
auto c2j(napi_env env, std::string cval) {
  napi_value val;
  napi_create_string_utf8(env, cval.c_str(), cval.size(), &val);
  return val;
}

template <>
auto c2j(napi_env env, const char* cval) {
  napi_value val;
  napi_create_string_utf8(env, cval, strlen(cval), &val);
  return val;
}

#endif
