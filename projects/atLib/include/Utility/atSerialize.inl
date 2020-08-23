
template<typename T> void atSerialize(atObjectDescriptor *pSerialized, const atVector2<T> &src)
{
  pSerialized->Add("x") = src.x;
  pSerialized->Add("y") = src.y;
}

template<typename T> void atSerialize(atObjectDescriptor *pSerialized, const atVector3<T> &src)
{
  pSerialized->Add("x") = src.x;
  pSerialized->Add("y") = src.y;
  pSerialized->Add("z") = src.z;
}

template<typename T> void atSerialize(atObjectDescriptor *pSerialized, const atVector4<T> &src)
{
  pSerialized->Add("x") = src.x;
  pSerialized->Add("y") = src.y;
  pSerialized->Add("z") = src.z;
  pSerialized->Add("w") = src.w;
}

template<typename T> void atSerialize(atObjectDescriptor *pSerialized, const atMatrix4x4<T> &src)
{
  pSerialized->SetType(atObjectDescriptor::OT_Array);
  atSerialize(pSerialized, &src.m, atArraySize(src.m));
}

template<typename T> void atSerialize(atObjectDescriptor *pSerialized, const atQuaternion<T> &src)
{
  pSerialized->Add("x") = src.x;
  pSerialized->Add("y") = src.y;
  pSerialized->Add("z") = src.z;
  pSerialized->Add("w") = src.w;
}

template<typename T> void atDeserialize(const atObjectDescriptor &serialized, atVector2<T> *pDst)
{
  serialized.Get("x").Deserialize(&pDst->x);
  serialized.Get("y").Deserialize(&pDst->y);
  serialized.Get("z").Deserialize(&pDst->z);
}

template<typename T> void atDeserialize(const atObjectDescriptor &serialized, atVector3<T> *pDst)
{
  serialized.Get("x").Deserialize(pDst->x);
  serialized.Get("y").Deserialize(pDst->y);
  serialized.Get("z").Deserialize(pDst->z);
}

template<typename T> void atDeserialize(const atObjectDescriptor &serialized, atVector4<T> *pDst)
{
  serialized.Get("x").Deserialize(pDst->x);
  serialized.Get("y").Deserialize(pDst->y);
  serialized.Get("z").Deserialize(pDst->z);
  serialized.Get("w").Deserialize(pDst->w);
}

template<typename T> void atDeserialize(const atObjectDescriptor &serialized, atMatrix4x4<T> *pDst)
{
  atDeserialize(serialized, &src.m, atArraySize(src.m));
}

template<typename T> void atDeserialize(const atObjectDescriptor &serialized, atQuaternion<T> *pDst)
{
  serialized.Get("x").Deserialize(pDst->x);
  serialized.Get("y").Deserialize(pDst->y);
  serialized.Get("z").Deserialize(pDst->z);
  serialized.Get("w").Deserialize(pDst->w);
}

template<typename T> void atSerialize(atObjectDescriptor *pSerialized, const T *pSrc, int64_t count)
{
  pSerialized->SetType(atObjectDescriptor::OT_Array);
  for (const T &val : atIterate(pSrc, count))
    pSerialized->Add("") = val;
}

// Containers
template<typename T> void atSerialize(atObjectDescriptor *pSerialized, const atVector<T> &src) { atSerialize(pSerialized, src.data(), src.size()); }

template<typename Key, typename Value> void atSerialize(atObjectDescriptor *pSerialized, const atHashMap<Key, Value> &src)
{
  pSerialized->SetType(atObjectDescriptor::OT_Object);
  for (const atKeyValue<Key, Value> &kvp : src)
    pSerialized->Add(atToString(kvp.m_key)) = kvp.m_val;
}

template<typename T> void atDeserialize(const atObjectDescriptor &serialized, T *pDst, int64_t count)
{
  if (!serialized.IsArray())
    return;

  for (int64_t i = 0; i < count; ++i)
    serialized.Get(i).Deserialize(pDst + i);
}

template<typename T> void atDeserialize(const atObjectDescriptor &serialized, atVector<T> *pDst)
{
  pDst->resize(serialized.GetMemberCount());
  atDeserialize(serialized, pDst->data(), pDst->size());
}

template<typename Key, typename Value> void atDeserialize(const atObjectDescriptor &serialized, atHashMap<Key, Value> *pDst)
{
  if (serialized.GetValueType() != atObjectDescriptor::OT_Object)
    return;

  for (const atObjectDescriptor &member : serialized.GetMembers())
    pDst->TryAdd(atFromString<Key>(member.GetName()), member.Deserialize<Value>());
}
