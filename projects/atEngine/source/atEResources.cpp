#include "atEResources.h"

static atEResources *_pInstance = nullptr;

atEResources* atEResources::Instance()
{
  if (!_pInstance)
    _pInstance = new atEResources;
  return _pInstance;
}

void atEResources::Destroy()
{
  if (_pInstance)
    delete _pInstance;
}

atResourceManager& atEResources::GetManager() { return m_manager; }
