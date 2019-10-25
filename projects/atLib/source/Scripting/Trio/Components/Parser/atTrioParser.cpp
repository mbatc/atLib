#include "atTrioParser.h"
#include "atTrioExpressionBuilder.h"

atTrioParser::atTrioParser(const atString &script)
  : m_root(atTP_NT_Block)
{
  if (script.length())
    Parse(script);
}

bool atTrioParser::Parse(const atString &script)
{
  Clear();
  m_lexer.Tokenize(script);
  while (EatToken());
  return Errors().size() == 0;
}

atTrioProgram atTrioParser::EvaluateTree(atTrioState *pState)
{
  return atTrioProgram(nullptr);
}

bool atTrioParser::TryEatToken(const atTrioLangToken &expectedType)
{
  if (GetCurrentToken().type != expectedType)
    return ThrowError(atTPE_UnexpectedToken);
  return EatToken();
}

bool atTrioParser::EatToken()
{
  if (!EndStatement())
    m_context.curStatement.push_back(GetCurrentToken());
  return NextToken();
}

bool atTrioParser::NextToken() { return ++m_context.currentToken < m_lexer.Tokens().size(); }

bool atTrioParser::EndStatement()
{
  const atTrioLexer::Token &token = GetCurrentToken();
  if (token.type != atTL_Token_Symbol || (token.id != atTL_Smbl_Semicolon))
    return false;
  atTrioParserNode newNode = m_expressionBuilder.Build(m_context.curStatement);
  m_context.pNode->AddParameter(std::move(newNode));
  m_context.curStatement.clear();
  return true;
}

bool atTrioParser::ThrowError(const atTrioParserError &error)
{
  return true;
}

const atTrioLexer::Token& atTrioParser::GetCurrentToken() { return GetToken(m_context.currentToken); }
const atTrioLexer::Token& atTrioParser::GetToken(const int64_t &index) { return m_lexer.Tokens()[index]; }

void atTrioParser::Clear()
{
  m_root = atTrioParserNode(atTP_NT_Block);
  m_errors.clear();
  m_context.currentToken = 0;
  m_context.nextTokenID = 0;
  m_context.pNode = &m_root;
}

const atVector<atTrioResult>& atTrioParser::Errors() { return m_errors; }
