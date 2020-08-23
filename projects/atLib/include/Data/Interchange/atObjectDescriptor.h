#ifndef atObjectDescriptor_h__
#define atObjectDescriptor_h__

#include "atXML.h"
#include "atJSON.h"
#include "atPool.h"
#include "atScan.h"

class atObjectDescriptor
{
public:
  enum ObjectType
  {
    OT_Null,
    OT_Object,
    OT_Array,
    OT_Value,
    OT_Count
  };

  enum ValueType
  {
    VT_None,
    VT_String,
    VT_Bool,
    VT_Int,
    VT_Float,
    VT_Count
  };

protected:
  struct NodeData
  {
    atString name;
    atString value;
    atVector<int64_t> children;

    ObjectType type = OT_Null;
    ValueType valueType = VT_None;
  };

  struct NodeTree
  {
    int64_t refCount = 1;
    atPool<NodeData> nodes;
  };

  atObjectDescriptor(NodeTree *pTree, int64_t nodeID = AT_INVALID_ID);

  atObjectDescriptor(const atString &name, const ObjectType &type, NodeTree *pTree);

public:
  atObjectDescriptor(const ObjectType &type = OT_Value);

  atObjectDescriptor(atObjectDescriptor &&move);
  atObjectDescriptor(const atObjectDescriptor &copy);

  atObjectDescriptor(const atXML &xml);
  atObjectDescriptor(const atJSON &json);
  
  template<typename T> explicit atObjectDescriptor(const T &o);
  template<typename T> atObjectDescriptor operator=(const T &o);

  atObjectDescriptor &operator=(atObjectDescriptor &&move);
  atObjectDescriptor &operator=(const atObjectDescriptor &copy);


  ~atObjectDescriptor();

  void Import(const atXML &xml);
  void Import(const atJSON &json);

  void Export(atXML *pXML) const;
  void Export(atJSON *pJSON) const;

  atString GetName() const;

  void SetType(const ObjectType &type);

  atObjectDescriptor Add(const atString &name, const ObjectType &type = OT_Value);
  atObjectDescriptor Add(const atObjectDescriptor &obj);

  atObjectDescriptor Set(const bool &value);
  atObjectDescriptor Set(const int64_t &value);
  atObjectDescriptor Set(const double &value);
  atObjectDescriptor Set(const atString &value);
  atObjectDescriptor Set(const atObjectDescriptor &value);
  atObjectDescriptor SetFromString(const atString &value);

  atObjectDescriptor Get(const atString &name) const;
  atObjectDescriptor Get(const int64_t &index) const;
  atObjectDescriptor GetOrAdd(const atString &name);

  bool Remove(const int64_t &index);
  bool Remove(const atString &name);

  int64_t Find(const atString &name) const;

  int64_t GetMemberCount() const;
  atVector<atString> GetMemberNames() const;
  atVector<atObjectDescriptor> GetMembers() const;

  bool AsBool() const;
  int64_t AsInt() const;
  double AsFloat() const;
  atString AsString() const;

  bool IsNull() const;
  bool IsValue() const;
  bool IsArray() const;
  bool IsObject() const;

  const ValueType& GetValueType() const;
  const ObjectType& GetObjectType() const;

  void Clear();

  // Serialize 'value' into this object descriptor
  template<typename T> atObjectDescriptor Serialize(const T &value);

  // Deserialize this object descriptor into 'pValue'
  template<typename T> T Deserialize() const;
  template<typename T> void Deserialize(T *pValue) const;

  atObjectDescriptor operator[](const int64_t &idx);
  atObjectDescriptor operator[](const atString &name);

  atObjectDescriptor operator[](const int64_t &idx) const;
  atObjectDescriptor operator[](const atString &name) const;

  bool operator==(const atObjectDescriptor &rhs) const;
  bool operator!=(const atObjectDescriptor &rhs) const;

  friend int64_t atStreamRead(atReadStream *pStream, atObjectDescriptor *pData, const int64_t count);
  friend int64_t atStreamWrite(atWriteStream *pStream, atObjectDescriptor *pData, const int64_t count);

protected:
  atObjectDescriptor SetValue(const atString &value, ValueType type);

  NodeData& GetNode();
  const NodeData& GetNode() const;

  void AquireNode(NodeTree *pTree, const int64_t &nodeID);

  void ReleaseTree();

  int64_t m_nodeID = AT_INVALID_ID;
  NodeTree *m_pTree = nullptr;
};

atTrivialStreamRead(atObjectDescriptor::ObjectType);
atTrivialStreamRead(atObjectDescriptor::ValueType);
atTrivialStreamWrite(atObjectDescriptor::ObjectType);
atTrivialStreamWrite(atObjectDescriptor::ValueType);

#include "atObjectDescriptor.inl"
#endif // atObjectDescriptor_h__
