#ifndef atURI_h__
#define atURI_h__

#include "atString.h"

class atURI
{
public:
  atURI();
  atURI(const atString &uri);
  atURI(const atString &scheme, const atString &auth, const atString &path, const atString &query, const atString &fragment);

  void SetScheme(const atString &scheme);
  void SetAuthority(const atString &auth);
  void SetPath(const atString &path);
  void SetQuery(const atString &query);
  void SetFragment(const atString &frag);
  void SetHost(const atString &host);
  void SetUserInfo(const atString &userInfo);
  void SetPort(const atString &port);

  atString Authority() const;
  
  const atString& Scheme() const;
  const atString& Path() const;
  const atString& Query() const;
  const atString& Fragment() const;
  const atString& Host() const;
  const atString& UserInfo() const;
  const atString& Port() const;

protected:
  atString m_scheme;
  atString m_userInfo;
  atString m_host;
  atString m_port;
  atString m_path;
  atString m_query;
  atString m_fragment;
};

atString atToString(const atURI &uri);
atURI atFromString(const atString &str);

#endif // atURI_h__
