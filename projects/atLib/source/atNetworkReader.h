#ifndef atNetworkReader_h__
#define atNetworkReader_h__

#include "atReadStream.h"
#include "atSocket.h"
#include "atString.h"

class atNetworkReader : public atReadStream
{
public:
  atNetworkReader();

  // See Open() for more information

  atNetworkReader(const atString &port, const int64_t timeout = -1);
  ~atNetworkReader();

  // Open() will return true if the target is readable
  //
  // if timeout == -1, 
  //  - Open() will not wait until the target is readable
  //  - CanWrite() can be used to check if the target is readable, allowing Open() to be a non-blocking action
  //
  // if timeout > 0, 
  //  - Open() will return when the target is readable or after [timeout] milliseconds
  //  - Open() will return false if the timeout was reached

  bool Open(const atString &port, const int64_t timeout = -1);

  // Close the connection
  void Close();

  // if TRUE the target can be read from
  // if FALSE all calls to Read() will fail

  bool CanRead();

  int64_t Read(void *pBuffer, const int64_t size);
  template <typename T> Read(T *pData, const int64_t count = 1);

protected:
  atSocket *m_pHost;
  atSocket *m_pConn;
};

template<typename T> atNetworkReader::Read(T * pData, const int64_t count) { return this->Read(pData, count); }

#endif // atNetworkReader_h__
