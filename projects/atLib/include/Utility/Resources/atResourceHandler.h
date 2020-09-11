#ifndef atResourceHandler_h__
#define atResourceHandler_h__

#include "atResourceHandle.h"

class atResourceHandlerBase
{
  friend class atResourceManager;
  friend class atResourceHandle;
public:
  atResourceHandlerBase(const atString &resourceTypeID);

  template<typename T> bool Handles() const;

  atResourceHandle Request(const atObjectDescriptor &request);
  atResourceManager* GetResourceManager() const;

protected:
  bool Remove(const atResourceGUID &guid);

  virtual bool Handles(const std::type_index &typeInfo) const = 0;
  virtual atObject _Load(const atObjectDescriptor &request) = 0;

  atHashMap<atResourceGUID, atResourceHandle::Instance*> m_resources;
  atResourceManager *m_pManager = nullptr;
  std::mutex m_resourceLock;
  const atString m_resourceTypeID;
};

template<typename T> class atResourceHandler : public atResourceHandlerBase
{
public:
  atResourceHandler(const atString &resourceTypeID);

  virtual bool Load(const atObjectDescriptor &request, T *pResource) = 0;

protected:
  atObject _Load(const atObjectDescriptor &request) override final;

  virtual bool Handles(const std::type_index &typeInfo) const;
};

#include "atResourceHandler.inl"

#endif // atResourceHandler_h__
