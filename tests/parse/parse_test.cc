#include <iostream>
#include <string>
#include <assert.h>

#include "front/parse/parser.hh"

// The purpose of this file is for functionality testing.
// See the other file for unit testing.

auto main(void) -> int32_t
{
    std::string input = R"(
        a = 10;
        b = 20;
        c = a + b;
    )";

    std::string input_decl = R"(
        int a;
    )";

    std::string input_if = R"(
        int a;
        int e;
        if (a == 3) {
            e = e + 4;
        }
    )";

    std::string input_full = R"(
        int a;
        int b;
        int c;

        a = 3 + 2;
        b = a + 3;

        if (b == 8) {
            a = 2;
            c = 3;
        }
    )";

    etbit::lexer::lexer lex{ input_if };
    etbit::parser::parser prs{ lex };

    auto program = prs.parse_context();

    if (prs.get_error_count() > 0) {
        for (const auto& e : prs.errors) {
            std::cout << e << "\n";
        }
        std::exit(EXIT_FAILURE);
    }

    program.print(std::cout);
}
