#include "atGLBuffer.h"
#include "atOpenGL.h"

int64_t atGLBuffer::Target()
{
  switch (m_mapping)
  {
  case atBT_ShaderData: return GL_UNIFORM_BUFFER;
  case atBT_VertexData: return GL_ARRAY_BUFFER;
  case atBT_IndexData: return GL_ELEMENT_ARRAY_BUFFER;
  }
  return GL_BUFFER;
}

bool atGLBuffer::Delete()
{
  if (GFXResource() == 0)
    return false;
  uint32_t bufID = (uint32_t)(int64_t)GFXResource();
  glDeleteBuffers(1, &bufID);
  m_pResource = 0;
  return true;
}

bool atGLBuffer::Upload()
{
  if (GFXResource() != 0)
    return true;

  uint32_t bufID = 0;
  glGenBuffers(1, &bufID);
  m_pResource = (void*)(int64_t)bufID;

  if (!m_pResource)
    return false;

  GLenum target = (GLenum)Target();
  glBindBuffer(target, bufID);
  glBufferData(target, (GLsizeiptr)m_data.size(), m_data.data(), GL_STATIC_DRAW);

#ifdef _DEBUG
  atString name;
  switch (m_mapping)
  {
  case atBT_IndexData: name = "Index Data"; break;
  case atBT_VertexData: name = "Index Data"; break;
  case atBT_ShaderData: name = "Index Data"; break;
  default: name = "Unknown Data"; break;
  }
  name += " Buffer";
  glObjectLabel(GL_BUFFER, bufID, (GLsizei)name.length(), name.c_str());
#endif

  return true;
}

bool atGLBuffer::Update()
{
  GLenum target = (GLenum)Target();
  uint32_t bufID = (uint32_t)(int64_t)GFXResource();

  glBindBuffer(target, bufID);
  glBufferSubData(target, 0, m_data.size(), m_data.data());
  return true;
}
