#include "atShader.h"
#include "atFile.h"

void atShader::SetSource(const atString &src)
{
  m_isFile = false;
  m_src = src;
}

void atShader::SetSourceFile(const atFilename &file)
{
  m_src = file.Path();
  m_isFile = true;
}

atString atShader::LoadSource() const { return m_isFile ? atFile::ReadText(m_src) : m_src; }
