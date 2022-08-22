#include "loader.hpp"

#include <tuple>

std::strong_ordering SourcePos::operator <=>(const SourcePos &other) const {
    return std::tie(line, column) <=> std::tie(other.line, other.column);
}

SourcePos SourcePos::moved( ssize_t chars, const SourceFile &code) const {
    Pos result(*this);

    if(chars >= 0)
        result.moveForward(chars, code);
    else
        result.moveBack(-chars, code);

    return result;
}

ssize_t SourcePos::distanceTo(
    const SourcePos &other, const SourceFile &code
) const {
    Pos first, second;
    bool negative = *this > other;
    size_t result = 0;

    if(negative){
        first = other;
        second = *this;
    } else {
        first = *this;
        second = other;
    }

    if(first.line == second.line){
        result = second.column - first.column;
        return negative ? -result : result;
    }

    if(first.column != 0){
        result += code[first.line].length() - first.column;
        first.column = 0;
        first.line++;
    }

    while(first.line != second.line){
        result += code[first.line].length();
        first.line++;
    }

    result += second.column;
    return negative ? -result : result;
}

void SourcePos::moveForward(size_t chars, const SourceFile &code){
    if(column + chars < code[line].length()){
        column += chars;
        return;
    }

    if(column != 0){
        chars -= code[line].length() - column;
        column = 0;
        line++;
    }

    while(chars >= code[line].length()){
        chars -= code[line].length();
        line++;
    }

    column += chars;
}

void SourcePos::moveBack(size_t chars, const SourceFile &code){
    if(column >= chars){
        column -= chars;
        return;
    }

    chars -= column;
    column = 0;

    while(chars > code[line - 1].length()){
        chars -= code[line - 1].length();
        line--;
    }

    if(chars != 0){
        line--;
        column = code[line].length() - chars;
    }
}

std::string SourcePos::toString() const {
    return std::to_string(line + 1) + ":" + std::to_string(column + 1);
}


SourceFile::SourceFile(
    std::istream &input, const std::string &name
): name(name){
    for(std::string line; std::getline(input, line); ){
        // Needed for the parser plugins to do e.g. paragraphs.
        line.push_back('\n');
        lines.push_back(line);
    }
}
