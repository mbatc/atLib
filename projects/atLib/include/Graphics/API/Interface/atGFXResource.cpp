#include "atGFXResource.h"

void atGFXResource::AddReference() { ++m_numReferences; }
bool atGFXResource::Release() { return --m_numReferences == 0; }
