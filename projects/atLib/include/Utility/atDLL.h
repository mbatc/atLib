#ifndef atDLL_h__
#define atDLL_h__

#include "atFilename.h"
#include "atPtr.h"
#include <type_traits>

class atDLL
{
  typedef void(*NullFunc)();
public:
  atDLL() = default;
  atDLL(atDLL &&o) = default;
  atDLL(const atDLL &o) = default;
  atDLL(const atFilename &path);

  atDLL& operator=(atDLL &&o) = default;
  atDLL& operator=(const atDLL & o) = default;

  // Get a function in the DLL as a callable object
  // The signature specified must match the exported function in the DLL
  template<typename FuncSig> std::function<FuncSig> Get(const atString &name);

  // Call a function in the DLL.
  // The return type and arguments must match the exported function in the DLL.
  // This function is not safe to call if the DLL does not export a function with 'name'
  // If the functions existence is not certain, use Get(name), check the result and call the return function instead.
  template<typename ReturnType, typename... Args> ReturnType Call(const atString &name, Args&&... args);

  // Check if the loaded DLL contains a function with the given name
  bool HasFunction(const atString &name);

  // Returns true if the DLL was successfully loaded
  bool IsLoaded() const;

  // Return the path of the DLL this object refers to
  atFilename GetDLLPath() const;

protected:
  class Module
  {
  public:
    Module(const atFilename &path);
    ~Module();

    NullFunc GetFunction(const atString &name);
    bool IsLoaded() const;
    const atFilename& GetPath() const;

  protected:
    NullFunc LoadFunction(const atString &name);

    void *pHandle = nullptr;
    atHashMap<atString, NullFunc> functions;
    atFilename m_dllPath;
  };

  atPtr<Module> m_module;
};

template<typename FuncSig>
inline std::function<FuncSig> atDLL::Get(const atString &name)
{
  if (!m_module)
    return nullptr;

  std::function<FuncSig> callable = (FuncSig*)m_module->GetFunction(name);
  return callable;
}

template<typename ReturnType, typename ...Args>
inline ReturnType atDLL::Call(const atString &name, Args&&... args)
{
  using FuncSig = std::decay_t<ReturnType>(std::decay_t<Args>...);
  std::function<FuncSig> callable = Get<FuncSig>(name);
  return callable(std::forward<Args>(args)...);
}

#endif // atDLL_h__
