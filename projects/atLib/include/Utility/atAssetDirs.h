#ifndef atAssetDirs_h__
#define atAssetDirs_h__

#include "atString.h"

class atAssetDirs
{
public:
  static void SetShaders(const atString &path);
  static void SetFonts(const atString &path);
  static void SetSkyboxes(const atString &path);

  static atString Shaders();
  static atString Fonts();
  static atString Skyboxes();
};

#endif // atAssets_h__
