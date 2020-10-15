#pragma once

#include <any>
#include <cstdint>
#include <string>

#include "error/error.h"

namespace miniplc0 {

enum TokenType {
  NULL_TOKEN,
  UNSIGNED_INTEGER,
  IDENTIFIER,
  BEGIN,
  END,
  VAR,
  CONST,
  PRINT,
  PLUS_SIGN,
  MINUS_SIGN,
  MULTIPLICATION_SIGN,
  DIVISION_SIGN,
  EQUAL_SIGN,
  SEMICOLON,
  LEFT_BRACKET,
  RIGHT_BRACKET
};

class Token final {
 private:
  using uint64_t = std::uint64_t;
  using int32_t = std::int32_t;

 public:
  friend void swap(Token &lhs, Token &rhs);

 public:
  Token(TokenType type, std::any value, uint64_t start_line,
        uint64_t start_column, uint64_t end_line, uint64_t end_column)
      : _type(type),
        _value(std::move(value)),
        _start_pos(start_line, start_column),
        _end_pos(end_line, end_column) {}
  Token(TokenType type, std::any value, std::pair<uint64_t, uint64_t> start,
        std::pair<uint64_t, uint64_t> end)
      : Token(type, value, start.first, start.second, end.first, end.second) {}
  Token(const Token &t) {
    _type = t._type;
    _value = t._value;
    _start_pos = t._start_pos;
    _end_pos = t._end_pos;
  }
  Token(Token &&t) : Token(TokenType::NULL_TOKEN, nullptr, 0, 0, 0, 0) {
    swap(*this, t);
  }
  Token &operator=(Token t) {
    swap(*this, t);
    return *this;
  }
  bool operator==(const Token &rhs) const {
    return _type == rhs._type && GetValueString() == rhs.GetValueString() &&
           _start_pos == rhs._start_pos && _end_pos == rhs._end_pos;
  }

  TokenType GetType() const { return _type; };
  std::any GetValue() const { return _value; };
  std::pair<uint64_t, uint64_t> GetStartPos() const { return _start_pos; }
  std::pair<uint64_t, uint64_t> GetEndPos() const { return _end_pos; }
  std::string GetValueString() const {
    try {
      return std::any_cast<std::string>(_value);
    } catch (const std::bad_any_cast &) {
    }
    try {
      return std::string(1, std::any_cast<char>(_value));
    } catch (const std::bad_any_cast &) {
    }
    try {
      return std::to_string(std::any_cast<int32_t>(_value));
    } catch (const std::bad_any_cast &) {
      DieAndPrint("No suitable cast for token value.");
    }
    return "Invalid";
  }

 private:
  TokenType _type;
  std::any _value;
  std::pair<uint64_t, uint64_t> _start_pos;
  std::pair<uint64_t, uint64_t> _end_pos;
};

inline void swap(Token &lhs, Token &rhs) {
  using std::swap;
  swap(lhs._type, rhs._type);
  swap(lhs._value, rhs._value);
  swap(lhs._start_pos, rhs._start_pos);
  swap(lhs._end_pos, rhs._end_pos);
}
}  // namespace miniplc0
