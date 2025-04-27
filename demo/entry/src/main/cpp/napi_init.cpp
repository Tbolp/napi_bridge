#include "napi/native_api.h"
#include "napi_bridge.h"
#include "napi_bridge_common.h"

struct Size {
  int width;
  int height;
};

template <>
auto c2j(napi_env env, Size cval) {
  napi_value val;
  napi_create_object(env, &val);
  napi_set_property(env, val, NAPI_J("width"), NAPI_J(cval.width));
  napi_set_property(env, val, NAPI_J("height"), NAPI_J(cval.height));
  return val;
}

template <>
Size j2c<Size>(napi_env env, napi_value jval) {
  Size result;
  napi_value val;
  napi_get_property(env, jval, NAPI_J("width"), &val);
  result.width = j2c<int>(env, val);
  napi_get_property(env, jval, NAPI_J("height"), &val);
  result.height = j2c<int>(env, val);
  return result;
}

int add(int a, int b) {
  return a + b;
}

int area(Size s) {
  return s.width * s.height;
}

Size square(int w) {
  return {.width = w, .height = w};
}

std::string str_add(std::string a, int b) {
  return a + std::to_string(b);
}

EXTERN_C_START
static napi_value Init(napi_env env, napi_value exports) {
  napi_property_descriptor desc[] = {
      {"add", nullptr, NAPI_BRIDGE(add), nullptr, nullptr, nullptr,
       napi_default, nullptr},
      {"area", nullptr, NAPI_BRIDGE(area), nullptr, nullptr, nullptr,
       napi_default, nullptr},
      {"square", nullptr, NAPI_BRIDGE(square), nullptr, nullptr, nullptr,
       napi_default, nullptr},
      {"str_add", nullptr, NAPI_BRIDGE(str_add), nullptr, nullptr, nullptr,
       napi_default, nullptr}};
  napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc);
  return exports;
}
EXTERN_C_END

static napi_module demoModule = {
    .nm_version = 1,
    .nm_flags = 0,
    .nm_filename = nullptr,
    .nm_register_func = Init,
    .nm_modname = "entry",
    .nm_priv = ((void*)0),
    .reserved = {0},
};

extern "C" __attribute__((constructor)) void RegisterEntryModule(void) {
  napi_module_register(&demoModule);
}
