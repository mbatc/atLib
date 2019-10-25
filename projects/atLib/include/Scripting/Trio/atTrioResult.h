#ifndef atTrioResult_h__
#define atTrioResult_h__

#include "atString.h"

enum atTrioResult_Code
{
  atTRC_Success,
  atTRC_Failure,
  atTRC_Unknown,
};

class atTrioResult
{
public:
  atTrioResult(const atTrioResult_Code &code);
  atTrioResult(const atTrioResult_Code &code, const atString &message);
  atTrioResult(const atTrioResult &code);
  atTrioResult(atTrioResult &&code);
  ~atTrioResult();

  const atTrioResult_Code& Code() const;
  atString Message() const;

protected:
  atTrioResult_Code m_code;
  atString *m_pMessage;
};

#endif // atTrioResult_h__
