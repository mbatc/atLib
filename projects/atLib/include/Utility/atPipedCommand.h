#ifndef atPipedCommand_h__
#define atPipedCommand_h__

#include "atString.h"

class atPipedCommand
{
public:
  static atVector<uint8_t> Run(const atString &command, const atVector<atString> &args);
};

#endif // atPipedCommand_h__
