#include "lexer.hpp"

std::deque<Token> tokenize(const SourceFile &code, Logger logger){
    const std::string tag = "[lexer]";
    std::deque<Token> tokens;

    enum class State { TEXT, LEFT, RIGHT, COMMAND };
    State state = State::TEXT;

    tokens.emplace_back(TokenKind::TEXT, 0, 0);

    for(size_t line = 0; line < code.lineCount(); ++line){
        for(size_t column = 0; column < code[line].length(); ++column){
            switch(state){

                case State::TEXT:
                    switch(code[line][column]){
                        case '[': state = State::LEFT; break;
                        case ']': state = State::RIGHT; break;
                        default:
                            tokens.back().value.push_back(code[line][column]);
                            break;
                    }
                    break;

                case State::LEFT:
                    switch(code[line][column]){
                        case '[':
                            tokens.back().value.push_back('[');
                            state = State::TEXT;
                            break;
                        case ']':
                            tokens.emplace_back(
                                TokenKind::TERMINATOR, line, column, 2
                            );
                            tokens.emplace_back(TokenKind::TEXT, line, column + 1);
                            break;
                    }
                    break;

                case State::RIGHT:

                    break;

                case State::COMMAND:

                    break;
            }
        }
        tokens.back().value.push_back('\n');
    }

    return tokens;
}
