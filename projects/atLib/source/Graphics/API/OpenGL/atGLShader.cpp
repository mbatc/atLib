#include "atGLShader.h"
#include "atOpenGL.h"

atGLShader::atGLShader(const atString &src, const atPipelineStage &stage) : atShader(src, stage) {}

bool atGLShader::Bind()
{
  return false;
}

bool atGLShader::Upload()
{
  if (NativeResource())
    return true;

  uint32_t glShaderID = glCreateShader((GLenum)Target());
  if (glShaderID == 0)
    return false;

  const char *src = m_src.c_str();
  int len = (int)m_src.length();
  glShaderSource(glShaderID, 1, &src, &len);
  glCompileShader(glShaderID);

  GLint result;
  glGetShaderiv(glShaderID, GL_COMPILE_STATUS, &result);

  if (result == GL_FALSE)
  {
    GLint bufLen;
    glGetShaderiv(glShaderID, GL_INFO_LOG_LENGTH, &bufLen);
    atVector<char> buffer(bufLen + 1, 0);
    glGetShaderInfoLog(glShaderID, (GLint)buffer.size(), nullptr, buffer.data());
    atRelAssert(false, atString("Failed to compile shader: \n") + buffer.data());
    glDeleteShader(glShaderID);
    return false;
  }

  m_pResource = (void*)(int64_t)glShaderID;
  return true;
}

bool atGLShader::Delete()
{
  if (!NativeResource())
    return false;

  uint32_t glShaderID = (uint32_t)(int64_t)NativeResource();
  glDeleteShader(glShaderID);
  return true;
}

int64_t atGLShader::Target()
{
  switch (Stage())
  {
  case atPS_Vertex: return GL_VERTEX_SHADER;
  case atPS_Fragment: return GL_FRAGMENT_SHADER;
  case atPS_Compute: return GL_COMPUTE_SHADER;
  case atPS_Geometry: return GL_GEOMETRY_SHADER;
  case atPS_Hull: return GL_TESS_CONTROL_SHADER;
  case atPS_Domain: return GL_TESS_EVALUATION_SHADER;
  }
  return GL_NONE;
}
