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
  template<typename T> int64_t Write(const T *pData, const int64_t count);


  bool operator!=(const atMemoryWriter &rhs);
  bool operator==(const atMemoryWriter &rhs);

  atVector<uint8_t> m_data;
  
protected:
  int64_t m_pos = 0;
};

template<typename T> int64_t atMemoryWriter::Write(const T *pData, const int64_t count) { return atStreamWrite(this, pData, count); }
template<typename T> int64_t atMemoryWriter::Write(const T &data) { return Write(&data, 1); }

#endif // atMemoryWriter_h__
