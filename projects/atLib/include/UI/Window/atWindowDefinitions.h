#ifndef _atWindowDefs_h__
#define _atWindowDefs_h__

#include "atString.h"

enum atWindowStyle : int64_t
{
  atWS_None           = 0,
  atWS_Border         = 1 << 0,
  atWS_ThickFrame     = 1 << 2,
  atWS_Popup          = 1 << 3,
  atWS_Caption        = 1 << 4,
  atWS_MinimizeButton = 1 << 5,
  atWS_MaximizeButton = 1 << 6,
  atWS_SystemMenu     = 1 << 7,
  atWS_HScroll        = 1 << 8,
  atWS_VScroll        = 1 << 9,
  atWS_Overlapped     = (int64_t)atWS_ThickFrame | atWS_Caption | atWS_SystemMenu | atWS_MinimizeButton | atWS_MaximizeButton,
};
atWindowStyle operator|(const atWindowStyle &a, const atWindowStyle &b);
atWindowStyle operator&(const atWindowStyle &a, const atWindowStyle &b);

enum atWindowFlags : int64_t
{
  atWF_None      = 0,
  atWF_Resizable = 1 << 0,
  atWF_Windowed  = 1 << 1,
  atWF_Visible   = 1 << 2,
  atWF_Default   = (int64_t)atWF_Resizable | atWF_Windowed | atWF_Visible,
};
atWindowFlags operator|(const atWindowFlags &a, const atWindowFlags &b);
atWindowFlags operator&(const atWindowFlags &a, const atWindowFlags &b);

struct atWindowCreateInfo
{
  atString title = "Default Window";
  atVec2I size = atVec2I(-1, -1);
  atVec2I pos = atVec2I(0, 0);
  atWindowStyle style = atWS_Overlapped;
  atWindowFlags flags = atWF_Resizable | atWF_Windowed | atWF_Visible;

  // Buffer descriptions
  int64_t colourBits = 32;
  int64_t alphaBits = 8;
  int64_t depthBits = 24;
};

#endif