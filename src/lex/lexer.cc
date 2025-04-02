#include "front/lex/lexer.hh"
#include <assert.h>

// Mutates the object. Reads the character one at a time. If reading over, sets current character to EOF.
void etbit::lexer::lexer::read_char()
{
    assert(!input.empty());
    at = ((readpos >= input.length()) ? '0' : input[readpos]);
    pos = readpos;
    readpos += 1;
}

char etbit::lexer::impl::lexer::peek_char() const
{
    return (readpos >= input.length() ? '0' : input[readpos]);
}

bool etbit::lexer::impl::lexer::is_start_of_identifier() const
{
    return (('a' <= at && at <= 'z') || ('A' <= at && at <= 'Z'));
}

bool etbit::lexer::impl::lexer::is_start_of_numeral() const
{
    return ('0' <= at && at <= '9');
}

std::string_view etbit::lexer::impl::lexer::read_literal(token::literal num_or_id) noexcept
{
    assert(pos <= input.length());
    auto position = pos;
    while (num_or_id == token::literal::NUMERAL ? is_start_of_numeral() : is_start_of_identifier()) {
        read_char();
    }
    return input.substr(position, pos - position);
}

void etbit::lexer::impl::lexer::skip_white_space() noexcept
{
    while ((at == ' ') || (at == '\r') || (at == '\n') || (at == '\t')) {
        read_char();
    }
}


