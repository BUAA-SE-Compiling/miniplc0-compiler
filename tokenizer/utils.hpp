#pragma once

#include <cctype>

// 我真是爱死 C++ 了.jpg
// See https://en.cppreference.com/w/cpp/string/byte/isspace#Notes
#define IS_FUNC(f)                                                          \
  inline bool f(char ch) { return std::f(static_cast<unsigned char>(ch)); } \
  using __let_this_macro_end_with_a_simicolon_##f = int

namespace miniplc0 {
IS_FUNC(isprint);
IS_FUNC(isspace);
IS_FUNC(isblank);
IS_FUNC(isalpha);
IS_FUNC(isupper);
IS_FUNC(islower);
IS_FUNC(isdigit);
}  // namespace miniplc0
