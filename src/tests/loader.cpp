#include "tests.hpp"

#include <sstream>

#include "loader.hpp"

TestResult loaderTests(Logger &logger){
    static const std::string tag = "[tests][loader]";
    Tester tester(tag, logger);

    {
        std::istringstream input(
            "A short test file.\n"
            "Line two.\n"
            "Last line.\n"
        );

        SourceFile file { input };

        tester.assert(
            "SourceFile loads the correct number of lines",
            file.lineCount() == 3
        );

        tester.assert(
            "SourceFile can be indexed with size_t",
            file[1] == "Line two.\n"
        );

        tester.assert(
            "SourceFile can be indexed with SourcePos",
            file[{ 2, 1 }] == 'a'
        );

        tester.assert(
            "SourcePos can be moved to the next line",
            SourcePos { 1, 6 }.moved(5, file) == SourcePos { 2, 1 }
        );

        tester.assert(
            "SourcePos reports distance correctly",
            SourcePos { 1, 6 }.distanceTo(SourcePos { 2, 1 }, file) == 5
        );
    }

    return tester.getResult();
}
