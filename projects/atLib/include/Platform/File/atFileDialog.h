#ifndef atFileDialog_h__
#define atFileDialog_h__

#include "atString.h"

class atFileDialog
{
public:
  static const char* Open(const atString &initialPath, const char *filter, const int64_t initialFilter = 1);
  static const char* SaveAs(const atString &initialPath, const char *filter, const int64_t initialFilter = 1);
  static const char* BrowseFolder();
};

#endif // atFileDialog_h__
