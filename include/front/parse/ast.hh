#ifndef ETBIT_PARSE_AST_HH
#define ETBIT_PARSE_AST_HH

#include <vector>
#include <memory>
#include "../token/token.hh"

#define str(x) #x

namespace etbit::ast
{
namespace impl
{

enum class expr_kind {
    TYPE,
    IDENTIFIER,
    NUM,
    INFIX,
    CONDN,
};

// Allow for Empty Base Optimization.
struct node {};

struct expr : node {
    virtual ~expr() = default;
    virtual void print(std::ostream& os) const = 0;
};

struct stmnt : node {
    virtual ~stmnt() = default;
    virtual void print(std::ostream& os) const = 0;
};

struct context : node {
    std::vector<std::unique_ptr<stmnt >> statements;

    void add_statement(auto statement)
    {
        if (statement != nullptr) {
            statements.push_back(std::move(statement));
        }
    }

    void print(std::ostream& os) const {
        for (const auto& e: statements)
        {
            if (e != nullptr) {
                e->print(os);
            }
            else {
                os << "context: nullptr in statement";
            }
        }
    }
};

using expr_ctl = std::unique_ptr<expr>;

// Expressions

struct type : public expr {
    expr_kind kind = expr_kind::TYPE;
    token::token token;

    enum class prec {
        EIGHT,
        FOUR
    } precision = prec::EIGHT; // For future use.

    type(token::token _token, prec _precision) : token(_token), precision(_precision) {}
    type() = default;

    void print(std::ostream& os) const override
    {
        os << str(expr_kind::TYPE) << " " << token;
    }
};

struct identifier : public expr {
    expr_kind kind = expr_kind::IDENTIFIER;
    token::token token;

    identifier(token::token&& _tok) : token(_tok) {}
    identifier() : identifier{token::token{token::tokens::ILLEGAL, "-1"}} {}

    void print(std::ostream& os) const override
    {
        os << str(expr_kind::IDENTIFIER) << " " << token;
    }
};

struct numeral : public expr {
    expr_kind kind = expr_kind::NUM;
    int8_t value;
    token::token token;

    numeral(int8_t _value, token::token& _token) : value(_value), token(_token) {}
    numeral() = default;

    void print(std::ostream & os) const override
    {
        os << str(expr_kind::NUM) << " " << token;
    }
};

struct infix_expr : public expr {
    expr_ctl left;
    std::string oper;
    expr_ctl right;
    expr_kind kind = expr_kind::INFIX;

    infix_expr(expr_ctl _left, std::string_view _oper, expr_ctl _right) : left(std::move(_left)), oper(_oper), right(std::move(_right)) {}
    infix_expr() = default;

    void print(std::ostream& os) const override
    {
        os << str(expr_kind::INFIX) << " \n";
        os << "Left: ";
        if (left != nullptr) {
            left->print(os);
        } else {
            os << "infix_expr: nullptr in left";
        }
        os << "\nOper: " << oper;
        os << "\nRight: ";

        if (right != nullptr) {
            right->print(os);
        } else {
            os << "infix_expr: nullptr in right";
        }
    }
};

struct condition_expr : public expr {
    expr_kind kind = expr_kind::CONDN;
    expr_ctl condition;

    condition_expr(expr_ctl _condition) : condition(std::move(_condition)) {}
    condition_expr() = default;

    void print(std::ostream & os) const override
    {
        os << str(expr_kind::CONDN) << " \n";
        if (condition != nullptr) {
            condition->print(os);
        } else {
            os << "condition_expr: nullptr";
        }
    }
};

// Statements

enum class stmnt_kind {
    EXPR_STMNT,
    IF_STMNT,
    ASSIGN_STMNT,
    VARDEC_STMNT,
};

// General expr statement to hold rvalues.
struct expr_stmnt : public stmnt {
    stmnt_kind kind = stmnt_kind::EXPR_STMNT;
    expr_ctl expression;

    expr_stmnt(expr_ctl _expression) : expression(std::move(_expression)) {}
    expr_stmnt() = default;

    void print(std::ostream & os) const override
    {
        os << str(stmnt_kind::EXPR_STMNT) << " \n";
        if (expression != nullptr) {
            expression->print(os);
        } else {
            os << "expr_stmnt: nullptr";
        }
    }
};

// For statements like int a;
struct vardecl_stmnt : public stmnt {
    stmnt_kind kind = stmnt_kind::VARDEC_STMNT;
    type decl_type;
    identifier ident;

    vardecl_stmnt(type _decl_type, identifier _ident) : decl_type(_decl_type), ident(_ident) {}
    vardecl_stmnt() = default;

    void print(std::ostream & os) const override
    {
        os << str(stmnt_kind::VARDEC_STMNT) << " \n";
        os << "TYPE: ";
        decl_type.print(os);
        os << "IDENT: ";
        ident.print(os);
    }
};

// For statements like a = 2 + 3;
struct [[deprecated("Use infix_expr combined with expr_stmnt"), maybe_unused]] assign_stmnt : public stmnt {
    stmnt_kind kind = stmnt_kind::ASSIGN_STMNT;
    identifier ident;
    expr_ctl expression;

    assign_stmnt(identifier _ident, expr_ctl _expression) : ident(_ident), expression(std::move(_expression)) {}
    assign_stmnt() = default;

    void print(std::ostream & os) const override
    {
        os << str(stmnt_kind::ASSIGN_STMNT) << " \n";
        os << "IDENT: ";
        ident.print(os);
        if (expression != nullptr) {
            expression->print(os);
        } else {
            os << "assign_stmnt: nullptr";
        }
    }
};

// If conditions.
struct if_stmnt : public stmnt {
    using condition = std::unique_ptr<condition_expr>;
    stmnt_kind kind = stmnt_kind::IF_STMNT;
    condition cond;
    context body;

    if_stmnt(condition _cond, context&& _body) : cond(std::move(_cond)), body(std::move(_body)) {}
    if_stmnt() = default;

    void print(std::ostream & os) const override
    {
        os << " IF COND: ";
        if (cond != nullptr) {
            cond->print(os);
        } else {
            os << "if_stmnt: nullptr in condition";
        }
        os << " BODY: ";
        body.print(os);
    }
};

};

using namespace impl;
};

#endif
