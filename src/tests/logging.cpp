#include "tests.hpp"

#include <sstream>

TestResult loggingTests(Logger &logger){
    const static std::string tag = "[tests][logging]";

    Tester tester(tag, logger);

    // A simple warning in a colorless stream.
    {
        std::ostringstream oss;
        Logger l(LogLevel::DEBUG, oss);
        l.quickMessage("[test]", LogLevel::WARNING, "This is a test message.");
        if(!tester.assert("Logger writes a simple message without color",
            oss.str() == "W[test]: This is a test message.\n"
        )){
            logger.quickMessage(tag, LogLevel::DEBUG, oss.str());
        }
    }

    // A highlight without colors.
    {
        std::ostringstream oss;
        Logger l(LogLevel::DEBUG, oss);
        l.startMessage("[test]", LogLevel::ERROR);
        l.write("Something. ");
        l.writeHighlight(
            "There is a highlighted segment in here.", TextColor::RED, 11, 30
        );
        l.endMessage();
        tester.assertEqual(
            "Logger can highlight a segment without color",
            oss.str(),
            "E[test]: Something. There is a highlighted segment in here.\n"
            "E[test]:                       ^^^^^^^^^^^^^^^^^^^\n"
        );
    }

    return tester.getResult();
}
