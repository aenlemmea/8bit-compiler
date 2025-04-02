#ifndef ETBIT_PARSER_HH
#define ETBIT_PARSER_HH

#include "../lex/lexer.hh"
#include "ast.hh"

namespace etbit::parser
{
namespace impl
{
class parser
{
public:
    explicit parser(lexer::lexer& _lexer) : lexer(_lexer) {}
    ast::context parse_context();
    auto get_error_count() const;
private:
    lexer::lexer lexer;
    token::token current_token;
    token::token peek_token;
    std::vector<std::string> errors;

    void add_error(std::string msg);

    [[nodiscard]] bool is_current_token(token::tokens kind) const;
    [[nodiscard]] bool is_peek_token(token::tokens kind) const;
    [[nodiscard]] bool expect_token(token::tokens tok);
    void set_next_token() noexcept;

    auto parse_statement() -> std::unique_ptr<ast::stmnt>
    {
        switch (current_token.kind) {
            using enum token::tokens;
        case TYPE_INT_DECL:
            return parse_decl_statement();
        case IDENT:
        case NUMERAL:
            return parse_expr_stmnt();
        case IF:
            return parse_if_statement();
        default:
            return nullptr;
        }
    }

    // Methods for expression parsing. Using recursive descent.
    ast::expr_ctl parse_expr();
    ast::expr_ctl parse_comparison_expr();
    ast::expr_ctl parse_factor();

    // Methods for actually returning statements.
    std::unique_ptr<ast::expr_stmnt> parse_expr_stmnt();
    std::unique_ptr<ast::vardecl_stmnt> parse_decl_statement();
    std::unique_ptr<ast::if_stmnt> parse_if_statement();

    [[deprecated("parse_expr_stmnt overlaps with assignment"), maybe_unused]] std::unique_ptr<ast::assign_stmnt> parse_assign_statement();

};
};

using namespace impl;
};

#endif
