#include "ut.hpp"
#include <string>

#include "front/lex/lexer.hh"

int main(void) {

    using namespace boost::ut;
    std::vector<std::tuple<std::string, std::string, int>> inputexp;
    inputexp.reserve(3);
    inputexp.push_back({"int a;", "Kind: TYPE_INT_DECL Value: int\nKind: IDENT Value: a\nKind: SEMICOLON Value: ;\n", 3});
    inputexp.push_back({"a = 2 + b;", "Kind: IDENT Value: a\nKind: ASSIGNMENT Value: =\nKind: NUMERAL Value: 2\nKind: PLUS Value: +\nKind: IDENT Value: b\nKind: SEMICOLON Value: ;\n",6});
    inputexp.push_back({"if(a == 3) { e = 5; }", "Kind: IF Value: if\nKind: LPAREN Value: (\nKind: IDENT Value: a\nKind: EQUALS Value: ==\nKind: NUMERAL Value: 3\nKind: RPAREN Value: )\nKind: LCURLY Value: {\nKind: IDENT Value: e\nKind: ASSIGNMENT Value: =\nKind: NUMERAL Value: 5\nKind: SEMICOLON Value: ;\nKind: RCURLY Value: }\n", 12});

    std::vector<std::string> got(3, "");

    for (size_t i = 0; i < inputexp.size(); i++) {
        std::string exp;
        auto e = inputexp[i];
        etbit::lexer::lexer lx{std::get<0>(e)};
        for (int i = 0; i < std::get<2>(e); i += 1) {
            exp += lx.next_token().to_string();
        }
        got[i] = (exp);
    }

    "lexer_ut"_test = [&got, &inputexp]() {
        expect(got[0] == std::get<1>(inputexp[0])) << "Test case 1 failed";
        expect(got[1] == std::get<1>(inputexp[1])) << "Test case 2 failed";
        expect(got[2] == std::get<1>(inputexp[2])) << "Test Case 3 failed";
    };

}
