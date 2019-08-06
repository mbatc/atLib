#include "atContainerTest.h"
#include "atHashMapTest.h"
#include "atStringTest.h"
#include "atVectorTest.h"

void tests::containers::String()
{
}

void tests::containers::Vector()
{
  atVectorTestConstructor();
  atVectorTestDestructor();
  atVectorTestCopy();
  atVectorTestMove();
  atVectorTestClear();
  atVectorTestStreamReadTrivial();
  atVectorTestStreamWriteTrivial();
  atVectorTestStreamWrite();
  atVectorTestStreamRead();
}

void tests::containers::HashMap()
{
}
