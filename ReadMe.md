# NAPI BRIDGE

此项目用于在鸿蒙开发中将c接口封装为一个胶水函数用来提供给arkts使用

- 仅需引入头文件即可实现将c接口导出到arkts
- 支持自定转换函数实现c接口和arkts对象的映射

## 使用示例

### 示例1

使用NAPI_BRIDGE将需要导出的函数转换为一个胶水函数

```
#include "napi_bridge.h"
#include "napi_bridge_common.h"

int add(int a, int b) {
  return a + b;
}

EXTERN_C_START
static napi_value Init(napi_env env, napi_value exports) {
  napi_property_descriptor desc[] = {
      {"add", nullptr, NAPI_BRIDGE(add), nullptr, nullptr, nullptr,
       napi_default, nullptr}};
  napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc);
  return exports;
}
```

### 示例2

导出的函数包含Size结构,此时需要自定义转换函数用来处理arkts对象和c++对象之间的转换

可根据实际情况实现c2j和j2c,一般情况入参需要实现j2c,返回值需要实现c2j

```
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

int area(Size s) {
  return s.width * s.height;
}

static napi_value Init(napi_env env, napi_value exports) {
  napi_property_descriptor desc[] = {
      {"area", nullptr, NAPI_BRIDGE(area), nullptr, nullptr, nullptr,
       napi_default, nullptr}};
  napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc);
  return exports;
}
```

**完整示例参考demo文件夹的鸿蒙工程**
