#include "atPipedCommand.h"

atVector<uint8_t> atPipedCommand::Run(const atString &command, const atVector<atString> &args)
{
  char buffer[1024] = { 0 };

  atString cmdLine = command;
  for (const atString &v : args)
    cmdLine += " \"" + v + "\"";

  atString result = "";
  FILE* hPipe = _popen(cmdLine.c_str(), "r");
  if (!hPipe)
    return {};
  
  try
  {
    while (fgets(buffer, sizeof(buffer), hPipe) != NULL)
      result += buffer;
  }
  catch (...) {
    // Something went wrong
  }
  _pclose(hPipe);

  return atVector<uint8_t>((uint8_t*)result.c_str(), result.length() + 1);
}
