#include "logging.hpp"
#include <iostream>
#include <unordered_map>

#include <unistd.h>


// Constants for Logger

const std::unordered_map<LogLevel, TextColor> levelColors {
    { LogLevel::ERROR, TextColor::EROR },
    { LogLevel::WARNING, TextColor::WARNING },
    { LogLevel::INFO, TextColor::INFO },
    { LogLevel::DEBUG, TextColor::DEBUG },
};

const std::unordered_map<LogLevel, char> levelSigns {
    { LogLevel::ERROR, 'E' },
    { LogLevel::WARNING, 'W' },
    { LogLevel::INFO, 'I' },
    { LogLevel::DEBUG, 'D' },
};


// Private Logger API

void Logger::detectTerminal(){
    useColor = (
        (stream == &std::cout && isatty(STDOUT_FILENO)) ||
        (stream == &std::cerr && isatty(STDERR_FILENO)) ||
        (stream == &std::clog && isatty(STDERR_FILENO))
    );
}


void Logger::putColor(TextColor c){
    *stream << "\033[" << (int)c << "m";
}


void Logger::writeMessageHeader(){
    if(messageLogLevel > maxLevel)
        return;

    if(useColor){
        putColor(levelColors.at(messageLogLevel));
        *stream << messageTag;
        putColor(TextColor::NONE);
    } else {
        *stream << levelSigns.at(messageLogLevel) << messageTag;
    }

    *stream << ": ";
}


// Public Logger API

void Logger::enableColor(bool v){
#   ifndef NDEBUG
        if(insideMessage)
            throw std::logic_error(
                "Cannot change logger settings while inside a message."
            );
#   endif

    useColor = v;
}


void Logger::changeLogLevel(LogLevel v){
#   ifndef NDEBUG
        if(insideMessage)
            throw std::logic_error(
                "Cannot change logger settings while inside a message."
            );
#   endif

    maxLevel = v;
}


void Logger::startMessage(const std::string &tag, LogLevel level){
#   ifndef NDEBUG
        if(insideMessage)
            throw std::logic_error(
                "Cannot start a message before ending this one."
            );
#   endif

    insideMessage = true;
    messageTag = tag;
    messageLogLevel = level;
    messageColor = TextColor::NONE;
    highlightStart = highlightEnd = std::string::npos;
    messageLength = 0;
    writeMessageHeader();
}


void Logger::endMessage(){
#   ifndef NDEBUG
        if(!insideMessage)
            throw std::logic_error("There is no message to end.");
#   endif

    if(useColor)
        setColor(TextColor::NONE);

    put('\n');

    if(useColor){
        putColor(TextColor::NONE);
    } else if(highlightStart != std::string::npos){
        writeMessageHeader();
        for(size_t pos = 0; pos < highlightStart; ++pos)
            put(' ');
        for(size_t pos = highlightStart; pos < highlightEnd; ++pos)
            put('^');
        put('\n');

        highlightStart = std::string::npos;
        highlightEnd = std::string::npos;
    }

    insideMessage = false;
    stream->flush();
}


void Logger::setColor(TextColor color){
#   ifndef NDEBUG
        if(!insideMessage)
            throw std::logic_error("Cannot set the color outside of a message");
#   endif

    if(messageLogLevel > maxLevel)
        return;

    if(messageColor != color){
        messageColor = color;
        if(useColor)
            putColor(color);
    }
}


void Logger::write(const std::string &text){
#   ifndef NDEBUG
        if(!insideMessage)
            throw std::logic_error("Cannot write outside of a message");
#   endif

    if(messageLogLevel > maxLevel)
        return;

    *stream << text;
    messageLength += text.length();
}

void Logger::put(char c){
#   ifndef NDEBUG
        if(!insideMessage)
            throw std::logic_error("Cannot write outside of a message");
#   endif

    if(messageLogLevel > maxLevel)
        return;

    stream->put(c);
    messageLength++;
}


void Logger::writeHighlight(
    const std::string &text, TextColor color, size_t start, size_t end
){
#   ifndef NDEBUG
        if(highlightStart != std::string::npos)
            throw std::logic_error(
                "Multiple highlights in one message aren't supported."
            );
#   endif

    highlightStart = messageLength + start;
    highlightEnd = messageLength + end;

    if(useColor){
        for(size_t pos = 0; pos < start; ++pos)
            put(text[pos]);
        putColor(color);
        for(size_t pos = start; pos < end; ++pos)
            put(text[pos]);
        putColor(messageColor);
        for(size_t pos = end; pos < text.length(); ++pos)
            put(text[pos]);
    } else {
        *stream << text;
    }

    messageLength += text.length();
}

