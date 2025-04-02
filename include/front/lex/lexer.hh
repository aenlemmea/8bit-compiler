#ifndef ETBIT_LEXER_HH
#define ETBIT_LEXER_HH

#include "../token/token.hh"

namespace etbit::lexer
{
namespace impl
{

class lexer
{
public:
    explicit lexer(std::string_view _input) : input(_input)
    {
        read_char();
    }

    // Main API function for interacting with the lexer. Gets the next token from the input;
    [[nodiscard]] auto next_token() -> token::token
    {
        token::token output;
        skip_white_space();
        switch (at) {
            using enum token::tokens;
        case '+':
            output = token::token{PLUS, "+"};
            break;
        case '-':
            output = token::token{MINUS, "-"};
            break;
        case '=':
            if (peek_char() == '=') {
                output = token::token{EQUALS, "=="};
                read_char();
            } else {
                output = token::token{ASSIGNMENT, "="};
            }
            break;
        [[likely]] case ';':
            output = token::token{SEMICOLON, ";"};
            break;
        case '(':
            output = token::token{LPAREN, "("};
            break;
        case ')':
            output = token::token{RPAREN, ")"};
            break;
        case '{':
            output = token::token{LCURLY, "{"};
            break;
        case '}':
            output = token::token{RCURLY, "}"};
            break;
        case '0':
            output = token::token{ENDOFFILE, "EOF"};
            break;
        default:
            if (is_start_of_identifier()) {
                output.value = read_literal(token::literal::IDENTIFIER);
                output.kind = token::get_keyword_or_ident(output.value);
                return output;
            } else if (is_start_of_numeral()) {
                output.value = read_literal(token::literal::NUMERAL);
                output.kind = NUMERAL;
                return output;
            } else {
                output.value = "Terminal case reached. This should not happen";
            }
        }
        read_char();
        return output;
    }
private:
    std::string_view input;
    int32_t readpos = 0;
    int32_t pos = 0;
    char at;

    void read_char();
    char peek_char() const;
    void skip_white_space() noexcept;

    bool is_start_of_numeral() const;
    bool is_start_of_identifier() const;

    // Single method that provides a string (we use copy ellision here, hence not using reference or a string view)
    // given whether it is numeral or a identifer.
    [[nodiscard]] std::string_view read_literal(token::literal num_or_id) noexcept;
};
};
using namespace impl;
};

#endif

