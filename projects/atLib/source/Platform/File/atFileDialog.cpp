#include "atFileDialog.h"
#include <windows.h>

static const char *_defaultFilter = "All Files\0*.*\0\0";
static char _path[MAX_PATH] = { 0 };

static OPENFILENAME _GetOFN(const atString *pInitialPath, const char *filter, const int64_t initialFilter)
{
  OPENFILENAME ofn = { 0 };
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.lpstrInitialDir = pInitialPath ? pInitialPath->c_str(): nullptr;
  ofn.lpstrFilter = strlen(filter) > 0 ? filter : _defaultFilter;
  ofn.nFilterIndex = atMax(1, initialFilter);
  return ofn;
}

const char* atFileDialog::Open(const atString &initialPath, const char *filter, const int64_t initialFilter)
{
  OPENFILENAME ofn = _GetOFN(initialPath.length() > 0 ? &initialPath : nullptr, filter, initialFilter);
  memset(_path, 0, sizeof(_path));
  ofn.lpstrDefExt = "";
  ofn.lpstrFile = _path;
  ofn.nMaxFile = sizeof(_path);
  if (!GetOpenFileName(&ofn))
    return "";
  return ofn.lpstrFile;
}

const char* atFileDialog::SaveAs(const atString &initialPath, const char *filter, const int64_t initialFilter)
{
  OPENFILENAME ofn = _GetOFN(initialPath.length() > 0 ? &initialPath : nullptr, filter, initialFilter);
  memset(_path, 0, sizeof(_path));
  ofn.lpstrDefExt = "";
  ofn.lpstrFile = _path;
  ofn.nMaxFile = sizeof(_path);
  if (!GetSaveFileName(&ofn))
    return "";
  return ofn.lpstrFile;
}

const char* atFileDialog::BrowseFolder()
{
  return "";
}
