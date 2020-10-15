#pragma once

#include <cstdint>
#include <utility>

namespace miniplc0 {

enum Operation { ILL = 0, LIT, LOD, STO, ADD, SUB, MUL, DIV, WRT };

class Instruction final {
 private:
  using int32_t = std::int32_t;

 public:
  friend void swap(Instruction &lhs, Instruction &rhs);

 public:
  Instruction(Operation opr, int32_t x) : _opr(opr), _x(x) {}

  Instruction() : Instruction(Operation::ILL, 0) {}
  Instruction(const Instruction &i) {
    _opr = i._opr;
    _x = i._x;
  }
  Instruction(Instruction &&i) : Instruction() { swap(*this, i); }
  Instruction &operator=(Instruction i) {
    swap(*this, i);
    return *this;
  }
  bool operator==(const Instruction &i) const {
    return _opr == i._opr && _x == i._x;
  }

  Operation GetOperation() const { return _opr; }
  int32_t GetX() const { return _x; }

 private:
  Operation _opr;
  int32_t _x;
};

inline void swap(Instruction &lhs, Instruction &rhs) {
  using std::swap;
  swap(lhs._opr, rhs._opr);
  swap(lhs._x, rhs._x);
}
}  // namespace miniplc0
