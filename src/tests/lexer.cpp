#include "tests.hpp"
#include "lexer.hpp"

TestResult lexerTests(Logger &logger){
    const static std::string tag = "[tests][lexer]";

    Tester tester(tag, logger);

    {
        // SourceFile code = {
        //     "[h]1[]Heading[]",
        //     "   ",
        //     "Paragraph 1."
        //     " ",
        //     "[b]Paragraph[] 2."
        // };

        // std::deque<Token> tokens = tokenize(code, logger);
    }

    return tester.getResult();
}
