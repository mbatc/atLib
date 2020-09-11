#include "atGLBuffer.h"
#include "atOpenGL.h"

atGLBuffer::atGLBuffer(const atBufferType &type, const int64_t &size)
  : atGPUBuffer(type)
{
  Resize(size);
}

int64_t atGLBuffer::Target() { return Target(m_mapping); }

int64_t atGLBuffer::Target(const atBufferType &target)
{
  switch (target)
  {
  case atBT_ShaderData: return GL_UNIFORM_BUFFER;
  case atBT_VertexData: return GL_ARRAY_BUFFER;
  case atBT_IndexData: return GL_ELEMENT_ARRAY_BUFFER;
  }
  return GL_NONE;
}

bool atGLBuffer::Delete()
{
  if (NativeResource() == 0)
    return false;
  uint32_t bufID = (uint32_t)(int64_t)NativeResource();
  glDeleteBuffers(1, &bufID);
  m_pResource = 0;
  return true;
}

bool atGLBuffer::Upload()
{
  if (NativeResource() && m_bufferSize <= m_gpuBufferSize)
    return true;

  uint32_t bufID = (uint32_t)(int64_t)NativeResource();
  if (!NativeResource())
  {
    glGenBuffers(1, &bufID);
    m_pResource = (void *)(int64_t)bufID;
    if (!m_pResource)
      return false;
  }

  Bind(m_mapping);
  GLenum target = (GLenum)Target(m_mapping);
  glBufferData(target, m_bufferSize, nullptr, GL_STATIC_DRAW);
  m_gpuBufferSize = m_bufferSize;

#ifdef _DEBUG
  atString name;
  switch (m_mapping)
  {
  case atBT_IndexData: name = "Index Data"; break;
  case atBT_VertexData: name = "Vertex Data"; break;
  case atBT_ShaderData: name = "Shader Data"; break;
  default: name = "Unknown Data"; break;
  }
  name += " Buffer";
  glObjectLabel(GL_BUFFER, bufID, (GLsizei)name.length(), name.c_str());
#endif

  return true;
}

bool atGLBuffer::Resize(const int64_t &size)
{
  m_bufferSize = size;
  return Upload();
}

void *atGLBuffer::Map(const atGPUBuffer_MapFlags &flags)
{
  if (m_bufferSize == 0)
    return nullptr;

  if (m_pMappedPtr)
  {
    ++m_mappedCount;
    return m_pMappedPtr;
  }

  GLenum accessFlags = GL_NONE;
  if (flags == atGB_MF_ReadWrite)
    accessFlags = GL_READ_WRITE;
  else if (flags == atGB_MF_Read)
    accessFlags = GL_READ_ONLY;
  else if (flags == atGB_MF_Write)
    accessFlags = GL_WRITE_ONLY;

  if (accessFlags == GL_NONE)
    return nullptr;

  ++m_mappedCount;
  Bind(m_mapping);
  m_pMappedPtr = glMapBuffer((GLenum)Target(m_mapping), accessFlags);
  return m_pMappedPtr;
}

bool atGLBuffer::Unmap()
{
  if (!m_pMappedPtr)
    return false;
  if (--m_mappedCount == 0)
  {
    Bind(m_mapping);
    glUnmapBuffer((GLenum)Target(m_mapping));
    m_pMappedPtr = 0;
  }
  return true;
}

void atGLBuffer::Bind(const atBufferType &target)
{
  uint32_t bufferID = (uint32_t)(int64_t)NativeResource();
  glBindBuffer((GLenum)Target(), bufferID);
}
