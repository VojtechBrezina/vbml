#include "tests.hpp"
#include "lexer.hpp"

#include <sstream>
#include <map>

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

        tester.assertEqual(
            "Lexer removes empty text",
            tokenKindNames.at(tokens.front().kind),
            tokenKindNames.at(TokenKind::COMMAND)
        );

        {
            std::deque<Token>::const_iterator token = tokens.begin();
            token += 5;

            tester.assertEqual(
                "Lexer reads a long command correctly",
                tokenKindNames.at(token->kind),
                tokenKindNames.at(TokenKind::TEXT)
            );

            tester.assertEqual(
                "Lexer loads token starts correctly",
                token->start.toString(),
                "1:16"
            );

            tester.assertEqual(
                "Lexer reports the correct token length",
                std::to_string(token->length),
                "20"
            );

            tester.assertEqual(
                "Lexer stores the correct token value",
                token->value,
                "\n   \nParagraph 1.\n \n"
            );
        }
    }

    return tester.getResult();
}
