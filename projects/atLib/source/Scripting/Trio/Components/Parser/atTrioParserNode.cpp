#include "atTrioParserNode.h"

atTrioParserNode::atTrioParserNode(const atTrioLexer::Token &token)
  : m_token(token)
{
  switch (token.type)
  {
  case atTL_Token_Literal: m_type = atTP_NT_Value; break;
  case atTL_Token_Symbol: m_type = atTrioParserNodeType(atTP_NT_Add + token.id); break;
  case atTL_Token_Identifier: m_type = atTP_NT_Variable; break;
  }
}

atTrioParserNode::atTrioParserNode(const atTrioParserNodeType &type)
  : m_type(type)
{}

atTrioParserNode::atTrioParserNode(atTrioParserNode &&o)
{
  m_params = std::move(o.m_params);
  m_token = std::move(o.m_token);
  m_type = std::move(o.m_type);
}

atTrioParserNode::atTrioParserNode(const atTrioParserNode &o)
{
  m_params = o.m_params;
  m_token = o.m_token;
  m_type = o.m_type;
}

atTrioParserNode& atTrioParserNode::operator=(const atTrioParserNode &o)
{
  m_params = o.m_params;
  m_token = o.m_token;
  m_type = o.m_type;
  return *this;
}

void atTrioParserNode::SetType(const atTrioParserNodeType &type) { m_type = type; }

atTrioParserNode& atTrioParserNode::operator=(atTrioParserNode &&o)
{
  m_params = std::move(o.m_params);
  m_token = std::move(o.m_token);
  m_type = std::move(o.m_type);
  return *this;
}

void atTrioParserNode::AddParameter(atTrioParserNode &&node) { m_params.emplace_back(std::move(node)); }
void atTrioParserNode::AddParameter(const atTrioParserNode &node) { m_params.push_back(node); }
void atTrioParserNode::AddParameter(const atTrioLexer::Token &token) { return AddParameter(atTrioParserNode(token)); }
void atTrioParserNode::RemoveParameter(const int64_t &index) { m_params.erase(index); }
const atTrioLexer::Token& atTrioParserNode::Token() const { return m_token; }
const atTrioParserNodeType& atTrioParserNode::Type() const { return m_type; }
const atVector<atTrioParserNode>& atTrioParserNode::Parameter() { return m_params; }
