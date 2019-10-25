#include "atHashSetTest.h"
#include "atHashSet.h"

void atHashSetTestAdd()
{
  { // Simple small set test
    atHashSet<int64_t> set;
    set.Add(1);
    atRelAssert(set.Size() == 1, "atHashSet Add() failed.");
    atRelAssert(set.Contains(1), "atHashSet Add() failed.");

    set.Add(2);
    atRelAssert(set.Size() == 2, "atHashSet Add() failed.");
    atRelAssert(set.Contains(2), "atHashSet Add() failed.");

    set.Add(1000);
    atRelAssert(set.Size() == 3, "atHashSet Add() failed.");
    atRelAssert(set.Contains(1000), "atHashSet Add() failed.");

    set.Add(132);
    atRelAssert(set.Size() == 4, "atHashSet Add() failed.");
    atRelAssert(set.Contains(132), "atHashSet Add() failed.");
  }

  { // Large set test
    atHashSet<int64_t> set;
    for (int64_t i = 0; i < 1000; ++i)
      set.Add(i);
    for (int64_t i = 0; i < 1000; ++i)
      atRelAssert(set.Contains(i), "atHashSet Add() failed.");
  }
}

void atHashSetTestTryAdd()
{
  atHashSet<int64_t> set;
  atRelAssert(set.TryAdd(10), "atHashSet TryAdd() failed.");
  atRelAssert(set.Size() == 1, "atHashSet TryAdd() failed.");
  atRelAssert(set.Contains(10), "atHashSet TryAdd() failed.");

  atRelAssert(!set.TryAdd(10), "atHashSet TryAdd() failed.");
  atRelAssert(set.Size() == 1, "atHashSet TryAdd() failed.");
  atRelAssert(set.Contains(10), "atHashSet TryAdd() failed.");

  atRelAssert(set.TryAdd(12), "atHashSet TryAdd() failed.");
  atRelAssert(set.Size() == 2, "atHashSet TryAdd() failed.");
  atRelAssert(set.Contains(12), "atHashSet TryAdd() failed.");
  
  atRelAssert(!set.TryAdd(10), "atHashSet TryAdd() failed.");
  atRelAssert(set.Size() == 2, "atHashSet TryAdd() failed.");
  atRelAssert(set.Contains(12), "atHashSet TryAdd() failed.");
}

void atHashSetTestClear()
{
  atHashSet<int64_t> set;
  for (int64_t i = 0; i < 1000; ++i)
    set.Add(i);
  atRelAssert(set.Size() == 1000, "atHashSet Add() failed.");
  for (int64_t i = 0; i < 1000; ++i)
    atRelAssert(set.Contains(i), "atHashSet Add() failed.");

  set.Clear();
  atRelAssert(set.Size() == 0, "atHashSet Add() failed.");
  for (int64_t i = 0; i < 1000; ++i)
    atRelAssert(!set.Contains(i), "atHashSet Add() failed.");
}

void atHashSetTestSize()
{
  atHashSet<int64_t> set;
  for (int64_t i = 0; i < 1000; ++i)
    set.Add(i);

  atRelAssert(set.Size() == 1000, "atHashSet Size() failed.");
  set.Remove(1);
  set.Remove(2);
  set.Remove(3);
  
  atRelAssert(set.Size() == 997, "atHashSet Size() failed.");
}

void atHashSetTestRemove()
{
  // TODO
}

void atHashSetTestContains()
{
  // TODO
}

void atHashSetTestTryGet()
{
  // TODO
}
