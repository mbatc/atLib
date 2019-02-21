#include "atPrint.h"

atString atPrint::Float(const double &val)
{
  char buffer[64];
  sprintf(buffer, "%f", val);
  return buffer;
}

atString atPrint::Int(const int64_t &val)
{
  char buffer[64];
  sprintf(buffer, "%lld", val);
  return buffer;
}
