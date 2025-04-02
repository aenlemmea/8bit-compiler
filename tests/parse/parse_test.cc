#include <iostream>
#include <string>

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

    etbit::lexer::lexer lex{ input };
    etbit::parser::parser prs{ lex };

    auto program = prs.parse_context();

    for (auto& e : program.statements) {
        e->print(std::cout);
        std::cout << "\n\n";
    }
}
