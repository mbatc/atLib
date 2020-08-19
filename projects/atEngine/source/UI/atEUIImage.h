#ifndef atEUIImage_h__
#define atEUIImage_h__

#include "atEUIComponent.h"

class atEUIImage : public atEUIComponent
{
public:
  atEUIImage(atTexture *pImage = nullptr);

  virtual bool Draw(atCanvas2D *pGraphics) override;

  // Set the image
  void SetImage(atTexture *pImage);

  // Get the image
  atTexture* GetImage() const;

protected:
  atTexture *m_pTexture;
};

#endif // atEUILabel_h__
