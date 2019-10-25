#include "atContainerTest.h"
#include "atHashMapTest.h"
#include "atStringTest.h"
#include "atVectorTest.h"
#include "atHashSetTest.h"

void tests::containers::String()
{
  atStringTestConstructor();
  atStringTestLength();
  atStringTestCapacity();
  atStringTestToLower();
  atStringTestToUpper();
  atStringTestCompare();
  atStringTestAppend();
  atStringTestSubStr();
  atStringTestReplace();
  atStringTestSplit();
  atStringTestJoin();
  atStringTestSetString();
  atStringTestFindEnd();
  atStringTestFindReverse();
  atStringTestFindFirstNot();
  atStringTestFindLastNot();
  atStringTestFindFirstOf();
  atStringTestFindLastOf();
  atStringTestFindFirst();
  atStringTestFindLast();
  atStringTestOperators();
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
  atHashMapTestAdd();
  atHashMapTestContains();
  atHashMapTestGetOrAdd();
  atHashMapTestRemove();
  atHashMapTestGetKeys();
  atHashMapTestGetValues();
  atHashMapTestTryGet();
  atHashMapTestSize();
}

void tests::containers::HashSet()
{
  atHashSetTestAdd();
  atHashSetTestRemove();
  atHashSetTestContains();
  atHashSetTestTryGet();
  atHashSetTestTryAdd();
  atHashSetTestClear();
  atHashSetTestSize();
}
