#include "tests.hpp"

bool Tester::assert(const std::string &message, bool cond){
    result.total++;

    logger->startMessage(tag, LogLevel::INFO);
    logger->write(message);
    logger->write(": ");

    if(cond){
        logger->write(TextColor::GOOD, "OK");
        result.ok++;
    } else {
        logger->write(TextColor::BAD, "Fail");
    }

    logger->endMessage();

    return cond;
}

bool Tester::assertEqual(
    const std::string &message,
    const std::string &left, const std::string &right
){
    bool result = assert(message, left == right);

    if(!result){
        logger->startMessage(tag, LogLevel::ERROR);
        logger->write(TextColor::STRING, left);
        logger->write(" != ");
        logger->write(TextColor::STRING, right);
        logger->endMessage();
    }

    return result;
}

bool runTests(){
    const std::string tag = "[tests]";
    Logger logger(LogLevel::DEBUG, std::clog);
    TestResult result;

    logger.quickMessage(tag, LogLevel::INFO, "");
    logger.quickMessage(tag, LogLevel::INFO, "Running tests.");
    logger.quickMessage(tag, LogLevel::INFO, "");

    result += loggingTests(logger);
    result += loaderTests(logger);
    result += lexerTests(logger);

    // Log the final results.

    logger.startMessage(tag, LogLevel::INFO);
    logger.write("Final result: ");
    logger.write(
        result.ok == result.total ? TextColor::GOOD : TextColor::BAD,
        std::to_string(result.ok)
    );
    logger.write(TextColor::NONE, "/");
    logger.write(std::to_string(result.total));
    logger.endMessage();

    return result.ok == result.total;
}
