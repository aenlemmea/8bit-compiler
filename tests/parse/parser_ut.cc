#include "ut.hpp"
#include <string>

#include "front/parse/parser.hh"
#include <sstream>

std::string trim(std::string& str)
{
    std::string spd;
    for (auto c : str) {
        if (!(c == ' ' || c == '\r' || c == '\n' || c == '\t')) {
            spd += c;
        }
    }
    return spd;
}

int main(void)
{

    using namespace boost::ut;
    std::vector<std::tuple<std::string, std::string, int >> inputexp;
    inputexp.reserve(3);

    inputexp.push_back({"int a;", R"(stmnt_kind::VARDEC_STMNT
    TYPE:
    expr_kind::TYPE Kind: TYPE_INT_DECL Value: int
    IDENT:
    expr_kind::IDENTIFIER Kind: IDENT Value: a
    )", 1});

    inputexp.push_back({R"(int a;
        int e;
        if (a == 3) {
            e = e + 4;
        })", R"(
stmnt_kind::VARDEC_STMNT
TYPE:
expr_kind::TYPE Kind: TYPE_INT_DECL Value: int
IDENT:
expr_kind::IDENTIFIER Kind: IDENT Value: a
stmnt_kind::VARDEC_STMNT
TYPE:
expr_kind::TYPE Kind: TYPE_INT_DECL Value: int
IDENT:
expr_kind::IDENTIFIER Kind: IDENT Value: e
 IF COND: expr_kind::CONDN
expr_kind::INFIX
Left: expr_kind::IDENTIFIER Kind: IDENT Value: a
Oper: ==
Right: expr_kind::NUM Kind: NUMERAL Value: 3 BODY:
stmnt_kind::EXPR_STMNT
expr_kind::INFIX
Left: expr_kind::IDENTIFIER Kind: IDENT Value: e
Oper: =
Right: expr_kind::INFIX
Left: expr_kind::IDENTIFIER Kind: IDENT Value: e
Oper: +
Right: expr_kind::NUM Kind: NUMERAL Value: 4
             )", 3});


    std::vector<std::pair<std::string, int >> got(3);

    for (size_t i = 0; i < inputexp.size(); i++) {
        std::string exp;
        auto e = inputexp[i];
        etbit::lexer::lexer lx{std::get<0>(e)};
        etbit::parser::parser prs{ lx };
        auto prog = prs.parse_context();
        std::stringstream ss;
        prog.print(ss);
        got[i].first = ss.str();
        got[i].second = prog.statements.size();
    }

    "parser_ut_dump"_test = [&got, &inputexp]() {
        expect(trim(got[0].first) == trim(std::get<1>(inputexp[0]))) << "Test case 1 failed";
        expect(trim(got[1].first) == trim(std::get<1>(inputexp[1]))) << "Test case 2 failed";
    };

    "parser_ut_num_st"_test = [&got]() {
        expect(got[0].second == 1);
        expect(got[1].second == 3);
    };

}
