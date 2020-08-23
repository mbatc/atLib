#ifndef atResourceHandle_h__
#define atResourceHandle_h__

#include "atResourceGUID.h"
#include "atThreading.h"
#include "atObject.h"

class atResourceHandlerBase;

class atResourceHandle
{
  friend atResourceHandlerBase;
  template<typename T> friend class atResourceReadHandle;
  template<typename T> friend class atResourceWriteHandle;

  atResourceHandle(const atResourceGUID &guid, atResourceHandlerBase *pHandler);

public:
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

template<typename T> class atResourceReadHandle
{
public:
  const T& Get();
  const T& operator->();

protected:
  atResourceHandle m_handle;
};

template<typename T> class atResourceWriteHandle
{
public:
  T& Get();
  T& operator->();

protected:
  atResourceHandle m_handle;
};

#include "atResourceHandle.inl"
#endif // atResourceHandle_h__
