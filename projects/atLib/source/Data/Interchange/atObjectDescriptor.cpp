#include "atObjectDescriptor.h"

static atString _objectTypeName[atObjectDescriptor::OT_Count] =
{
  "null",
  "object",
  "array",
  "value"
};

static atString _valueTypeName[atObjectDescriptor::VT_Count] =
{
  "none",
  "string",
  "bool",
  "int",
  "float"
};

static atObjectDescriptor::ValueType _GetValueType(const atString &name)
{
  for (int64_t i = 0; i < atObjectDescriptor::VT_Count; ++i)
    if (_valueTypeName[i].compare(name, atSCO_None))
      return atObjectDescriptor::ValueType(i);
  return atObjectDescriptor::VT_None;
}

static atObjectDescriptor::ObjectType _GetObjectType(const atString &name)
{
  for (int64_t i = 0; i < atObjectDescriptor::OT_Count; ++i)
    if (_objectTypeName[i].compare(name, atSCO_None))
      return atObjectDescriptor::ObjectType(i);
  return atObjectDescriptor::OT_Value;
}

atObjectDescriptor &atObjectDescriptor::operator=(const atObjectDescriptor &copy)
{
  if (&copy == this)
    return *this;

  ReleaseTree();
  AquireNode(copy.m_pTree, copy.m_nodeID);
  return *this;
}

atObjectDescriptor &atObjectDescriptor::operator=(atObjectDescriptor &&move)
{
  if (&move == this)
    return *this;

  ReleaseTree();
  AquireNode(move.m_pTree, move.m_nodeID);
  move.ReleaseTree();
  return *this;
}

atObjectDescriptor::atObjectDescriptor(const atObjectDescriptor &copy)
{
  *this = copy;
}

atObjectDescriptor::atObjectDescriptor(const atXML &xml) { Import(xml); }
atObjectDescriptor::atObjectDescriptor(const atJSON &json) { Import(json); }

atObjectDescriptor::atObjectDescriptor(atObjectDescriptor &&move) { *this = std::move(move); }

atObjectDescriptor::atObjectDescriptor(const ObjectType &type /*= OT_Value*/)
  : m_pTree(atNew<NodeTree>())
{
  m_nodeID = m_pTree->nodes.emplace();
  SetType(type);
}

atObjectDescriptor::atObjectDescriptor(const atString &name, const ObjectType &type, NodeTree *pTree)
{
  NodeData nodeData;
  nodeData.type = type;
  nodeData.name = name;
  int64_t nodeID = pTree->nodes.Add(nodeData);
  AquireNode(pTree, nodeID);
}

atObjectDescriptor::atObjectDescriptor(NodeTree *pTree, int64_t nodeID /*= AT_INVALID_ID*/) { AquireNode(pTree, nodeID); }
atObjectDescriptor::~atObjectDescriptor() { ReleaseTree(); }

void atObjectDescriptor::Import(const atJSON &json)
{
  if (json.IsArray())
  {
    SetType(OT_Object);
    for (const atJSON &member : json.Array())
      Add("").Import(member);
  }
  else if (json.IsObject())
  {
    SetType(OT_Object);
    for (const atKeyValue<atString, atJSON> &member : json.Object())
      Add(member.m_key).Import(member.m_val);
  }
  else if (json.IsValue())
  {
    if (json.IsString())
      Set(json.Value());
    else
      SetFromString(json.Value());
  }
  else if (json.IsNull())
  {
    SetType(OT_Null);
  }
}

void atObjectDescriptor::Import(const atXML &xml)
{
  if (xml.GetChildCount() > 0)
  { // Object or array
    ObjectType ot = _GetObjectType(xml.GetAttribute("type"));
    if (ot != OT_Object && ot != OT_Array)
      ot = OT_Object;

    SetType(ot);
    for (int64_t i = 0; i < xml.GetChildCount(); ++i)
    {
      const atXML *pXml = xml.GetChild(i);
      Add(ot == OT_Array ? "" : pXml->GetTag()).Import(*pXml);
    }
  }
  else
  {
    ValueType vt = _GetValueType(xml.GetAttribute("data-type"));
    atString value = xml.AsString();
    if (vt == VT_None)
    {
      if (value.length() > 0)
        SetFromString(value);
      else
        SetType(OT_Null);
    }
    else
    {
      SetValue(value, vt);
    }
  }
}

void atObjectDescriptor::Export(atJSON *pJSON) const
{
  ObjectType type = GetObjectType();
  switch (type)
  {
  case OT_Value:
    pJSON->SetValue(AsString(), GetValueType() == VT_String);
    break;
  case OT_Array:
    pJSON->MakeArray();
    for (int64_t i = 0; i < GetMemberCount(); ++i)
    {
      atJSON childJson;
      Get(i).Export(&childJson);
      pJSON->SetElement(i, childJson);
    }
    break;
  case OT_Object:
    pJSON->MakeObject();
    for (const atObjectDescriptor &member : GetMembers())
    {
      atJSON childJson;
      member.Export(&childJson);
      pJSON->SetMember(member.GetName(), childJson);
    }
    break;
  }
}

void atObjectDescriptor::Export(atXML *pXML) const
{
  ObjectType type = GetObjectType();
  if (type != OT_Value)
    pXML->SetAttribute("type", _objectTypeName[type]);
 
  switch (type)
  {
  case OT_Value:
    pXML->SetAttribute("data-type", _valueTypeName[GetValueType()]);
    pXML->Set(AsString());
  break;
  case OT_Array:
    pXML->SetAttribute("count", atString(GetMemberCount()));
    for (const atObjectDescriptor &member : GetMembers())
    {
      atXML childXml;
      childXml.SetTag("i");
      member.Export(&childXml);
      pXML->AddChild(childXml);
    }
    break;
  case OT_Object:
    for (const atObjectDescriptor &member : GetMembers())
    {
      atXML childXml;
      childXml.SetTag(member.GetName());
      member.Export(&childXml);
      pXML->AddChild(childXml);
    }
    break;
  }
  atUnused(pXML);
}

atString atObjectDescriptor::GetName() const { return GetNode().name; }

void atObjectDescriptor::SetType(const ObjectType &type)
{
  NodeData &data = GetNode();
  if (data.type == type)
    return;
  Clear();
  data.type = type;
}

atObjectDescriptor atObjectDescriptor::Add(const atString &name, const ObjectType &type /*= OT_Value*/)
{
  if (GetObjectType() == OT_Value)
    SetType(name.length() > 0 ? OT_Object : OT_Array);
  int64_t idx = Find(name);
  if (idx >= 0)
  {
    atObjectDescriptor o = Get(idx);
    o.SetType(type);
    return o;
  }

  atObjectDescriptor o(name, type, m_pTree);
  GetNode().children.push_back(o.m_nodeID);
  return o;
}

atObjectDescriptor atObjectDescriptor::Add(const atObjectDescriptor &obj) { return Add(obj.GetName(), obj.GetObjectType()).Set(obj); }

atObjectDescriptor atObjectDescriptor::Set(const atString &value) { return SetValue(value, VT_String); }
atObjectDescriptor atObjectDescriptor::Set(const double &value) { return SetValue(atString(value), VT_Float); }
atObjectDescriptor atObjectDescriptor::Set(const int64_t &value) { return SetValue(atString(value), VT_Int); }
atObjectDescriptor atObjectDescriptor::Set(const bool &value) { return SetValue(value ? "true" : "false", VT_Bool); }

atObjectDescriptor atObjectDescriptor::Set(const atObjectDescriptor &value)
{
  SetType(value.GetObjectType());
  if (IsValue())
  {
    SetValue(value.AsString(), value.GetValueType());
  }
  else
  {
    for (const atObjectDescriptor &member : value.GetMembers())
      Add(member);
  }

  return *this;
}

atObjectDescriptor atObjectDescriptor::SetFromString(const atString &value)
{
  int64_t len = 0;

  // Try Int
  atScan::Int(value, &len);
  if (len == value.length())
    return SetValue(value, VT_Int);
  
  // Try as float
  atScan::Float(value, &len);
  if (len == value.length())
    return SetValue(value, VT_Float);

  // Try as bool
  atScan::Bool(value, &len);
  if (len == value.length())
    return SetValue(value, VT_Bool);

  // Default to string
  return Set(value);
}

atObjectDescriptor atObjectDescriptor::Get(const int64_t &index) const
{
  const NodeData &node = GetNode();
  if (index < 0 || index >= node.children.size())
    return atObjectDescriptor();
  return atObjectDescriptor(m_pTree, node.children[index]);
}

atObjectDescriptor atObjectDescriptor::GetOrAdd(const atString &name)
{
  int64_t idx = Find(name);
  return idx == AT_INVALID_ID ? Add(name) : Get(idx);
}

atObjectDescriptor atObjectDescriptor::Get(const atString &name) const { return Get(Find(name)); }

bool atObjectDescriptor::Remove(const atString &name)
{
  return Remove(Find(name));
}

bool atObjectDescriptor::Remove(const int64_t &index)
{
  if (index < 0 || index >= GetMemberCount())
    return false;
  atObjectDescriptor child = Get(index);
  child.Clear();

  int64_t id = child.m_nodeID;
  child.ReleaseTree();
  m_pTree->nodes.erase(id);
  return true;
}

int64_t atObjectDescriptor::Find(const atString &name) const
{
  for (int64_t i = 0; i < GetMemberCount(); ++i)
    if (Get(i).GetName() == name)
      return i;
  return -1;
}

int64_t atObjectDescriptor::GetMemberCount() const
{
  return GetNode().children.size();
}

atVector<atString> atObjectDescriptor::GetMemberNames() const
{
  atVector<atString> names;
  for (int64_t i = 0; i < GetMemberCount(); ++i)
    names.push_back(Get(i).GetName());
  return names;
}

atVector<atObjectDescriptor> atObjectDescriptor::GetMembers() const
{
  atVector<atObjectDescriptor> members;
  for (int64_t i = 0; i < GetMemberCount(); ++i)
    members.push_back(Get(i));
  return members;
}

bool atObjectDescriptor::AsBool() const { return atScan::Bool(GetNode().value); }
int64_t atObjectDescriptor::AsInt() const { return atScan::Int(GetNode().value); }
double atObjectDescriptor::AsFloat() const { return atScan::Float(GetNode().value); }
atString atObjectDescriptor::AsString() const { return GetNode().value; }

bool atObjectDescriptor::IsNull() const { return GetObjectType() == OT_Null; }
bool atObjectDescriptor::IsValue() const { return GetObjectType() == OT_Value; }
bool atObjectDescriptor::IsArray() const { return GetObjectType() == OT_Array; }
bool atObjectDescriptor::IsObject() const { return GetObjectType() == OT_Object; }

const atObjectDescriptor::ValueType &atObjectDescriptor::GetValueType() const { return GetNode().valueType; }
const atObjectDescriptor::ObjectType &atObjectDescriptor::GetObjectType() const { return GetNode().type; }

void atObjectDescriptor::Clear()
{
  while (int64_t count = GetMemberCount() > 0)
    Remove(count - 1);

  NodeData &node = GetNode();
  node.value = "";
  node.type = OT_Null;
  node.valueType = VT_None;
}

atObjectDescriptor atObjectDescriptor::operator[](const atString &name) const { return Get(name); }

bool atObjectDescriptor::operator==(const atObjectDescriptor &rhs) const
{
  if (GetObjectType() != rhs.GetObjectType()) return false;
  if (GetValueType() != rhs.GetValueType()) return false;
  if (GetMemberCount() != rhs.GetMemberCount()) return false;

  if (IsArray())
  { // We care about the order if it is an array
    for (int64_t i = 0; i < GetMemberCount(); ++i)
      if (Get(i) != rhs.Get(i))
        return false;
  }
  else if (IsObject())
  { // We care about the names if it is an object
    for (const atString &member : GetMemberNames())
      if (Get(member) != rhs.Get(member))
        return false;
  }
  else if (AsString() != rhs.AsString())
      return false;
  return true;
}

bool atObjectDescriptor::operator!=(const atObjectDescriptor &rhs) const { return !(*this == rhs); }

atObjectDescriptor atObjectDescriptor::operator[](const int64_t &idx) const { return Get(idx); }

atObjectDescriptor atObjectDescriptor::operator[](const atString &name) { return Get(name); }
atObjectDescriptor atObjectDescriptor::operator[](const int64_t &idx) { return Get(idx); }

atObjectDescriptor atObjectDescriptor::SetValue(const atString &value, ValueType type)
{
  SetType(OT_Value);
  NodeData &node = GetNode();
  node.value = value;
  node.valueType = type;
  return *this;
}

const atObjectDescriptor::NodeData &atObjectDescriptor::GetNode() const { return m_pTree->nodes[m_nodeID]; }
atObjectDescriptor::NodeData &atObjectDescriptor::GetNode() { return m_pTree->nodes[m_nodeID]; }

void atObjectDescriptor::AquireNode(NodeTree *pTree, const int64_t &nodeID)
{
  m_nodeID = AT_INVALID_ID;
  if (!pTree)
    return;

  ++pTree->refCount;
  m_pTree = pTree;
  m_nodeID = nodeID;
}

void atObjectDescriptor::ReleaseTree()
{
  if (!m_pTree)
    return;

  if (--m_pTree->refCount == 0)
    atDelete(m_pTree);

  m_pTree = nullptr;
  m_nodeID = AT_INVALID_ID;
}

int64_t atStreamRead(atReadStream *pStream, atObjectDescriptor *pData, const int64_t count)
{
  int64_t size = 0;
  for (int64_t i = 0; i < count; ++i)
  {
    pData[i].Clear();
    atObjectDescriptor::NodeData &node = pData[i].GetNode();
    size += atStreamRead(pStream, &node.name, 1);
    size += atStreamRead(pStream, &node.type, 1);
    size += atStreamRead(pStream, &node.valueType, 1);
    size += atStreamRead(pStream, &node.value, 1);

    int64_t childCount = 0;
    size += atStreamRead(pStream, &childCount, 1);
    for (int64_t i = 0; i < childCount; ++i)
    {
      atObjectDescriptor child = pData[i].Add("");
      atStreamRead(pStream, &child, 1);
    }
  }
  return size;
}

int64_t atStreamWrite(atWriteStream *pStream, atObjectDescriptor *pData, const int64_t count)
{
  int64_t size = 0;
  for (int64_t i = 0; i < count; ++i)
  {
    atObjectDescriptor::NodeData &node = pData[i].GetNode();
    size += atStreamWrite(pStream, &node.name, 1);
    size += atStreamWrite(pStream, &node.type, 1);
    size += atStreamWrite(pStream, &node.valueType, 1);
    size += atStreamWrite(pStream, &node.value, 1);

    size += atStreamWrite(pStream, &node.children.size(), 1);
    for (int64_t i = 0; i < pData[i].GetMemberCount(); ++i)
    {
      atObjectDescriptor child = pData[i].Get(i);
      size += atStreamWrite(pStream, &child, 1);
    }
  }

  return size;
}
