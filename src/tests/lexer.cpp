#include "tests.hpp"
#include "lexer.hpp"

#include <sstream>

#include "loader.hpp"


TestResult lexerTests(Logger &logger){
    const static std::string tag = "[tests][lexer]";

    Tester tester(tag, logger);

    {
        std::istringstream input;
        input.str(
            "[h]1[]Heading[]\n"
            "   \n"
            "Paragraph 1.\n"
            " \n"
            "[b]Paragraph[] 2.\n"
        );

        SourceFile code { input };

        std::deque<Token> tokens = tokenize(code, logger);


    }

    return tester.getResult();
}
