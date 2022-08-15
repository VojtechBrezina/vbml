#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <exception>


/// The colors used in logging with some aliases for more clarity.
enum class TextColor {
    NONE = 0,

    RED = 31,
    GREEN = 32,
    YELLOW = 33,
    CYAN = 36,

    EROR = RED,
    WARNING = YELLOW,
    INFO = CYAN,
    DEBUG = GREEN,

    STRING = YELLOW,
    PARAM = YELLOW,
    NAME = GREEN,
};

/// The level at which any messages are logged to allow filtering as is usual.
enum class LogLevel {
    ERROR = 0, ///< Shown in red.
    WARNING = 1, ///< Shown in yellow.
    INFO = 2, ///< Shown in cyan.
    DEBUG = 3, ///< Shown in green.
};

/// A logger capable of displaying colorful or colorless output and filtering
/// messages that exxceed the desired log level.
///
/// First initialize the class passing in either a reference to a stream you
/// would like to manage yourself or a pointer to a newly created one.
/// The logger will try to detect the stream's capbility to display color, but
/// you can override that with Logger::enableColor.
/// Each message starts with a call to Logger::startMessage and ends with a call
/// to Logger::endMessage.
/// In between those you can call any of the write* methods. Passing a '\n' into
/// any of them will break the output, but the class doesn't check.
class Logger {
    // General settings:

    /// The maximum level allowed for this logger.
    LogLevel maxLevel;

    /// A pointer to the output stream.
    std::ostream *stream;

    /// If true, the Logger's destructor will delete the stream.
    bool ownsStream;

    /// Whether to use color or alternate highlihting methods.
    bool useColor = true;

    /// Not very robust override with enableColor if necessary.
    void detectTerminal();

    void putColor(TextColor c);


    // Message state:

    /// Check for errors in calling the begin / end methods.
    bool insideMessage = false;

    /// The tag of this message needs to be repeated when highlighting without
    /// colors.
    std::string messageTag;

    /// Check if we should output at all and match the highlight tag.
    LogLevel messageLogLevel;

    /// Useful for highlighting.
    TextColor messageColor;

    /// Useful for the colorless highlights.
    void writeMessageHeader();

    size_t highlightStart = std::string::npos;
    size_t highlightEnd = std::string::npos;


    public:
        /// Use a stream you do not manage, just by passing in the reference.
        Logger(
            LogLevel maxLevel, std::ostream &stream
        ): maxLevel(maxLevel), stream(&stream), ownsStream(false){
            detectTerminal();
        }

        /// Use a newly created stream and take ownership of it.
        Logger(
            LogLevel maxLevel, std::ostream *stream
        ): maxLevel(maxLevel), stream(stream), ownsStream(true){
            detectTerminal();
        }

        /// Delete the stream if you own it.
        ~Logger(){
            if(ownsStream)
                delete stream;
        }

        /// Turn color on or offk.
        void enableColor(bool v);

        /// Change the maximum acceptable log level.
        void changeLogLevel(LogLevel v);

        /// Start a single message.
        ///
        /// @param tag - The color coded tag to display at the begining.
        /// @param level - The log level of this message. Anything too high for
        ///  this logger will have the whole message automatically discarded.
        void startMessage(const std::string &tag, LogLevel level);

        /// End the current message.
        void endMessage();

        /// Set the text cololr for the next write call.
        void setColor(TextColor color);

        /// Write a piece of text to the stream. (Using \n in here should be
        /// avoided.
        void write(const std::string &text);

        /// Like write, but the selected area gets highlighted. Use this when
        /// you need to highlight the segment even in non color mode.
        void writeHighlight(
            const std::string &text, TextColor color, size_t start, size_t end
        );

        /// If your message doesn't need any colors, use this.
        void quickMessage(
            const std::string &tag, LogLevel level, const std::string &text
        ){
            startMessage(tag, level);
            write(text);
            endMessage();
        }
};