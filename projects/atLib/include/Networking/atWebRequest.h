#ifndef atWebRequest_h__
#define atWebRequest_h__

#include <typeindex>

class atWebRequest
{
public:
  virtual std::type_index TypeID() const = 0;
  template <typename T> bool Is() const;
  template <typename T> const T &As() const;
  template <typename T> T &As();
};

template <typename T> class atWebRequestInterface : public atWebRequest
{
public:
  atWebRequestInterface();

  std::type_index TypeID() const override;

protected:
  const std::type_index m_typeID;
};

#include "atWebRequest.inl"
#endif // atWebRequest_h__
