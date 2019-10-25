#ifndef atTrioParser_h__
#define atTrioParser_h__

#include "atHashMap.h"
#include "atTrioResult.h"
#include "atTrioProgram.h"
#include "atTrioParserNode.h"
#include "atTrioExpressionBuilder.h"

enum atTrioParserError
{
  atTPE_Failure,
  atTPE_UnexpectedToken,
  atTPE_ExpectedToken,
  atTPE_UnknownIdentifier,
  atTPE_Count
};

class atTrioParser
{
public:
  atTrioParser(const atString &script = "");

  // Parse a script
  bool Parse(const atString &script);

  // Evaluate the parser tree into a Trio Program
  atTrioProgram EvaluateTree(atTrioState *pState);

  // Get errors produced
  const atVector<atTrioResult>& Errors();
  
  // Clear parser data
  void Clear();
  
protected:
  bool TryEatToken(const atTrioLangToken &expectedType);
  bool EatToken();
  bool NextToken();
  bool EndStatement();
  bool ThrowError(const atTrioParserError &error);

  const atTrioLexer::Token& GetCurrentToken();
  const atTrioLexer::Token& GetToken(const int64_t &index);

  struct
  {
    atVector<atTrioLexer::Token> curStatement;
    int64_t nextTokenID = 0;
    int64_t currentToken = 0;
    atTrioParserNode *pNode = nullptr;
  } m_context;

  atTrioLexer m_lexer;
  atTrioParserNode m_root;
  atTrioExpressionBuilder m_expressionBuilder;
  atVector<atTrioResult> m_errors;
};

#endif // atTrioParser_h__
