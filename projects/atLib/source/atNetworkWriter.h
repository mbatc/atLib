#ifndef atNetworkWriter_h__
#define atNetworkWriter_h__

#include "atWriteStream.h"
#include "atSocket.h"
#include "atString.h"

class atNetworkWriter : public atWriteStream
{
public:
  ~atNetworkWriter();
  atNetworkWriter();

  // See Open() for more information

  atNetworkWriter(const atString &addr, const atString &port, const int64_t timeout = -1);

  // if timeout == -1, 
  //  - Open() will not wait until the target is writeable
  //  - Open() will always return true
  //  - CanWrite() can be used to check if the target is writeable, allowing Open() to be a non-blocking action
  //
  // if timeout > 0, 
  //  - Open() will return when the target is writeable or after [timeout] milliseconds
  //  - Open() will return true if the target is writeable
  //  - Open() will return false if the timeout was reached

  bool Open(const atString &addr, const atString &port, const int64_t timeout = -1);

  // Close the connection
  void Close();

  // if TRUE the target can be written to
  // if FALSE all calls to Write() will fail
  
  bool CanWrite();

  int64_t Write(void *pData, const int64_t len);
  template <typename T> int64_t Write(const T *pData, const int64_t count = 1);
  template <typename T> int64_t Write(const T &data);

protected:
  atSocket *m_pConn;
};

template<typename T> int64_t atNetworkWriter::Write(const T *pData, const int64_t count) { return this->Write(pData, count); }
template<typename T> int64_t atNetworkWriter::Write(const T &data) { return this->Write(data); }

#endif // atNetworkWriter_h__
