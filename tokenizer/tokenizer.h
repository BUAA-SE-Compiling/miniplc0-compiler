#pragma once

#include <cstdint>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "error/error.h"
#include "tokenizer/token.h"
#include "tokenizer/utils.hpp"

namespace miniplc0 {

class Tokenizer final {
 private:
  using uint64_t = std::uint64_t;

  // 状态机的所有状态
  enum DFAState {
    INITIAL_STATE,
    UNSIGNED_INTEGER_STATE,
    PLUS_SIGN_STATE,
    MINUS_SIGN_STATE,
    DIVISION_SIGN_STATE,
    MULTIPLICATION_SIGN_STATE,
    IDENTIFIER_STATE,
    EQUAL_SIGN_STATE,
    SEMICOLON_STATE,
    LEFTBRACKET_STATE,
    RIGHTBRACKET_STATE
  };

 public:
  Tokenizer(std::istream &ifs)
      : _rdr(ifs), _initialized(false), _ptr(0, 0), _lines_buffer() {}
  Tokenizer(Tokenizer &&tkz) = delete;
  Tokenizer(const Tokenizer &) = delete;
  Tokenizer &operator=(const Tokenizer &) = delete;

  // 核心函数，返回下一个 token
  std::pair<std::optional<Token>, std::optional<CompilationError>> NextToken();
  // 一次返回所有 token
  std::pair<std::vector<Token>, std::optional<CompilationError>> AllTokens();

 private:
  // 检查 Token 的合法性
  std::optional<CompilationError> checkToken(const Token &);
  //
  // ** 你需要完成这个函数 **
  //
  // 返回下一个 token，是 NextToken 实际实现部分
  std::pair<std::optional<Token>, std::optional<CompilationError>> nextToken();

  // 从这里开始其实是一个基于行号的缓冲区的实现
  // 为了简单起见，我们没有单独拿出一个类实现
  // 核心思想和 C 的文件输入输出类似，就是一个 buffer 加一个指针，有三个细节
  // 1.缓冲区包括 \n
  // 2.指针始终指向下一个要读取的 char
  // 3.行号和列号从 0 开始

  // 一次读入全部内容，并且替换所有换行为 \n
  // 这样其实是不合理的，这里只是简单起见这么实现
  void readAll();
  // 一个简单的总结
  // | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9  | 偏移
  // | = | = | = | = | = | = | = | = | = | =  |
  // | h | a | 1 | 9 | 2 | 6 | 0 | 8 | 1 | \n |（缓冲区第0行）
  // | 7 | 1 | 1 | 4 | 5 | 1 | 4 |             （缓冲区第1行）
  // 这里假设指针指向第一行的 \n，那么有
  // nextPos() = (1, 0)
  // currentPos() = (0, 9)
  // previousPos() = (0, 8)
  // nextChar() = '\n' 并且指针移动到 (1, 0)
  // unreadLast() 指针移动到 (0, 8)
  std::pair<uint64_t, uint64_t> nextPos();
  std::pair<uint64_t, uint64_t> currentPos();
  std::pair<uint64_t, uint64_t> previousPos();
  std::optional<char> nextChar();
  bool isEOF();
  void unreadLast();

 private:
  std::istream &_rdr;
  // 如果没有初始化，那么就 readAll
  bool _initialized;
  // 指向下一个要读取的字符
  std::pair<uint64_t, uint64_t> _ptr;
  // 以行为基础的缓冲区
  std::vector<std::string> _lines_buffer;
};
}  // namespace miniplc0
