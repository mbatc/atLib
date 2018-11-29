#ifndef atUIButton_h__
#define atUIButton_h__

#include "atString.h"

class atUIActiveRect
{
public:
  atUIActiveRect() = delete;

  bool Begin(const atString &name);
  void End();

  atVec2I pos;
  atVec2I size;

  atString name;
  int64_t id;
};

#endif // atUIButton_h__
