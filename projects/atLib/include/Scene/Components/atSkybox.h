#ifndef atSkybox_h__
#define atSkybox_h__

#include "atSceneComponent.h"

class atSkybox : public atSceneComponent
{
public:
  bool Draw(const atMat4D &vp) override;

  int64_t TypeID() const override;
  static const int64_t typeID;
protected:

};

#endif // atSkybox_h__
