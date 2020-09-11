#ifndef atResourceHandle_h__
#define atResourceHandle_h__

#include "atResourceGUID.h"
#include "atThreading.h"
#include "atObject.h"

class atResourceHandlerBase;

// Handle to a generic resource type
class atResourceHandle
{
  struct Instance
  {
    int64_t refCount;
    std::mutex refLock;

    atObject resource;
    atResourceHandlerBase *pHandler = nullptr;
    atResourceGUID guid;
    bool loaded = false;
    bool loadFailed = false;
  } *m_pInstance = nullptr;

  friend atResourceHandlerBase;
  template<typename T> friend class atResourceReadHandle;
  template<typename T> friend class atResourceWriteHandle;

  atResourceHandle(Instance *pInstance);

public:
  atResourceHandle();
  atResourceHandle(const atResourceHandle &o);
  atResourceHandle(atResourceHandle &&o);
  ~atResourceHandle();

  atResourceHandle& operator=(atResourceHandle &&o);
  atResourceHandle& operator=(const atResourceHandle &o);

  // Check if the handle has a valid resource
  bool HasResource() const;
  
  // Check if the resource has finished loading
  bool IsLoaded() const;

  // Check if loading the resource failed
  bool LoadFailed() const;

  // Check if the resource is of the specified type
  template<typename T> bool Is() const;

  // Get a read handle to the resource
  template<typename T> atResourceReadHandle<T> GetReadable() const;

  // Get a write handle to the resource
  template<typename T> atResourceWriteHandle<T> GetWriteable() const;

protected:
  void TakeRef(Instance *pInstance);

  void ReleaseRef();
};

// Typed wrapper for atResourceHandle
template<typename T> class atResourceHandleT
{
public:
  atResourceHandleT(const atResourceHandle &handle = atResourceHandle()) : m_handle(handle) {}

  // Check if the handle has a valid resource
  bool HasResource() const { return m_handle.Is<T>(); }

  // Check if the resource has finished loading
  bool IsLoaded() const { return HasResource() && m_handle.IsLoaded(); }

  // Check if loading the resource failed
  bool LoadFailed() const { return HasResource() && m_handle.LoadFailed(); }

  // Get a read handle to the resource
  atResourceReadHandle<T> GetReadable() const { return atResourceReadHandle<T>(m_handle); }

  // Get a write handle to the resource
  atResourceWriteHandle<T> GetWriteable() const { return atResourceWriteHandle<T>(m_handle); }

  operator atResourceHandle() { return m_handle; }

protected:
  atResourceHandle m_handle;
};

// Typed read and write handles to a resource
template<typename T> class atResourceReadHandle
{
public:
  atResourceReadHandle(const atResourceHandle &handle = atResourceHandle()) { m_handle = handle; }
  const T &Get() { return m_handle.m_pInstance->resource.As<T>(); }

  template<typename U = T>
  const std::enable_if_t<std::is_pointer<U>::value, U> operator->() { return Get(); }

  template<typename U = T>
  const std::enable_if_t<!std::is_pointer<U>::value, U> *operator->() { return &Get(); }

protected:
  atResourceHandle m_handle;
};

template<typename T> class atResourceWriteHandle
{
public:
  atResourceWriteHandle(const atResourceHandle &handle = atResourceHandle()) { m_handle = handle; }
  T& Get() { return m_handle.m_pInstance->resource.As<T>(); }
  
  template<typename U = T>
  std::enable_if_t<std::is_pointer<U>::value, U> operator->() { return Get(); }

  template<typename U = T>
  std::enable_if_t<!std::is_pointer<U>::value, U> *operator->() { return &Get(); }

protected:
  atResourceHandle m_handle;
};

#include "atResourceHandle.inl"
#endif // atResourceHandle_h__
