#pragma once

#include <cstdint>
#include <iostream>
#include <string>
#include <utility>

namespace miniplc0 {

inline void DieAndPrint(std::string condition) {
  std::cout << "Exception: " << condition << "\n";
  std::cout << "The program should not reach here.\n";
  std::cout << "Please check your program carefully.\n";
  std::cout
      << "If you believe it's not your fault, please report this to TAs.\n";
  std::abort();
  // or *((int*)114514) = 19260817;
}

// To keep it simple, we don't create an error system.
enum ErrorCode {
  ErrNoError,  // Should be only used internally.
  ErrStreamError,
  ErrEOF,
  ErrInvalidInput,
  ErrInvalidIdentifier,
  ErrIntegerOverflow,  // int32_t overflow.
  ErrNoBegin,
  ErrNoEnd,
  ErrNeedIdentifier,
  ErrConstantNeedValue,
  ErrNoSemicolon,
  ErrInvalidVariableDeclaration,
  ErrIncompleteExpression,
  ErrNotDeclared,
  ErrAssignToConstant,
  ErrDuplicateDeclaration,
  ErrNotInitialized,
  ErrInvalidAssignment,
  ErrInvalidPrint
};

class CompilationError final {
 private:
  using uint64_t = std::uint64_t;

 public:
  friend void swap(CompilationError &lhs, CompilationError &rhs);

  CompilationError(uint64_t line, uint64_t column, ErrorCode err)
      : _pos(line, column), _err(err) {}
  CompilationError(std::pair<uint64_t, uint64_t> pos, ErrorCode err)
      : CompilationError(pos.first, pos.second, err) {}
  CompilationError(const CompilationError &ce) {
    _pos = ce._pos;
    _err = ce._err;
  }
  CompilationError(CompilationError &&ce)
      : CompilationError(0, 0, ErrorCode::ErrNoError) {
    swap(*this, ce);
  }
  CompilationError &operator=(CompilationError ce) {
    swap(*this, ce);
    return *this;
  }
  bool operator==(const CompilationError &rhs) const {
    return _pos == rhs._pos && _err == rhs._err;
  }

  std::pair<uint64_t, uint64_t> GetPos() const { return _pos; }
  ErrorCode GetCode() const { return _err; }

 private:
  std::pair<uint64_t, uint64_t> _pos;
  ErrorCode _err;
};

inline void swap(CompilationError &lhs, CompilationError &rhs) {
  using std::swap;
  swap(lhs._pos, rhs._pos);
  swap(lhs._err, rhs._err);
}
}  // namespace miniplc0
