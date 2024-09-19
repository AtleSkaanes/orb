#pragma once

// Project headers
#include <orb/parser/ast.hpp>
#include <orb/parser/types.hpp>

// STDLIB
#include <fstream>
#include <variant>


namespace parser
{
using numberVariant = std::variant<int8_t, int16_t, int32_t, int64_t, float, double, long double>;

class Lexer
{
public:
    struct TokenData
    {
    };

private:
    std::ifstream filestream;
    std::string identifier;
    numberVariant number;
    Type type;
    ast::Operator op;
    std::ifstream::pos_type prevTokPos;

public:
    Lexer(const char* path);

    TokenData NextToken();
    ast::Operator GetOperator();
    std::string GetIdentifier();
    Type GetType();
    numberVariant GetNumber();


private:
    void SkipWhitespace();
    bool NextIdentifier();
    bool NextNumber();
    /* bool NextOperator(); */
};

} // namespace parser
