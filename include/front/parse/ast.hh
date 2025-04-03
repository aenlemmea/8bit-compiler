#ifndef ETBIT_PARSE_AST_HH
#define ETBIT_PARSE_AST_HH

#include <vector>
#include <memory>
#include <exception>
#include "../token/token.hh"

#define stringify(x) #x

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
    virtual void generate(std::ostream&) const {}
    virtual void _gen_left(std::ostream&) const {}
    virtual void _gen_right(std::ostream&) const {}
};

struct stmnt : node {
    virtual ~stmnt() = default;
    virtual void print(std::ostream& os) const = 0;
    virtual void generate(std::ostream&) const {}
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
            os << "\n";
        }
    }

    void generate(std::ostream& os) const {
        for (const auto& e: statements) {
            if (e != nullptr) {
                e->generate(os);
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
        os << stringify(expr_kind::TYPE) << " " << token;
    }

    void generate(std::ostream &) const override {
        // No OP <>
    }
};

struct identifier : public expr {
    expr_kind kind = expr_kind::IDENTIFIER;
    token::token token;

    friend struct infix_expr;

    identifier(token::token&& _tok) : token(_tok) {}
    identifier() : identifier{token::token{token::tokens::ILLEGAL, "-1"}} {}

    void print(std::ostream& os) const override
    {
        os << stringify(expr_kind::IDENTIFIER) << " " << token;
    }

    void generate(std::ostream & os) const override {
        // TODO
    }

    void _gen_left(std::ostream& os) const override {
        os << "lda " << /*TODO*/ "\n";
    }

    void _gen_right(std::ostream& os) const override {
        os << "mov B M " << /*TODO*/ "\n";
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
        os << stringify(expr_kind::NUM) << " " << token;
    }

    void generate(std::ostream & os) const override {
        // TODO
    }

    void _gen_left(std::ostream& os) const override {
        os << "ldi A" << /*TODO*/ "\n";
    }

    void _gen_right(std::ostream& os) const override {
        os << "ldi B" << /*TODO*/ "\n";
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
        os << stringify(expr_kind::INFIX) << " \n";
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

    void generate(std::ostream & os) const override {
        left->_gen_left(os);
        right->_gen_right(os);

        if (oper == "+") {
            os << "add\n";
        } else if (oper == "=") {

        } else if (oper == "-") {
            os << "sub\n";
        } else if (oper == "==") {
            os << "cmp\n";
        } else {
            throw std::runtime_error( "unsupported operation: " + oper );
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
        os << stringify(expr_kind::CONDN) << " \n";
        if (condition != nullptr) {
            condition->print(os);
        } else {
            os << "condition_expr: nullptr";
        }
    }

    void generate(std::ostream & os) const override {
        condition->generate(os);
    }
};

// Statements

enum class stmnt_kind {
    EXPR_STMNT,
    IF_STMNT,
    ASSIGN_STMNT,
    VARDEC_STMNT,
};

// Wrapper around expr (more generally hosts rvalues).
struct expr_stmnt : public stmnt {
    stmnt_kind kind = stmnt_kind::EXPR_STMNT;
    expr_ctl expression;

    expr_stmnt(expr_ctl _expression) : expression(std::move(_expression)) {}
    expr_stmnt() = default;

    void print(std::ostream & os) const override
    {
        os << "\n";
        os << stringify(stmnt_kind::EXPR_STMNT) << " \n";
        if (expression != nullptr) {
            expression->print(os);
        } else {
            os << "expr_stmnt: nullptr";
        }
    }

    void generate(std::ostream & os) const override {
        expression->generate(os);
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
        os << stringify(stmnt_kind::VARDEC_STMNT) << " \n";
        os << "TYPE: \n";
        decl_type.print(os);
        os << "\nIDENT: \n";
        ident.print(os);
    }

    void generate(std::ostream & os) const override {
        ident.generate(os);
    }
};

struct [[deprecated("Use infix_expr combined with expr_stmnt"), maybe_unused]] assign_stmnt : public stmnt {
    stmnt_kind kind = stmnt_kind::ASSIGN_STMNT;
    identifier ident;
    expr_ctl expression;

    assign_stmnt(identifier _ident, expr_ctl _expression) : ident(_ident), expression(std::move(_expression)) {}
    assign_stmnt() = default;

    void print(std::ostream & os) const override
    {
        os << stringify(stmnt_kind::ASSIGN_STMNT) << " \n";
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

    void generate(std::ostream & os) const override {
        cond->generate(os);
        os << "jne %donelabel\n";
        body.generate(os);
        os << "jmp %donelabel\n";
        os << "donelabel: \n";
    }
};

};

using namespace impl;
};

#endif
