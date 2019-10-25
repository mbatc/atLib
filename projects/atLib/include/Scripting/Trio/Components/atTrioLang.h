#ifndef atTrioLang_h__
#define atTrioLang_h__

#include "atString.h"

enum atTrioLangKeyword
{
  atTL_KW_If,
  atTL_KW_Else,
  atTL_KW_Return,
  atTL_KW_Count,
  atTL_KW_Unknown
};

enum atTrioLangSymbol
{
  atTL_Smbl_Add,
  atTL_Smbl_Sub,
  atTL_Smbl_Mul,
  atTL_Smbl_Div,
  atTL_Smbl_Mod,
  atTL_Smbl_Assign,
  atTL_Smbl_Equal,
  atTL_Smbl_NotEqual,
  atTL_Smbl_LessThan,
  atTL_Smbl_MoreThan,

  atTL_Smbl_LPar,
  atTL_Smbl_RPar,
  atTL_Smbl_LBrace,
  atTL_Smbl_RBrace,
  atTL_Smbl_LBoxBrace,
  atTL_Smbl_RBoxBrace,
  atTL_Smbl_Semicolon,
  atTL_Smbl_Comma,
  atTL_Smbl_Dot,

  atTL_Smbl_Count,
  atTL_Smbl_Unknown
};

enum atTrioLangLiteral
{
  atTL_LT_True,
  atTL_LT_False,
  atTL_LT_Decimal,
  atTL_LT_Integer,
  atTL_LT_Count,
  atTL_LT_Unknown
};

enum atTrioLangType
{
  atTL_Type_Int,
  atTL_Type_Float,
  atTL_Type_Char,
  atTL_Type_Count,
  atTL_Type_Unknown
};

enum atTrioLangToken
{
  atTL_Token_Keyword,
  atTL_Token_Literal,
  atTL_Token_Identifier,
  atTL_Token_Symbol,
  atTL_Token_Count,
  atTL_Token_Unknown
};

class atTrioLang
{
public:
  static const atString symbol[atTL_Smbl_Count];
  static const atString keyword[atTL_KW_Count];
  static const atString literals[atTL_LT_Count];
  static const atString types[atTL_Type_Count];

  static atTrioLangKeyword KeywordID(const atString &name);
  static atTrioLangLiteral LiteralID(const atString &name);
  static atTrioLangSymbol SymbolID(const atString &name);
  static atTrioLangType TypeID(const atString &name);
};

#endif // atTrioLang_h__
