#include "atURI.h"

atURI::atURI(const atString &scheme, const atString &auth, const atString &path, const atString &query, const atString &fragment)
{
  SetScheme(scheme);
  SetAuthority(auth);
  SetPath(path);
  SetQuery(query);
  SetFragment(fragment);
}

atURI::atURI() {}

atURI::atURI(const atString &uri)
{
  // Find Scheme
  int64_t schemeEnd = atMax(0, uri.find_first_of(':'));
  SetScheme(uri.substr(0, schemeEnd));

  // Find Authority String
  int64_t authStart = uri.find("//", schemeEnd);
  int64_t authEnd = schemeEnd;
  if (authStart > 0)
  {
    authStart += 2;
    authEnd = uri.find('/', authStart);
    if (authEnd < 0)
      authEnd = uri.length();
    SetAuthority(uri.substr(authStart, authEnd));
  }

  // Find Fragment
  int64_t fragStart = uri.find_first_of('#', authEnd);
  if (fragStart < 0) fragStart = uri.length();
  SetFragment(uri.substr(fragStart + 1, uri.length()));
  
  // Find Query
  int64_t queryStart = uri.find_first_of('?', authEnd);
  if (queryStart < 0) queryStart = uri.length();
  SetQuery(uri.substr(queryStart + 1, fragStart - 1));

  // Find Path
  SetPath(uri.substr(authEnd, atMin(fragStart, queryStart)));
}

void atURI::SetAuthority(const atString &auth)
{
  m_userInfo = "";
  m_host = "";
  m_port = "";

  int64_t userEnd = atMax(0, auth.find_first_of('@'));
  int64_t portStart = auth.find_last_of(':');
  if (portStart < 0) portStart = auth.length();
  if (userEnd > 0) m_userInfo = auth.substr(0, userEnd);
  if (portStart != auth.length()) m_port = auth.substr(portStart, auth.length());
  m_host = auth.substr(userEnd, portStart);
}

atString atURI::Authority() const
{
  atString auth = Host();
  if (UserInfo().length() > 0)
    auth = UserInfo() + "@" + auth;
  if (Port().length() > 0)
    auth += ":" + Port();
  return auth;
}

atString atToString(const atURI &uri)
{
  atString ret;
  
  // Append scheme if it exists
  if (uri.Scheme().length() > 0)
    ret = uri.Scheme() + ":";

  // Append authority if it exists
  atString auth = uri.Authority();
  if (auth.length() > 0)
    ret += "//" + auth + "/";

  // Always set path
  ret += uri.Path();

  // Append query if it exists
  if (uri.Query().length() > 0)
    ret += "?" + uri.Query();

  // Append fragment if it exists
  if (uri.Fragment().length() > 0)
    ret += "#" + uri.Fragment();
  return ret;
}

void atURI::SetScheme(const atString &scheme) { m_scheme = scheme; }
void atURI::SetPath(const atString &path) { m_path = path; }
void atURI::SetQuery(const atString &query) { m_query = query; }
void atURI::SetFragment(const atString &frag) { m_fragment = frag; }
void atURI::SetHost(const atString &host) { m_host = host; }
void atURI::SetUserInfo(const atString &userInfo) { m_userInfo = userInfo; }
void atURI::SetPort(const atString &port) { m_port = port; }

const atString& atURI::Scheme() const { return m_scheme; }
const atString& atURI::Path() const { return m_path; }
const atString& atURI::Query() const { return m_query; }
const atString& atURI::Fragment() const { return m_fragment; }
const atString& atURI::Host() const { return m_host; }
const atString& atURI::UserInfo() const { return m_userInfo; }
const atString& atURI::Port() const { return m_port; }
