#ifndef _atWindowDefs_h__
#define _atWindowDefs_h__

#include "atTypes.h"

enum atWindowStyle : int64_t
{
  atWS_None =           0,
  atWS_Border =         1 << 0,
  atWS_ThickFrame =     1 << 2,
  atWS_Popup =          1 << 3,
  atWS_Caption =        1 << 4,
  atWS_MinimizeButton = 1 << 5,
  atWS_MaximizeButton = 1 << 6,
  atWS_SystemMenu =     1 << 7,
  atWS_HScroll =        1 << 8,
  atWS_VScroll =        1 << 9,
  atWS_Overlapped = atWS_ThickFrame | atWS_Caption | atWS_SystemMenu | atWS_MinimizeButton | atWS_MaximizeButton,
};

#endif