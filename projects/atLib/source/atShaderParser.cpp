
// -----------------------------------------------------------------------------
// The MIT License
// 
// Copyright(c) 2018 Michael Batchelor, 
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// -----------------------------------------------------------------------------

#include "atShaderParser.h"
#include "atScan.h"
#include "atFile.h"

static int64_t _ParseDataLayout(atHLSLBufferDesc *pLayout, const char *src, int64_t len, atVector<atVec4I64> *pUsedRegisters);

static void _AddInputStruct(atHashMap<atString, atTypeDesc> *pInputs, const atHLSLBufferDesc &data)
{
  for (const atString &var : data.packingOrder)
  {
    const atTypeDesc *pType = data.members.TryGet(var);
    const atString *pSem = data.semantics.TryGet(var);
    const atHLSLBufferDesc *pStruct = data.structs.TryGet(var);

    if (pStruct)
      _AddInputStruct(pInputs, *pStruct);
    else if (pType && pSem && pSem->length() != 0)
      pInputs->Add(*pSem, *pType);
  }
}

static int64_t _ReadVariable(atHashMap<atString, atTypeDesc> *pList, atHashMap<atString, atString> *pSem, const char *src, int64_t len)
{
  int64_t offset = atString::_find_first_not(src, len, atString::Whitespace());
  int64_t dist = 0;
  atString token = atScan::String(src + offset, &dist);

  int64_t openBracket = token.find_first_of("(");
  offset += openBracket > 0 ? openBracket + (dist - token.length()) : dist;
  token = token.replace(";", " ");
  token = token.replace(",", " ");
  token = atScan::String(token);

  atTypeDesc desc = atShaderParser::HLSLTypeDesc(token);
  
  if (desc.type != atType_Unknown)
  {
    atString name = atScan::String(src + offset, &dist).replace(";","").replace(",","").replace(")", "");
    int64_t colon = atString::_find(src, len, ':', offset);
    int64_t semicolon = atString::_find(src, len, ';', offset);
    int64_t comma = atString::_find(src, len, ',', offset);
    pList->Add(name, desc);
    pSem->Add(name);

    if (colon > 0 && (colon < semicolon || comma < semicolon))
      pSem->Get(name) = atScan::String(src + colon + 1, &dist).replace(";", "").replace(",", "");

    offset = atMin(comma < 0 ? semicolon : comma, semicolon) + 1;
  }

  return atMin(offset + 1, len);
}

static int64_t _ReadStruct(const atString &typeName, atHashMap<atString, atHLSLBufferDesc> *pList, const char *src, int64_t len, atVector<atVec4I64> *pUsedRegisters)
{
  int64_t offset = atString::_find_first_not(src, len, atString::Whitespace());
  atString name(src + offset, src + atString::_find_first_of(src, len, atString::Whitespace(), offset));
  pList->Add(name, atHLSLBufferDesc());
  atHLSLBufferDesc *pDesc = pList->TryGet(name);

  pDesc->name = name;
  pDesc->type = atHLSLBT_Struct;
  if (typeName == "cbuffer")
    pDesc->type = atHLSLBT_Const;
  else if (typeName == "tbuffer")
    pDesc->type = atHLSLBT_Tex;

  offset += name.length();

  int64_t colon = atString::_find_first_of(src, len, ':', offset);
  offset = atString::_find_first_of(src, len, '{', offset) + 1;

  atVec4I64 &used = pUsedRegisters->at(pDesc->type);
  if (colon < offset && colon != AT_INVALID_INDEX)
  {
    atVector<atString> regDef = atString(src + colon + 1, src + offset).split(" \n{()", true);
    if (regDef.size() > 1 && regDef[0] == "register")
    {
      pDesc->reg = atScan::Int(regDef[1].substr(regDef[1].find_first_of(atString::Numerals()), regDef[1].length()));
      atAssert((used[pDesc->reg / 64] & (1ll << pDesc->reg)) == 0, ("Buffer already bound to the register " + regDef[1]).c_str());
      used[pDesc->reg / 64] |= 1ll << pDesc->reg;
    }
  }

  offset += _ParseDataLayout(pDesc, src + offset, len - offset, pUsedRegisters);
  return atMin(offset + 1, len);
}

static int64_t _ParseDataLayout(atHLSLBufferDesc *pLayout, const char *src, int64_t len, atVector<atVec4I64> *pUsedRegisters)
{
  atString token;
  atString nextToken;
  int64_t offset = 0;
  while (offset < len)
  {
    nextToken = "";
    int64_t dist = 0;
    token = atScan::String(src + offset, &dist);
    if(dist < len)
      nextToken = atScan::String(src + offset + dist);

    if (token.find('}') != -1)
      break;

    int64_t structCount = pLayout->structs.Size();
    int64_t memberCount = pLayout->members.Size();

    if (token == "struct" || token == "cbuffer" || token == "tbuffer")
    {
      offset += dist;
      offset += _ReadStruct(token, &pLayout->structs, src + offset, len - offset, pUsedRegisters);
    }
    else
    {
      offset += _ReadVariable(&pLayout->members, &pLayout->semantics, src + offset, len - offset);
    }

    if (structCount != pLayout->structs.Size())
      pLayout->packingOrder.push_back(nextToken.replace(";", ""));
    if (memberCount != pLayout->members.Size())
      pLayout->packingOrder.push_back(nextToken.replace(";", ""));
  }

  int64_t packOffset = 0;

  for (atString vars : pLayout->packingOrder)
  {
    atTypeDesc *pType = pLayout->members.TryGet(vars);
    atHLSLBufferDesc *pStruct = pLayout->structs.TryGet(vars);
    const int64_t varWidth = pType ? pType->size : pStruct->actualSize;
    const int64_t packBytes = packOffset % 16;
    if (packBytes + varWidth > 16 && packBytes != 0)
      packOffset = (packOffset / 16 + 1) * 16;
    pLayout->packingOffsets.push_back(packOffset);
    packOffset += varWidth;
    pLayout->size += varWidth;
  }
  pLayout->actualSize = packOffset;

  return atMin(offset + 1, len);
}

atShaderParser::atShaderParser(const atFilename &file) : m_fn(file) { Parse(); }

atShaderParser::atShaderParser(const atShaderParser &copy)
  : m_fn(copy.m_fn)
  , m_cbuffers(copy.m_cbuffers)
  , m_tbuffers(copy.m_tbuffers)
  , m_inputs(copy.m_inputs)
  , m_src(copy.m_src)
{}

atShaderParser::atShaderParser(atShaderParser &&move)
  : m_fn(std::move(move.m_fn))
  , m_cbuffers(std::move(move.m_cbuffers))
  , m_tbuffers(std::move(move.m_tbuffers))
  , m_inputs(std::move(move.m_inputs))
  , m_src(std::move(move.m_src))
{}

bool atShaderParser::Parse()
{
  Clear();
  if (!ReadFile())
    return false;

  atVector<atVec4I64> usedReg(atHLSLBT_Count,  atVec4I64::zero());
  atHLSLBufferDesc dataLayout;
  _ParseDataLayout(&dataLayout, m_src.c_str(), m_src.length(), &usedReg);

  // Find main() and read inputs variable names/semantics
  int64_t ipStart = m_src.find_end(" main(");
  if (ipStart == 0)
    return false;
  int64_t ipEnd = m_src.find(')', ipStart);
  
  int64_t scanLen = 0;
  int64_t offset = ipStart;
  atString token;
  while (offset < ipEnd)
  {
    token = atScan::String(m_src.c_str() + offset, &scanLen).replace(",", "");
    offset += scanLen;
    if (dataLayout.members.Contains(token))
    {
      if (dataLayout.semantics[token].length() != 0)
        m_inputs.Add(dataLayout.semantics.Get(token), dataLayout.members.Get(token));
    }
    else if (dataLayout.structs.Contains(token))
      _AddInputStruct(&m_inputs, dataLayout.structs.Get(token));
  }

  bool hasGlobalBuffer = dataLayout.members.Size() != 0;
  int64_t cReg[atHLSLBT_Count] = { 0 };
  int64_t gReg = -1;
  if (hasGlobalBuffer)
  {
    gReg = 0;
    atVec4I64 &used = usedReg[atHLSLBT_Const];
    while ((used[gReg / 64] & (1ll << gReg)) > 0) ++gReg;
  }

  cReg[atHLSLBT_Const] = gReg + 1;

  for (auto& kvp : dataLayout.structs)
  {
    atVec4I64 &used = usedReg[kvp.m_val.type];
    if (kvp.m_val.reg < 0)
    {
      kvp.m_val.reg = cReg[kvp.m_val.type]++;
      while ((used[kvp.m_val.reg / 64] & (1ll << kvp.m_val.reg)) > 0) ++kvp.m_val.reg;
      used[kvp.m_val.reg / 64] |= (1ll << kvp.m_val.reg);
    }
    cReg[kvp.m_val.type] = kvp.m_val.reg + 1;

    if (kvp.m_val.type == atHLSLBT_Const)
      m_cbuffers.Add(kvp.m_val.reg, kvp.m_val);
    else if (kvp.m_val.type == atHLSLBT_Tex)
      m_tbuffers.Add(kvp.m_val.reg, kvp.m_val);

    for (int64_t i = 0; i < dataLayout.packingOrder.size(); ++i)
      if (dataLayout.packingOrder[i] == kvp.m_key)
      {
        dataLayout.packingOrder.erase(i);
        break;
      }
  }

  if (hasGlobalBuffer)
  {
    // Re-evalute packing offsets and size for global constant buffer (as structs have been removed)
    int64_t packOffset = 0;
    dataLayout.structs.Clear();
    dataLayout.packingOffsets.clear();
    for (atString vars : dataLayout.packingOrder)
    {
      atTypeDesc *pType = dataLayout.members.TryGet(vars);
      atHLSLBufferDesc *pStruct = dataLayout.structs.TryGet(vars);
      const int64_t varWidth = pType ? pType->size : pStruct->actualSize;
      const int64_t packBytes = packOffset % 16;
      if (packBytes + varWidth > 16 && packBytes != 0)
        packOffset = (packOffset / 16 + 1) * 16;
      dataLayout.packingOffsets.push_back(packOffset);
      packOffset += varWidth;
      dataLayout.size += varWidth;
    }
    dataLayout.actualSize = packOffset;
    dataLayout.reg = gReg;
    m_cbuffers.Add(gReg, dataLayout);
  }

  offset = 0;
  scanLen = 0;

  // Get texture locations
  int64_t location = 0;
  while ((offset = atString::_find(m_src.c_str(), m_src.length(), "Texture2D", offset + scanLen)) != AT_INVALID_INDEX)
    m_textures.Add(location++, atScan::String(m_src.c_str() + (offset += 9), &scanLen).replace(";", ""));
  // Get sample locations
  location = 0;
  while ((offset = atString::_find(m_src.c_str(), m_src.length(), "SamplerState", offset + scanLen)) != AT_INVALID_INDEX)
    m_samplers.Add(location++, atScan::String(m_src.c_str() + (offset += 12), &scanLen).replace(";", ""));

  return true;
}

void atShaderParser::Clear()
{
  m_inputs.Clear();
  m_cbuffers.Clear();
  m_tbuffers.Clear();
}

bool atShaderParser::ReadFile()
{
  atFile file;
  if (!file.Open(m_fn, atFM_Read))
    return false;
  atVector<char> data(file.Info().Size(), 0);
  file.Read(data.data(), data.size());
  m_src = data;
  return true;
}

atTypeDesc atShaderParser::HLSLTypeDesc(const atString &type)
{
  atTypeDesc desc(atType_Unknown, 1);
  if (type.find_first_of("(){}.,;'\"") != AT_INVALID_INDEX)
    return desc;

  if (type.find("float") == 0) desc.type = atType_Float32;
  else if (type.find("double") == 0) desc.type = atType_Float64;
  else if (type.find("int") == 0) desc.type = atType_Int32;
  else if (type.find("uint") == 0) desc.type = atType_Uint32;
  else if (type.find("dword") == 0) desc.type = atType_Uint32;

  if (desc.type == atType_Unknown)
    return desc;

  int64_t num = type.find_first_of(atString::Numerals());
  int64_t len = 0;
  if (num > 0)
    while (num < type.length())
    {
      num = type.find_first_of(atString::Numerals(), num);
      if (num < 0)
        break;

      desc.count *= atScan::Int(type.begin() + num, &len);
      num += atMin(1, len);
    }

  desc.size = desc.count * atSize(desc.type);
  return desc;
}

const atShaderParser& atShaderParser::operator=(const atShaderParser &rhs)
{
  m_cbuffers = rhs.m_cbuffers;
  m_tbuffers = rhs.m_tbuffers;
  m_inputs = rhs.m_inputs;
  m_fn = rhs.m_fn;
  m_src = rhs.m_src;
  m_textures = rhs.m_textures;
  m_samplers = rhs.m_samplers;
  return *this;
}
