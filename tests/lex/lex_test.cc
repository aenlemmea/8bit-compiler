#include <iostream>
#include <string>

#include "front/lex/lexer.hh"

// The purpose of this file is for functionality testing.
// See the other file for unit testing.

auto main(void) -> int32_t
{
    std::string input = R"(
        a = 10;
        b = 20;
        c = a + b;
        if (c == 30) {
            c = c + 1;
        }
    )";

    etbit::lexer::lexer lex{ input };
    for (std::size_t i = 0; i < 29; i += 1) {
        std::cout << i + 1 << " " <<  lex.next_token() << "\n";
    }
}
