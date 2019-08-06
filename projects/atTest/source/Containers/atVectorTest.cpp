#include "atVectorTest.h"
#include "atTypes.h"
#include "atMemoryWriter.h"

void atVectorTestConstructor()
{
}

void atVectorTestDestructor()
{
}

void atVectorTestAllocation()
{
}

void atVectorTestClear()
{
}

void atVectorTestCopy()
{
}

void atVectorTestMove()
{
}

void atVectorTestAccessor()
{
}

template <typename T> void _TestStreamRead()
{

}

template <typename T> void _TestStreamWrite(int64_t size, T (*getItemFunc)(int64_t index))
{
  atVector<T> vec;
  for (int64_t i = 0; i < size; ++i)
    vec.push_back(getItemFunc(i));

  atMemoryWriter memWriter;
  memWriter.Write(vec);
  atRelAssert(memcmp(memWriter.m_data.data(), &vec.size(), sizeof(int64_t)) == 0, "Size written incorrect");
  atRelAssert(memcmp(memWriter.m_data.data() + sizeof(int64_t), vec.data(), sizeof(T) * vec.size()) == 0, "Data written incorrect");
}

void _atVectorTestStreamReadAllTrivial(int64_t vectorSize)
{
  _TestStreamWrite<uint8_t>(vectorSize, [](int64_t index) { return (uint8_t)rand(); });
  _TestStreamWrite<uint16_t>(vectorSize, [](int64_t index) { return (uint16_t)rand(); });
  _TestStreamWrite<uint32_t>(vectorSize, [](int64_t index) { return (uint32_t)rand(); });
  _TestStreamWrite<uint64_t>(vectorSize, [](int64_t index) { return (uint64_t)rand(); });
  _TestStreamWrite<int8_t>(vectorSize, [](int64_t index) { return (int8_t)rand(); });
  _TestStreamWrite<int16_t>(vectorSize, [](int64_t index) { return (int16_t)rand(); });
  _TestStreamWrite<int32_t>(vectorSize, [](int64_t index) { return (int32_t)rand(); });
  _TestStreamWrite<int64_t>(vectorSize, [](int64_t index) { return (int64_t)rand(); });
}

void atVectorTestStreamReadTrivial()
{
  _atVectorTestStreamReadAllTrivial(153);
  _atVectorTestStreamReadAllTrivial(1000);
  _atVectorTestStreamReadAllTrivial(10000);
}

void atVectorTestStreamWriteTrivial()
{
}

void atVectorTestStreamRead()
{
}

void atVectorTestStreamWrite()
{
}
