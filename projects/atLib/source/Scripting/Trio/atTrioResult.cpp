#include "atTrioResult.h"

atTrioResult::atTrioResult(const atTrioResult_Code &code)
  : m_code(code)
  , m_pMessage(nullptr)
{}

atTrioResult::atTrioResult(const atTrioResult_Code &code, const atString &message)
  : m_code(code)
  , m_pMessage(atNew<atString>(message))
{}

atTrioResult::atTrioResult(const atTrioResult &result)
{
  if (result.m_pMessage)
  {
    if (!m_pMessage)
      m_pMessage = atNew<atString>();
    *m_pMessage = *result.m_pMessage;
  }

  m_code = result.m_code;
}

atTrioResult::atTrioResult(atTrioResult &&result)
{
  std::swap(m_pMessage, result.m_pMessage);
  std::swap(m_code, result.m_code);
}

atTrioResult::~atTrioResult()
{
  if (m_pMessage)
    atDelete(m_pMessage);
}

atString atTrioResult::Message() const { return m_pMessage ? *m_pMessage : ""; }
const atTrioResult_Code& atTrioResult::Code() const { return m_code; }
