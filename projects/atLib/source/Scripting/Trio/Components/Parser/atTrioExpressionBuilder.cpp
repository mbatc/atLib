#include "atTrioExpressionBuilder.h"

atTrioParserNode atTrioExpressionBuilder::Build(const atVector<atTrioLexer::Token> &tokens) { return Build(tokens, 0, tokens.size()); }

atTrioParserNode atTrioExpressionBuilder::Build(const atVector<atTrioLexer::Token> &tokens, const int64_t &first, const int64_t &last)
{
  if (last - first == 1)
    return atTrioParserNode(tokens[first]);

  atTrioParserNode expression;
  for (int64_t i = first; i < last; ++i)
    if (tokens[i].type == atTL_Token_Symbol)
    {
      // Set Parameters
      if (first != i)
        expression.AddParameter(Build(tokens, first, i)); // Build sub expression
      expression.AddParameter(tokens[i]);
      if (i + 1 < last)
        expression.AddParameter(Build(tokens, i + 1, last)); // Build sub expression

      // Set Type
      if (expression.Parameter().size() < 3)
        expression.SetType(atTP_NT_Op);
      else
        expression.SetType(atTP_NT_BinOp);
      return expression;
    }

  // Two identifiers not separated by a symbol must be a declaration
  expression.SetType(atTP_NT_Declaration);
  for (int64_t i = first; i < last; ++i)
    expression.AddParameter(tokens[i]);
  return expression;
}

