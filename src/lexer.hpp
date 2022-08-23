#pragma once

#include <string>
#include <deque>
#include <map>

#include "logging.hpp"
#include "loader.hpp"

enum class TokenKind {
    COMMAND,
    TERMINATOR,
    TEXT,
};

const std::map<TokenKind, const std::string> tokenKindNames = {
    {TokenKind::COMMAND, "Command"},
    {TokenKind::TERMINATOR, "Terminator"},
    {TokenKind::TEXT, "Text"}
};

/// A token needs to know its kind, any optional value and the characters form
/// the source it spans across.
struct Token {
    TokenKind kind;
    SourcePos start;
    size_t length;
    std::string value;

    Token(
        TokenKind kind, SourcePos start,
        size_t length = 0, const std::string &value = ""
    ): kind(kind), start(start), length(length), value(value){}

    /// Just a convenience - there is no end property and this is a struct
    /// anyway.
    void setEnd(const SourcePos &end, const SourceFile &code){
        length = start.distanceTo(end, code);
    }
};

std::deque<Token> tokenize(const SourceFile &code, Logger logger);

void logTokens(Logger &logger, const std::deque<Token> &tokens);

