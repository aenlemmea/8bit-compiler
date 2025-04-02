#include "front/parse/parser.hh"

#include <assert.h>

#define SEMI_CHECK {\
                    if (!expect_token(token::tokens::SEMICOLON)) { \
                        add_error("Expected a SEMICOLON"); return nullptr; \
                    }\
}

bool etbit::parser::parser::is_current_token(token::tokens kind) const
{
    return current_token.kind == kind;
}

bool etbit::parser::impl::parser::is_peek_token(token::tokens kind) const
{
    return peek_token.kind == kind;
}

auto etbit::parser::impl::parser::get_error_count() const
{
    return errors.size();
}

void etbit::parser::impl::parser::add_error(std::string msg)
{
    errors.emplace_back(__PRETTY_FUNCTION__ + msg + " Seeing: " + token::tokmap[peek_token.kind] + " Currently: " + token::tokmap[current_token.kind]);
}

void etbit::parser::impl::parser::set_next_token() noexcept
{
    current_token = peek_token;
    peek_token = lexer.next_token();
}

bool etbit::parser::impl::parser::expect_token(token::tokens tok)
{
    if (is_peek_token(tok)) {
        set_next_token();
        return true;
    }
    return false;
}

etbit::ast::context etbit::parser::parser::parse_context()
{
    etbit::ast::context program;
    while (!is_current_token(token::tokens::ENDOFFILE)) {
        auto statement = parse_statement();
        program.add_statement(std::move(statement));
        set_next_token();
    }
    return program;
}

// Parsing core methods

std::unique_ptr<etbit::ast::expr_stmnt> etbit::parser::impl::parser::parse_expr_stmnt()
{
    auto expr = parse_expr();
    return std::make_unique<ast::expr_stmnt>(std::move(expr));

}

etbit::ast::expr_ctl etbit::parser::impl::parser::parse_expr()
{
    auto left = parse_comparison_expr();
    assert(left != nullptr);

    if (expect_token(token::tokens::ASSIGNMENT)) {
        // TODO Same code is repeated thrice, maybe look into macros or else do a template mp function for this.
        // Only thing different is the function calls in right part.
        auto oper_copy = current_token;
        set_next_token();

        auto right = parse_expr();
        assert(right != nullptr);
        return std::make_unique<ast::infix_expr>(std::move(left), oper_copy.value, std::move(right));
    }

    while (expect_token(token::tokens::PLUS) || expect_token(token::tokens::MINUS)) {
        auto oper_copy = current_token;
        set_next_token();

        auto right = parse_factor();
        assert(right != nullptr);
        left = std::make_unique<ast::infix_expr>(std::move(left), oper_copy.value, std::move(right));
    }
    return left;
}

// Handles the == operator (comparisons)
etbit::ast::expr_ctl etbit::parser::impl::parser::parse_comparison_expr()
{
    auto left = parse_factor();
    assert(left != nullptr);

    if (expect_token(token::tokens::EQUALS)) {
        auto oper_copy = current_token;
        set_next_token();

        auto right = parse_factor();
        assert(right != nullptr);
        left = std::make_unique<ast::infix_expr>(std::move(left), oper_copy.value, std::move(right));
    }
    return left;
}

etbit::ast::expr_ctl etbit::parser::impl::parser::parse_factor()
{
    if (is_current_token(token::tokens::NUMERAL)) {
        // TODO Handle the exceptions for stoi.
        return std::make_unique<ast::numeral>(std::stoi(current_token.value), current_token);
    } else if (is_current_token(token::tokens::IDENT)) {
        return std::make_unique<ast::identifier>(std::move(current_token));
    } else {
        return nullptr;
    }
}

// Parse int a; type statements
std::unique_ptr<etbit::ast::vardecl_stmnt> etbit::parser::impl::parser::parse_decl_statement()
{
    auto statem = std::make_unique<ast::vardecl_stmnt>();
    statem->decl_type.token = current_token;

    if (!expect_token(token::tokens::IDENT)) {
        add_error("parse_decl_statement: Expected IDENT");
        return nullptr;
    }

    statem->ident.token = current_token;
    SEMI_CHECK

    return statem;
}

std::unique_ptr<etbit::ast::if_stmnt> etbit::parser::impl::parser::parse_if_statement()
{
    // TODO
    return nullptr;
}






