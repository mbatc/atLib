#include "atAssetDirs.h"

static atString _shaderDir = "../../projects/atLib/assets/shaders/";
static atString _fontDir = "../../projects/atLib/assets/fonts/";
static atString _skyboxes = "../../projects/atLib/assets/skyboxes/";

void atAssetDirs::SetShaders(const atString &path) { _shaderDir = path; }
void atAssetDirs::SetFonts(const atString &path) { _fontDir = path; }
void atAssetDirs::SetSkyboxes(const atString &path) { _skyboxes = path; }

atString atAssetDirs::Shaders() { return _shaderDir; }
atString atAssetDirs::Fonts() { return _fontDir; }
atString atAssetDirs::Skyboxes() { return _skyboxes; }
