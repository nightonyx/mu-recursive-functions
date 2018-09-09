#include <lib4aio/utils/boolean_utils/boolean_utils.h>

boolean is_space(const char c)
{
    return c == ' ';
}

boolean is_line_break(const char c)
{
    return c == '\n';
}

boolean is_space_or_line_break(const char c)
{
    return is_space(c) || is_line_break(c);
}

boolean is_opening_parenthesis(const char c)
{
    return c == '(';
}

boolean is_closing_parenthesis(const char c)
{
    return c == ')';
}

boolean is_opening_brace(const char c)
{
    return c == '{';
}

boolean is_closing_brace(const char c)
{
    return c == '}';
}

boolean is_equal_sign(const char c)
{
    return c == '=';
}

boolean is_sign(const char c)
{
    return
            c == '+'
            || c == '-'
            || c == '*'
            || c == '/'
            || c == '&'
            || c == '='
            || c == '~'
            || c == '>'
            || c == '<'
            || c == '%'
            || c == '^'
            || c == '!';
}

boolean is_dot(const char c)
{
    return c == '.';
}

boolean is_colon(const char c)
{
    return c == ':';
}

boolean is_comma(const char c)
{
    return c == ',';
}

boolean is_single_quote(const char c)
{
    return c == '\'';
}

boolean is_more_sign(const char c)
{
    return c == '>';
}

boolean is_less_sign(const char c)
{
    return c == '<';
}

boolean is_plus_sign(const char c)
{
    return c == '+';
}

boolean is_minus_sign(const char c)
{
    return c == '-';
}

boolean is_multiply_sign(const char c)
{
    return c == '*';
}

boolean is_division_sign(const char c)
{
    return c == '/';
}

boolean is_mod_sign(const char c)
{
    return c == '%';
}

boolean is_tilde_sign(const char c)
{
    return c == '~';
}

boolean is_and_sign(const char c)
{
    return c == '&';
}

boolean is_or_sign(const char c)
{
    return c == '|';
}

boolean is_exclamation_point(const char c)
{
    return c == '!';
}

boolean is_semicolon(const char c)
{
    return c == ';';
}
