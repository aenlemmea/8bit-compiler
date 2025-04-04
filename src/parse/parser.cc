#include "front/parse/parser.hh"

#include <assert.h>
#include <utility>
#include <algorithm>

#define SEMI_CHECK {\
                    if (!expect_token(token::tokens::SEMICOLON)) { \
                        add_error("Expected a SEMICOLON", __PRETTY_FUNCTION__); return nullptr; \
                    }\
}

// Helper methods.

void etbit::parser::impl::parser::set_next_token() noexcept
{
    current_token = std::exchange(peek_token, lexer.next_token());
}

void etbit::parser::impl::parser::add_var(std::string& value)
{
    decl_vars.push_back(value);
}

bool etbit::parser::impl::parser::is_declared(std::string& value) const
{
    return std::find(decl_vars.begin(), decl_vars.end(), value) != decl_vars.end();
}

bool etbit::parser::parser::is_current_token(token::tokens kind) const
{
    return current_token.kind == kind;
}

bool etbit::parser::impl::parser::is_peek_token(token::tokens kind) const
{
    return peek_token.kind == kind;
}

std::size_t etbit::parser::impl::parser::get_error_count() const
{
    return errors.size();
}

void etbit::parser::impl::parser::add_error(std::string msg, std::string func)
{
    errors.emplace_back(func + " " + msg + " Seeing: " + token::tokmap[peek_token.kind] + " Currently: " + token::tokmap[current_token.kind]);
}

bool etbit::parser::impl::parser::expect_token(token::tokens tok)
{
    if (is_peek_token(tok)) {
        set_next_token();
        return true;
    }
    return false;
}

// Actual public api to start parsing. A context is a given source unit to be converted to a program.
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

// Redirection method to convert from expr -> expr_stmnt
volatile std::unique_ptr<etbit::ast::stmnt> etbit::parser::impl::parser::parse_expr_stmnt()
{
    auto expr = parse_expr();

    // if it is a infix expr.
    // yes -> left is IDENT?
    //        oper is =
    // yes? -> return a assign_statement;
    // no? -> do as usual.
    if (auto inf = dynamic_cast<ast::infix_expr*>(expr.get()); inf != nullptr) {
            if (inf->oper == "=") {
                if (auto left_hold = dynamic_cast<ast::identifier*>(inf->left.get()); left_hold != nullptr) {

                    return std::make_unique<ast::assign_stmnt>(*left_hold, std::move(inf->right));
                }
        }
    } else {
        return std::make_unique<ast::expr_stmnt>(std::move(expr));
    }

    return nullptr;
}

// Driver function to handle longest matching sequence.
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

// Smallest atom that can be parsed.
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
        add_error("parse_decl_statement: Expected IDENT", __PRETTY_FUNCTION__);
        return nullptr;
    }

    statem->ident.token = current_token;
    SEMI_CHECK

    return statem;
}

// Parse if (a == 3) { e = e + 4; } type of statements.
std::unique_ptr<etbit::ast::if_stmnt> etbit::parser::impl::parser::parse_if_statement()
{
    if (!is_current_token(token::tokens::IF)) {
        add_error("Expected an IF", __PRETTY_FUNCTION__);
        return nullptr;
    }

    if (!expect_token(token::tokens::LPAREN)) {
        add_error("Expect '(' after IF", __PRETTY_FUNCTION__);
        return nullptr;
    }

    set_next_token();

    // Handle conditional expressions.
    auto condition = parse_comparison_expr();
    assert(condition != nullptr);

    if (!expect_token(token::tokens::RPAREN)) {
        add_error("Expected ')' after CONDITION", __PRETTY_FUNCTION__);
        return nullptr;
    }

    if (!expect_token(token::tokens::LCURLY)) {
        add_error("Expected '{' after CONDITION", __PRETTY_FUNCTION__);
        return nullptr;
    }

    auto statem = std::make_unique<ast::if_stmnt>(); // Do not change the allocation order.
    statem->cond = std::make_unique<ast::condition_expr>(std::move(condition));

    while (!is_current_token(token::tokens::RCURLY) && !is_current_token(token::tokens::ENDOFFILE)) {
        auto body_stmnt = parse_statement();
        statem->body.add_statement(std::move(body_stmnt));
        set_next_token();
    }

    if (!is_current_token(token::tokens::RCURLY)) {
        add_error("Expected '}' after CONDITION", __PRETTY_FUNCTION__);
        return nullptr;
    }
    return statem;
}






