#pragma once

#include <string>
#include <deque>

#include "logging.hpp"
#include "loader.hpp"

enum class TokenKind {
    COMMAND,
    TERMINATOR,
    TEXT,
};

struct Token {
    TokenKind kind;
    size_t line;
    size_t column;
    size_t length;
    std::string value;

    Token(
        TokenKind kind, size_t line, size_t column,
        size_t length = 0, const std::string &value = ""
    ): kind(kind), line(line), column(column){}
};

std::deque<Token> tokenize(const SourceFile &code, Logger logger);
