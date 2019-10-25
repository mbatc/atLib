#include "atStringTest.h"
#include "atString.h"

void atStringTestConstructor()
{
  {
    char str[] = "abcde";
    atString testStr(str, str + 3);
    atRelAssert(testStr[0] == 'a', "atString constructor test failed.");
    atRelAssert(testStr[1] == 'b', "atString constructor test failed.");
    atRelAssert(testStr[2] == 'c', "atString constructor test failed.");
    atRelAssert(testStr[3] == '\0', "atString constructor test failed.");
  }

  {
    atString testStr = "Hello World";
    atRelAssert(testStr[0] == 'H', "atString constructor test failed.");
    atRelAssert(testStr[1] == 'e', "atString constructor test failed.");
    atRelAssert(testStr[2] == 'l', "atString constructor test failed.");
    atRelAssert(testStr[3] == 'l', "atString constructor test failed.");
    atRelAssert(testStr[4] == 'o', "atString constructor test failed.");
    atRelAssert(testStr[5] == ' ', "atString constructor test failed.");
    atRelAssert(testStr[6] == 'W', "atString constructor test failed.");
    atRelAssert(testStr[7] == 'o', "atString constructor test failed.");
    atRelAssert(testStr[8] == 'r', "atString constructor test failed.");
    atRelAssert(testStr[9] == 'l', "atString constructor test failed.");
    atRelAssert(testStr[10] == 'd', "atString constructor test failed.");
    atRelAssert(testStr[11] == '\0', "atString constructor test failed.");
  }
}

void atStringTestLength()
{
  {
    atString str;
    str = "Hello World";
    atRelAssert(str.length() == 11, "atString length() test failed.");

    str = "Something";
    atRelAssert(str.length() == 9, "atString length() test failed.");
  }
}

void atStringTestCapacity()
{
  atString str;
  str = "Hello World";
  atRelAssert(str.capacity() == str.vector().capacity(), "atString capacity() test failed.");
}

void atStringTestToLower()
{
  {
    atString test = "ThIs Is A Test From UPPER to l0wer 1234567890";
    atRelAssert(test.to_lower() == "this is a test from upper to l0wer 1234567890", "atString to_lower() failed");
  }

  {
    atString test = "1234567890_+:<>?";
    atRelAssert(test.to_lower() == "1234567890_+:<>?", "atString to_lower() failed");
  }
}

void atStringTestToUpper()
{
  {
    atString test = "this is a lower string";
    atRelAssert(test.to_upper() == "THIS IS A LOWER STRING", "atString to_upper() failed");
  }

  {
    atString test = "1234567890_+:<>?";
    atRelAssert(test.to_upper() == "1234567890_+:<>?", "atString to_upper() failed");
  }
}

void atStringTestCompare()
{
}

void atStringTestAppend()
{
}

void atStringTestSubStr()
{
}

void atStringTestReplace()
{
}

void atStringTestSplit()
{
}

void atStringTestJoin()
{
}

void atStringTestSetString()
{
}

void atStringTestFindEnd()
{
}

void atStringTestFindReverse()
{
}

void atStringTestFindFirstNot()
{
}

void atStringTestFindLastNot()
{
}

void atStringTestFindFirstOf()
{
}

void atStringTestFindLastOf()
{
}

void atStringTestFindFirst()
{
}

void atStringTestFindLast()
{
}

void atStringTestOperators()
{
}
