#ifndef atTrioLexer_h__
#define atTrioLexer_h__

#include "atString.h"
#include "atTrioLang.h"

class atTrioLexer
{
public:
  struct Token
  {
    int64_t id = -1;
    void *pValue = nullptr;
    atString name = "";
    atTrioLangToken type = atTL_Token_Unknown;
  };

  atTrioLexer(const atString &script = "");

  void Tokenize(const atString &script);
  
  const atVector<Token>& Tokens();

  static Token Identify(const atString &name);
  
protected:
  static bool GetNextToken(const char **ppScript, atString *pToken);

  atVector<Token> m_tokens;
};

#endif // atTrioLexer_h__
