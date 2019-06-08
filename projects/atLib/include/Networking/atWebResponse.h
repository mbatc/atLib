#ifndef atWebResponse_h__
#define atWebResponse_h__

#include <typeindex>

class atWebResponse
{
  virtual std::type_index TypeID() const = 0;
  template <typename T> bool Is() const;
  template <typename T> const T &As() const;
  template <typename T> T &As();
};

template <typename T> class atWebResponseInterface : public atWebResponse
{
public:
  atWebResponseInterface();

  std::type_index TypeID() const override;

protected:
  const std::type_index m_typeID;
};

#include "atWebResponse.inl"
#endif // atWebResponse_h__
