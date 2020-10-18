#include "atDLL.h"
#include <libloaderapi.h>

atDLL::atDLL(const atFilename &path)
  : m_module(atNew(Module)(path))
{}

bool atDLL::HasFunction(const atString &name) { return m_module && m_module->GetFunction(name) != nullptr; }

bool atDLL::IsLoaded() const { return m_module && m_module->IsLoaded(); }

atFilename atDLL::GetDLLPath() const { return m_module ? m_module->GetPath() : ""; }

atDLL::Module::Module(const atFilename &path)
  : m_dllPath(path)
  , pHandle(LoadLibrary(path.c_str()))
{}

atDLL::Module::~Module() { FreeLibrary((HMODULE)pHandle); }

atDLL::NullFunc atDLL::Module::GetFunction(const atString &name)
{
  if (!IsLoaded())
    return nullptr;

  NullFunc *pFunc = functions.TryGet(name);
  if (!pFunc)
  {
    NullFunc newFunc = LoadFunction(name);
    functions.Add(name, newFunc);
    return newFunc;
  }

  return *pFunc;
}

bool atDLL::Module::IsLoaded() const { return pHandle != nullptr; }

const atFilename &atDLL::Module::GetPath() const { return m_dllPath; }

atDLL::NullFunc atDLL::Module::LoadFunction(const atString &name)
{
  return IsLoaded() ? (NullFunc)GetProcAddress((HMODULE)pHandle, name.c_str()) : nullptr;
}
