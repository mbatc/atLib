#ifndef atTrioParserNode_h__
#define atTrioParserNode_h__

#include "atTrioLexer.h"

enum atTrioParserNodeType
{
  atTP_NT_None,

  // Structures
  atTP_NT_Block,
  atTP_NT_Statement,
  atTP_NT_Declaration,

  // Expressions
  atTP_NT_Variable,
  atTP_NT_Value,
  atTP_NT_Op,
  atTP_NT_BinOp,

  atTP_NT_Add,    // Arithmetic
  atTP_NT_Sub,
  atTP_NT_Mul,
  atTP_NT_Div,
  atTP_NT_Mod,
  atTP_NT_Assign, // Comparison
  atTP_NT_Equal,
  atTP_NT_NotEqual,
  atTP_NT_LessThan,
  atTP_NT_MoreThan,

  // Control Flow
  atTP_NT_Conditional,
  
  atTP_NT_Count
};

enum atTrioParserExpression
{
  atTP_Exp_Variable,
  atTP_Exp_Value,
  atTP_Exp_Op,
  atTP_Exp_BinOp,
  atTP_Exp_Add,
  atTP_Exp_Sub,
  atTP_Exp_Div,
  atTP_Exp_Mul,
  atTP_Exp_Mod,
};

class atTrioParserNode
{
public:
  atTrioParserNode(const atTrioLexer::Token &token);
  atTrioParserNode(const atTrioParserNodeType &type = atTP_NT_None);

  atTrioParserNode(atTrioParserNode &&o);
  atTrioParserNode(const atTrioParserNode &o);

  atTrioParserNode& operator=(atTrioParserNode &&o);
  atTrioParserNode& operator=(const atTrioParserNode &o);

  void SetType(const atTrioParserNodeType &type);

  void AddParameter(atTrioParserNode &&node);
  void AddParameter(const atTrioParserNode &node);
  void AddParameter(const atTrioLexer::Token &token);

  void RemoveParameter(const int64_t &index);

  const atTrioParserNodeType& Type() const;
  const atVector<atTrioParserNode>& Parameter();
  const atTrioLexer::Token& Token() const;

protected:
  atTrioLexer::Token m_token;
  atTrioParserNodeType m_type;
  atVector<atTrioParserNode> m_params;
};

#endif // atTrioParserNode_h__
