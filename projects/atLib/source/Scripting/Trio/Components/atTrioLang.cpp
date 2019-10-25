#include "atTrioLang.h"

const atString atTrioLang::symbol[atTL_Smbl_Count] =
{
  "+",
  "-",
  "*",
  "/",
  "%",
  "=",
  "==",
  "!=",
  "<",
  ">",
  "(",
  ")",
  "{",
  "}",
  "[",
  "]",
  ";",
  ",",
  ".",
};

const atString atTrioLang::literals[atTL_LT_Count] =
{
  "true",
  "false",
};

const atString atTrioLang::keyword[atTL_KW_Count] =
{
  "if",
  "else",
  "return",
};

const atString atTrioLang::types[atTL_Type_Count] =
{
  "int",
  "float",
  "char",
};

int64_t _FindID(const atString &name, const atString *pNames, const int64_t &count, const int64_t &invalidResult)
{
  for (int64_t i = 0; i < count; ++i)
    if (pNames[i] == name)
      return i;
  return invalidResult;
}

atTrioLangKeyword atTrioLang::KeywordID(const atString &name) { return atTrioLangKeyword(_FindID(name, keyword, atTL_KW_Count, atTL_KW_Unknown)); }
atTrioLangLiteral atTrioLang::LiteralID(const atString &name) { return atTrioLangLiteral(_FindID(name, literals, atTL_LT_Count, atTL_LT_Unknown)); }
atTrioLangSymbol atTrioLang::SymbolID(const atString &name) { return atTrioLangSymbol(_FindID(name, symbol, atTL_Smbl_Count, atTL_Smbl_Unknown)); }
atTrioLangType atTrioLang::TypeID(const atString &name) { return atTrioLangType(_FindID(name, types, atTL_Type_Count, atTL_Type_Unknown)); }
