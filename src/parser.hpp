#pragma once

#include <string>

#include "ast.hpp"

namespace Parser
{
enum class Status
{
    InExpr,
};

class TokenParser
{
   private:
    std::string _currBuffer = "";
    Ast::AstToken _tokenTree;

   public:
    TokenParser();

    void ParseNext(char currChar, char nextChar);

    Ast::AstToken GetAST();
};

}  // namespace Parser