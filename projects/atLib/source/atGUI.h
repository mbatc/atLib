#ifndef atGUI_h__
#define atGUI_h__

#include "atUIWindow.h"
#include "atRenderable.h"

class atGUI
{
public:
  atGUI() = delete;



protected:
  static atRenderable m_ro;
  static atVector<atVec3F> m_vtx;
  static atVector<uint32_t> m_idx;
  static atVector<atVec4F> m_color;
};

#endif // atUI_h__
