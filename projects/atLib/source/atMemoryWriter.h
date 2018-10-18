#ifndef atMemoryWriter_h__
#define atMemoryWriter_h__

#include "atFileCommon.h"

class atMemoryWriter : public atWriteStream
{
public:
  atMemoryWriter();
  atMemoryWriter(atMemoryWriter &&move);
  atMemoryWriter(const atMemoryWriter &move);
  ~atMemoryWriter();

  void Clear();
  bool Seek(const int64_t offset, const atFileSeek start = atFS_Current);

  int64_t Write(void *pData, const int64_t len);
  template<typename T> int64_t Write(const T &data);


  bool operator!=(const atMemoryWriter &rhs);
  bool operator==(const atMemoryWriter &rhs);

protected:
  atVector<uint8_t> m_data;
  int64_t m_pos = 0;
};

template<typename T> int64_t atMemoryWriter::Write(const T & data) { return atStreamWrite(this, data); }

#endif // atMemoryWriter_h__
