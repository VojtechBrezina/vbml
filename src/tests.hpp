#pragma once

#include "logging.hpp"

/// Noone likes std::pair, right?
struct TestResult {
    int total = 0;
    int ok = 0;

    TestResult operator +=(const TestResult &other){
        total += other.total;
        ok += other.ok;
        return *this;
    }
};

/// A simple tool to help tests cooperate with loggers in a less verbose way.
class Tester {

    /// The logger tag to use.
    std::string tag;

    /// The logger to output to. (It is just assumed to outlive this instance.)
    Logger *logger;

    TestResult result;

    public:

        Tester(
            const std::string &tag, Logger &logger
        ): tag(tag), logger(&logger){}

        bool assertTrue(const std::string &message, bool cond);

        bool assertEqual(
            const std::string & message, 
            const std::string &left, const std::string &right
        );

        TestResult getResult() const { return result; }
};

/// Run all the tests for the program.
bool runTests();

/// These are defined in src/tests/*.cpp
TestResult loggingTests(Logger &logger);
TestResult lexerTests(Logger &logger);
TestResult loaderTests(Logger &logger);
