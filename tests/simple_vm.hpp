#pragma once

#include <array>
#include <climits>
#include <cstdint>
#include <exception>
#include <string>
#include <vector>

#include "instruction/instruction.h"

namespace miniplc0 {
// This is a simplified version of miniplc0 vm implementation for testing.
class VM {
 private:
  using uint64_t = std::uint64_t;
  using int32_t = std::int32_t;
  using int64_t = std::int64_t;

 public:
  VM(std::vector<Instruction> v)
      : _codes(std::move(v)), _stack(2048, 0), _ip(0), _sp(0) {}
  VM(const VM &) = delete;
  VM(VM &&) = delete;
  VM &operator=(VM) = delete;

  // If it crashes, let it crash.
  std::vector<int32_t> Run() {
    std::vector<int32_t> v;
    for (auto &it : _codes) {
      auto x = it.GetX();
      switch (it.GetOperation()) {
        case Operation::ILL:
          throw std::out_of_range("ILL");
          break;
        case Operation::LIT:
          _stack[_sp] = x;
          _sp++;
          break;
        case Operation::LOD:
          _stack[_sp] = _stack[x];
          _sp++;
          break;
        case Operation::STO:
          _stack[x] = _stack[_sp - 1];
          _sp--;
          break;
        case Operation::ADD:
          _stack[_sp - 2] = add(_stack[_sp - 2], _stack[_sp - 1]);
          _sp--;
          break;
        case Operation::SUB:
          _stack[_sp - 2] = sub(_stack[_sp - 2], _stack[_sp - 1]);
          _sp--;
          break;
        case Operation::DIV:
          _stack[_sp - 2] = div(_stack[_sp - 2], _stack[_sp - 1]);
          _sp--;
          break;
        case Operation::MUL:
          _stack[_sp - 2] = mul(_stack[_sp - 2], _stack[_sp - 1]);
          _sp--;
          break;
        case Operation::WRT:
          v.emplace_back(_stack[_sp - 1]);
          _sp--;
          break;
      }
    }
    return v;
  }

 private:
  int32_t add(int32_t lhs, int32_t rhs) {
    int64_t r = (int64_t)lhs + (int64_t)rhs;
    if (r < INT_MIN || r > INT_MAX)
      throw std::out_of_range("addition out of range");
    return lhs + rhs;
  }

  int32_t sub(int32_t lhs, int32_t rhs) {
    int64_t r = (int64_t)lhs - (int64_t)rhs;
    if (r < INT_MIN || r > INT_MAX)
      throw std::out_of_range("subtraction out of range");
    return lhs - rhs;
  }

  // See CSAPP Chapter.2
  int32_t mul(int32_t lhs, int32_t rhs) {
    int32_t r = lhs * rhs;
    if (!lhs || r / lhs == rhs)
      return r;
    else
      throw std::out_of_range("multiplication out of range");
  }

  int32_t div(int32_t lhs, int32_t rhs) {
    if (rhs == 0) throw std::out_of_range("divide by zero");
    if (rhs == -1 && lhs == INT_MIN)
      throw std::out_of_range("INT_MIN/-1");
    else
      return lhs / rhs;
  }

 private:
  std::vector<Instruction> _codes;
  std::vector<int32_t> _stack;
  uint64_t _ip;
  uint64_t _sp;
};
}  // namespace miniplc0
