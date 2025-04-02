#ifndef ETBIT_TOKEN_HH
#define ETBIT_TOKEN_HH

#include <stdint.h>
#include <string>
#include <ostream>
#include <unordered_map>

namespace etbit::token
{
namespace impl
{
// Do NOT change the order of definition.
// The enum is class qualified for type safety and scoped to 8 bit unsigned ints since the language is small.
enum class tokens : uint8_t {
    TYPE_INT_DECL,
    IDENT,
    NUMERAL,

    ASSIGNMENT,
    PLUS,
    MINUS,

    LPAREN,
    RPAREN,
    LCURLY,
    RCURLY,

    EQUALS,
    IF,

    SEMICOLON,
    ILLEGAL,
    ENDOFFILE,
};

enum class literal : char {
    NUMERAL,
    IDENTIFIER,
};

// Returns if an token kind is operator or not.
inline bool is_operator(tokens tok)
{
    return tok == tokens::PLUS || tok == tokens::MINUS || tok == tokens::EQUALS || tok == tokens::ASSIGNMENT;
}

// Map to store token strings. C++ enums are primitive!
inline std::unordered_map<tokens, std::string> tokmap = {
    {tokens::TYPE_INT_DECL, "TYPE_INT_DECL"},
    {tokens::IDENT, "IDENT"},
    {tokens::NUMERAL, "NUMERAL"},
    {tokens::ASSIGNMENT, "ASSIGNMENT"},
    {tokens::PLUS, "PLUS"},
    {tokens::MINUS, "MINUS"},
    {tokens::LPAREN, "LPAREN"},
    {tokens::RPAREN, "RPAREN"},
    {tokens::LCURLY, "LCURLY"},
    {tokens::RCURLY, "RCURLY"},
    {tokens::EQUALS, "EQUALS"},
    {tokens::IF, "IF"},
    {tokens::SEMICOLON, "SEMICOLON"},
    {tokens::ILLEGAL, "ILLEGAL"},
    {tokens::ENDOFFILE, "ENDOFFILE"},
};

// Provides the struct for the token datatype. Since this is a aggregate type, it would be
// logical to use the default provided constructors but explicit constructor will allow the lexer
// to define on-the-fly values and restrict out of behaviour expectation.
struct token {
    tokens kind;
    std::string value;

    explicit token(tokens _kind, std::string_view _value) : kind(_kind), value(_value) {}

    token() : token{tokens::ILLEGAL, "-1"} {}

    friend std::ostream &operator<<(std::ostream& os, const token& t)
    {
        os << "Kind: " << tokmap[t.kind] << " Value: " << t.value;
        return os;
    }
};

// Helper function for the lexer to see if a string given is a recognized keyword or not.
inline tokens get_keyword_or_ident(std::string_view val)
{
    if (val == "if") {
        return tokens::IF;
    } else if (val == "int") {
        return tokens::TYPE_INT_DECL;
    } else return tokens::IDENT;
}
};

using namespace impl;
};

#endif
