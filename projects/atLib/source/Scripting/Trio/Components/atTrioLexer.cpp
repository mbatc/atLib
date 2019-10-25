#include "atTrioLexer.h"
#include "atScan.h"

atTrioLexer::atTrioLexer(const atString &script) { if (script.length()) Tokenize(script); }

void atTrioLexer::Tokenize(const atString &script)
{
  const char *start = script.c_str();
  atString token;
  while (GetNextToken(&start, &token))
    m_tokens.push_back(Identify(token));
}

atTrioLexer::Token atTrioLexer::Identify(const atString &name)
{
  Token t;
  t.id = atTrioLang::KeywordID(name);
  if (t.id != atTL_KW_Unknown)
  {
    t.type = atTL_Token_Keyword;
    return t;
  }

  t.id = atTrioLang::SymbolID(name);
  if (t.id != atTL_Smbl_Unknown)
  {
    t.type = atTL_Token_Symbol;
    return t;
  }

  for (int64_t i = 0; i < atTL_LT_Count; ++i)
  {
    if (name == atTrioLang::literals[i])
    {
      t.id = i;
      t.type = atTL_Token_Literal;
      return t;
    }
    else if (i == atTL_LT_Integer)
    {
      int64_t len = 0;
      int64_t val = atScan::Int(name, &len);
      if (len == 0 || name.find('.') >= 0)
        continue;
      t.id = i;
      t.name = name;
      t.type = atTL_Token_Literal;
      memcpy(&t.pValue, &val, atMin(sizeof(void*), sizeof(int64_t)));
      return t;
    }
    else if (i == atTL_LT_Decimal)
    {
      int64_t len = 0;
      double val = atScan::Float(name, &len);
      if (len == 0 || name.find('.') < 0)
        continue;
      t.id = i;
      t.name = name;
      t.type = atTL_Token_Literal;
      memcpy(&t.pValue, &val, atMin(sizeof(void*), sizeof(double)));
      return t;
    }
  }

  // Return as an identifier
  t.id = -1;
  t.type = atTL_Token_Identifier;
  t.name = name;
  return t;
}
bool atTrioLexer::GetNextToken(const char **ppScript, atString *pToken)
{
  if (**ppScript == 0)
    return false;

  *pToken = "";
  int64_t res = atString::_find_first_not(*ppScript, atString::Whitespace());
  if (res < 0)
    return false;

  *ppScript += res;

  bool isNumber = true;
  bool isDecimal = false;
  bool exit = false;
  while (!exit && atString::_find_first_of(atString::Whitespace(), **ppScript) && **ppScript != 0)
  {
    bool isDecimal = isNumber && atString::_starts_with(*ppScript, atTrioLang::symbol[atTL_Smbl_Dot]);
    isNumber &= (atString::_find_first_of(atString::Numerals(), **ppScript) != AT_INVALID_INDEX) || isDecimal;

    // Check for symbols that will separate tokens
    for (int64_t smblID = 0; smblID < atTL_Smbl_Count; ++smblID)
      if ((smblID != atTL_Smbl_Dot || !isDecimal) && atString::_starts_with(*ppScript, atTrioLang::symbol[smblID]))
      {
        exit = true;
        break;
      }

    if (!exit || pToken->length() == 0)
      *pToken += *((*ppScript)++);
  }

  return pToken->length() != 0;
}

const atVector<atTrioLexer::Token>& atTrioLexer::Tokens() { return m_tokens; }
