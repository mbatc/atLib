#ifndef atGFXObject_h__
#define atGFXObject_h__

#include "atGFXContext.h"

class atGFXResource
{
public:
  ~atGFXResource() { Delete(); }
  virtual bool Bind() { return false; }   // Bind the buffer to the current rendering context
  virtual bool Upload() { return false; } // Upload the buffer to the GPU
  virtual bool Delete() { return false; } // Delete the GPU buffer object
  void* GFXResource() { return m_pResource; }

  virtual atGraphicsAPI API() { return atGfxApi_None; }

protected:
  void *m_pResource = nullptr;
};

#endif // atGFXObject_h__
