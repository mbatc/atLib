#ifndef atTrioExpressionBuilder_h__
#define atTrioExpressionBuilder_h__

#include "atHashSet.h"
#include "atTrioParserNode.h"

class atTrioExpressionBuilder
{
public:
  atTrioParserNode Build(const atVector<atTrioLexer::Token> &tokens);

protected:
  // Internal function used so that sublist don't need to be allocated
  atTrioParserNode Build(const atVector<atTrioLexer::Token> &tokens, const int64_t &first, const int64_t &last);
};

#endif // atTrioExpressionBuilder_h__
