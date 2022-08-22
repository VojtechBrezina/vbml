#include "lexer.hpp"

#include <map>

void writeUnexpectedTokenError(
    Logger &logger, const std::string &tag,
    const std::string &expected,
    const SourceFile &code, const SourcePos &pos, size_t length
){
    logger.startMessage(tag, LogLevel::ERROR);
    logger.write(code.getName());
    logger.put(':');
    logger.write(pos.toString());
    logger.write(": Expected ");
    logger.write(TextColor::STRING, expected);
    logger.write(" got ");
    logger.write(TextColor::STRING, code[pos.line].substr(pos.column, length));
    logger.write(": ");
    logger.setColor(TextColor::STRING);
    logger.writeHighlight(
        code[pos.line].substr(0, code[pos.line].length() - 1), TextColor::EROR,
        pos.column, pos.column + length
    );
    logger.endMessage();
}

std::deque<Token> tokenize(const SourceFile &code, Logger logger){
    const std::string tag = "[lexer]";
    std::deque<Token> tokens;

    enum class State { TEXT, LEFT, RIGHT, COMMAND };
    State state = State::TEXT;

    tokens.emplace_back(TokenKind::TEXT, SourcePos { 0, 0 });

    for(SourcePos pos; pos < code.endPos(); pos.moveForward(1, code)){
        switch(state){

            case State::TEXT:
                switch(code[pos]){
                    case '[': state = State::LEFT; break;
                    case ']': state = State::RIGHT; break;
                    default:
                        tokens.back().value.push_back(code[pos]);
                        break;
                }
                break;

            case State::LEFT:
                switch(code[pos]){
                    case '[':
                        tokens.back().value.push_back('[');
                        state = State::TEXT;
                        break;
                    case ']':
                        tokens.back().setEnd(pos.moved(-1, code), code);
                        tokens.emplace_back(
                            TokenKind::TERMINATOR, pos.moved(-1, code), 2
                        );
                        tokens.emplace_back(
                            TokenKind::TEXT, pos.moved(1, code)
                        );
                        state = State::TEXT;
                        break;
                    default:
                        tokens.back().setEnd(pos.moved(-1, code), code);
                        tokens.emplace_back(
                            TokenKind::COMMAND, pos.moved(-1, code)
                        );
                        if(!isspace(code[pos])){
                            if(
                                isalnum(code[pos]) ||
                                code[pos] == '.' ||
                                code[pos] == '-'
                            ){
                                tokens.back().value.push_back(code[pos]);
                            } else {
                                writeUnexpectedTokenError(
                                    logger, tag,
                                    "space, '-', or '.'", code, pos, 1
                                );
                            }
                        }
                        state = State::COMMAND;
                }
                break;

            case State::RIGHT:
                switch(code[pos]){
                    case ']':
                        tokens.back().value.push_back(']');
                        break;
                    default:
                        tokens.back().value.push_back(code[pos]);
                        writeUnexpectedTokenError(
                            logger, tag,
                            "[", code, pos, 1
                        );
                        state = State::TEXT;
                        break;
                }
                break;

            case State::COMMAND:
                switch(code[pos]){
                    case ']':
                        tokens.back().setEnd(pos.moved(1, code), code);
                        tokens.emplace_back(
                            TokenKind::TEXT, pos.moved(1, code)
                        );
                        state = State::TEXT;
                        break;
                    default:
                        if(!isspace(code[pos])){
                            if(
                                isalnum(code[pos]) ||
                                code[pos] == '.' ||
                                code[pos] == '-'
                            ){
                                tokens.back().value.push_back(code[pos]);
                            } else {
                                writeUnexpectedTokenError(
                                    logger, tag,
                                    "space, '-', or '.'", code, pos, 1
                                );
                            }
                        }
                        break;
                }
                break;
        }
    }

    tokens.back().setEnd(code.endPos(), code);

    switch(state){
        case State::TEXT: break; // The only allowed one.
        case State::COMMAND:
            logger.startMessage(tag, LogLevel::ERROR);
            logger.write(code.getName());
            logger.write(": ");
            logger.write("Incomplete command at the end of the file.");
            logger.endMessage();
            break;
        case State::LEFT:
            logger.startMessage(tag, LogLevel::ERROR);
            logger.write(code.getName());
            logger.write(": ");
            logger.write(
                "Incomplete command or terminator at the end of the file."
            );
            logger.endMessage();
            break;
        case State::RIGHT:
            logger.startMessage(tag, LogLevel::ERROR);
            logger.write(code.getName());
            logger.write(": ");
            logger.write("Unmatched ']' at the end of the file.");
            logger.endMessage();
            break;
    }

    std::deque<Token>::iterator iter = tokens.begin();
    while(iter != tokens.end()){
        if(iter->kind == TokenKind::TEXT && iter->value.empty()){
            ++iter;
            tokens.erase(std::prev(iter));
        } else {
            ++iter;
        }
    }

    return tokens;
}

void logTokens(Logger &logger, const std::deque<Token> &tokens){
    static const std::map<TokenKind, const std::string> tokenNames = {
        {TokenKind::COMMAND, "Command"},
        {TokenKind::TERMINATOR, "Terminator"},
        {TokenKind::TEXT, "Text"}
    };
    static const std::string tag = "[lexer]";

    logger.quickMessage(tag, LogLevel::DEBUG, "Logging tokens...");

    for(const Token &token : tokens){
        logger.startMessage("[lexer]", LogLevel::DEBUG);
        logger.write(TextColor::NAME, tokenNames.at(token.kind));
        logger.put('(');
        logger.write(token.start.toString());
        logger.write(" - ");
        logger.write(std::to_string(token.length));
        logger.put(')');
        if(token.kind != TokenKind::TERMINATOR){
            logger.write(": ");
            logger.setColor(TextColor::STRING);
            logger.put('"');
            for(char c : token.value){
                switch(c){
                    case '\n':
                        logger.write(TextColor::PUNCT, "\\n");
                        break;
                    default:
                        logger.put(c);
                        break;
                }
            }
            logger.put('"');
            logger.setColor(TextColor::NONE);
        }
        logger.endMessage();
    }
}
