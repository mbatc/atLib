#include "atGLPrgm.h"
#include "atOpenGL.h"
#include "atGLShader.h"
#include "atGraphics.h"
#include "atGLTexture.h"
#include "atGLBuffer.h"
#include "atFormat.h"

atTypeDesc _GetTypeDesc(GLenum glType)
{
  switch (glType)
  {
  case GL_FLOAT:             return atGetTypeDesc<float>();
  case GL_FLOAT_VEC2:        return atGetTypeDesc<atVec2F>();
  case GL_FLOAT_VEC3:        return atGetTypeDesc<atVec3F>();
  case GL_FLOAT_VEC4:        return atGetTypeDesc<atVec4F>();
  case GL_FLOAT_MAT2:        return atGetTypeDesc<void>();
  case GL_FLOAT_MAT3:        return atGetTypeDesc<void>();
  case GL_FLOAT_MAT4:        return atGetTypeDesc<atMat4F>();
  case GL_FLOAT_MAT2x3:      return atGetTypeDesc<void>();
  case GL_FLOAT_MAT2x4:      return atGetTypeDesc<void>();
  case GL_FLOAT_MAT3x2:      return atGetTypeDesc<void>();
  case GL_FLOAT_MAT3x4:      return atGetTypeDesc<void>();
  case GL_FLOAT_MAT4x2:      return atGetTypeDesc<void>();
  case GL_FLOAT_MAT4x3:      return atGetTypeDesc<void>();
  case GL_INT:               return atGetTypeDesc<int32_t>();
  case GL_INT_VEC2:          return atGetTypeDesc<atVec2I>();
  case GL_INT_VEC3:          return atGetTypeDesc<atVec3I>();
  case GL_INT_VEC4:          return atGetTypeDesc<atVec4I>();
  case GL_UNSIGNED_INT:      return atGetTypeDesc<uint32_t>();
  case GL_UNSIGNED_INT_VEC2: return atGetTypeDesc<atVector2<uint32_t>>();
  case GL_UNSIGNED_INT_VEC3: return atGetTypeDesc<atVector3<uint32_t>>();
  case GL_UNSIGNED_INT_VEC4: return atGetTypeDesc<atVector4<uint32_t>>();
  case GL_DOUBLE:            return atGetTypeDesc<double>();
  case GL_DOUBLE_VEC2:       return atGetTypeDesc<atVec2D>();
  case GL_DOUBLE_VEC3:       return atGetTypeDesc<atVec3D>();
  case GL_DOUBLE_VEC4:       return atGetTypeDesc<atVec4D>();
  case GL_DOUBLE_MAT2:       return atGetTypeDesc<void>();
  case GL_DOUBLE_MAT3:       return atGetTypeDesc<void>();
  case GL_DOUBLE_MAT4:       return atGetTypeDesc<atMat4D>();
  case GL_DOUBLE_MAT2x3:     return atGetTypeDesc<void>();
  case GL_DOUBLE_MAT2x4:     return atGetTypeDesc<void>();
  case GL_DOUBLE_MAT3x2:     return atGetTypeDesc<void>();
  case GL_DOUBLE_MAT3x4:     return atGetTypeDesc<void>();
  case GL_DOUBLE_MAT4x2:     return atGetTypeDesc<void>();
  case GL_DOUBLE_MAT4x3:     return atGetTypeDesc<void>();
  }

  return atGetTypeDesc<void>();
}

atGFXTextureType _GetTexTypeDesc(GLenum glType)
{
  switch (glType)
  {
  case GL_SAMPLER_1D:
  case GL_SAMPLER_1D_ARRAY:
  case GL_INT_SAMPLER_1D:
  case GL_INT_SAMPLER_1D_ARRAY:
  case GL_UNSIGNED_INT_SAMPLER_1D:
  case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY:
  case GL_IMAGE_1D:
  case GL_IMAGE_1D_ARRAY:
  case GL_INT_IMAGE_1D:
  case GL_INT_IMAGE_1D_ARRAY:
  case GL_INT_IMAGE_2D_ARRAY:
  case GL_UNSIGNED_INT_IMAGE_1D:
  case GL_UNSIGNED_INT_IMAGE_1D_ARRAY:
  case GL_SAMPLER_1D_SHADOW:
  case GL_SAMPLER_1D_ARRAY_SHADOW:
    return atGFX_TT_1D;

  case GL_SAMPLER_2D:
  case GL_SAMPLER_2D_RECT:
  case GL_SAMPLER_2D_ARRAY:
  case GL_SAMPLER_2D_MULTISAMPLE:
  case GL_SAMPLER_2D_MULTISAMPLE_ARRAY:
  case GL_INT_SAMPLER_2D:
  case GL_INT_SAMPLER_2D_RECT:
  case GL_INT_SAMPLER_2D_ARRAY:
  case GL_INT_SAMPLER_2D_MULTISAMPLE:
  case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
  case GL_UNSIGNED_INT_SAMPLER_2D:
  case GL_UNSIGNED_INT_SAMPLER_2D_RECT:
  case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY:
  case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE:
  case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
  case GL_IMAGE_2D:
  case GL_IMAGE_2D_RECT:
  case GL_IMAGE_2D_ARRAY:
  case GL_IMAGE_2D_MULTISAMPLE:
  case GL_IMAGE_2D_MULTISAMPLE_ARRAY:
  case GL_INT_IMAGE_2D:
  case GL_INT_IMAGE_2D_RECT:
  case GL_INT_IMAGE_2D_MULTISAMPLE:
  case GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY:
  case GL_UNSIGNED_INT_IMAGE_2D:
  case GL_UNSIGNED_INT_IMAGE_2D_RECT:
  case GL_UNSIGNED_INT_IMAGE_2D_ARRAY:
  case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE:
  case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY:
  case GL_SAMPLER_2D_SHADOW:
  case GL_SAMPLER_2D_RECT_SHADOW:
  case GL_SAMPLER_2D_ARRAY_SHADOW:
    return atGFX_TT_2D;

  case GL_SAMPLER_3D:
  case GL_INT_SAMPLER_3D:
  case GL_UNSIGNED_INT_SAMPLER_3D:
  case GL_IMAGE_3D:
  case GL_INT_IMAGE_3D:
  case GL_UNSIGNED_INT_IMAGE_3D:
    return atGFX_TT_3D;
  }

  return atGFX_TT_None;
}

bool atGLPrgm::BindIndices(atGFXBufferInterface *pBuffer)
{
  atGLBuffer *pGLBuffer = (atGLBuffer*)pBuffer;
  if (pBuffer->Type() != atBT_IndexData)
    return false;
 
  glBindBuffer((GLenum)pGLBuffer->Target(), (uint32_t)(int64_t)pGLBuffer->GFXResource());
  m_indicesType = pGLBuffer->Desc().type;
  m_indexCount = pGLBuffer->Count();
  return true;
}

bool atGLPrgm::BindAttribute(const atString &name, atGFXBufferInterface *pBuffer)
{
  AttributeDesc *pDesc = m_attributes.TryGet(name);
  if (!pDesc)
    return false;

  atGLBuffer *pGLBuffer = (atGLBuffer*)pBuffer;
  if (pBuffer->Type() != atBT_VertexData)
    return false;

  m_vertexCount = atMin(pBuffer->Count(), m_vertexCount);
  glBindBuffer((GLenum)pGLBuffer->Target(), (uint32_t)(int64_t)pGLBuffer->GFXResource());
  glEnableVertexAttribArray((GLuint)pDesc->glLoc);
  glVertexAttribPointer((GLuint)pDesc->glLoc, (GLint)pBuffer->Desc().width, (GLenum)atFormat::GLType(pBuffer->Desc().type), GL_FALSE, 0, 0);
  return true;
}

bool atGLPrgm::BindTexture(const atString &name, atGFXTexInterface *pTexture)
{
  TexDesc *pDesc = m_textures.TryGet(name);
  if (!pDesc)
    return false;

  atGLTexture *pGLTex = (atGLTexture*)pTexture;
  if (pDesc->unit == -1)
  {
    // Set sampler texture unit
    pDesc->unit = m_activeTextures++;
    glUniform1i((GLint)pDesc->glLoc, (GLint)pDesc->unit);
  }

  // Bind texture to texture unit
  glActiveTexture((GLenum)(GL_TEXTURE0 + pDesc->unit));
  pGLTex->Bind();
  return true;
}

bool atGLPrgm::BindSampler(const atString &name, atGFXSamplerInterface *pSampler) { return false; }

bool atGLPrgm::SetUniform(const atString &name, const void *pData, const atTypeDesc &info)
{
  VarDesc *pDesc = m_uniforms.TryGet(name);
  if (!pDesc)
    return false;

  uint32_t glLoc = (uint32_t)pDesc->glLoc;
  float *f = (float*)pData;
  int32_t *i = (int32_t*)pData;
  uint32_t *ui = (uint32_t*)pData;
  GLsizei glCount = (GLsizei)info.count;
  
  switch (info.type)
  {
  case atType_Float32:
    switch (info.width)
    {
    case 1: glUniform1fv(glLoc, glCount, f); break;
    case 2: glUniform2fv(glLoc, glCount, f); break;
    case 3: glUniform3fv(glLoc, glCount, f); break;
    case 4: glUniform4fv(glLoc, glCount, f); break;
    case 16: glUniformMatrix4fv(glLoc, glCount, GL_FALSE, f);
    default: return false;
    }
    break;

  case atType_Int32:
    switch (info.width)
    {
    case 1: glUniform1iv(glLoc, glCount, i); break;
    case 2: glUniform2iv(glLoc, glCount, i); break;
    case 3: glUniform3iv(glLoc, glCount, i); break;
    case 4: glUniform4iv(glLoc, glCount, i); break;
    default: return false;
    }
    break;

  case atType_Uint32:
    switch (info.width)
    {
    case 1: glUniform1uiv(glLoc, glCount, ui); break;
    case 2: glUniform2uiv(glLoc, glCount, ui); break;
    case 3: glUniform3uiv(glLoc, glCount, ui); break;
    case 4: glUniform4uiv(glLoc, glCount, ui); break;
    default: return false;
    }
    break;

  default: return false;
  }

  return true;
}

bool atGLPrgm::Draw(const bool &indexedMode, const atGFX_PrimitiveType &primType, const int64_t &elementCount, const int64_t &elementOffset)
{
  atOpenGL *pGL = (atOpenGL*)atGraphics::GetCtx();
  if (!pGL)
    return false;

  if (indexedMode && m_activeTextures != atType_Unknown)
    pGL->DrawIndexed(elementCount < 0 ? m_indexCount : elementCount, elementOffset, 0, primType, m_indicesType);
  else
    pGL->Draw(elementCount < 0 ? m_vertexCount : elementCount, elementOffset, primType);

  return true;
}

bool atGLPrgm::Delete()
{
  if (!GFXResource())
    return false;

  uint32_t prgmID = (uint32_t)(int64_t)GFXResource();
  uint32_t vaoID = (uint32_t)m_vao;
  
  glDeleteProgram(prgmID);
  glDeleteVertexArrays(1, &vaoID);

  m_vao = 0;
  m_pResource = 0;
  m_activeTextures = 0;
  m_indexCount = INT64_MAX;
  m_vertexCount = INT64_MAX;
  m_attributes.Clear();
  m_textures.Clear();
  m_uniforms.Clear();
  return true;
}

bool atGLPrgm::Upload()
{
  if (GFXResource())
    return true;
  
  if (!m_pStages[atPS_Vertex] || !m_pStages[atPS_Fragment])
    return false;

  uint32_t glPrgmID = 0;
  glPrgmID = glCreateProgram();

  if (glPrgmID == 0)
    return false;

  for (std::shared_ptr<atGFXShaderInterface> pShader : m_pStages)
  {
    atGLShader *pGLShader = (atGLShader*)pShader.get();

    if (!pGLShader)
      continue;

    if (pShader->Upload())
      glAttachShader(glPrgmID, (uint32_t)(int64_t)pGLShader->GFXResource());
    else
      return false;
  }
  
  glLinkProgram(glPrgmID);

  GLint result = 1;
  glGetProgramiv(glPrgmID, GL_LINK_STATUS, &result);

  if (result == GL_FALSE)
  {
    GLint bufLen = 0;
    glGetProgramiv(glPrgmID, GL_INFO_LOG_LENGTH, &bufLen);
    atVector<char> buffer(bufLen + 1, 0);
    glGetProgramInfoLog(glPrgmID, (GLint)buffer.size(), 0, buffer.data());
    atRelAssert(false, atString("Failed to link shader program: \n") + buffer.data());
  }

  m_pResource = (void*)(int64_t)glPrgmID;

  GLuint vaoID;
  glGenVertexArrays(1, &vaoID);
  m_vao = vaoID;
  
  Reflect();
  return true;
}

bool atGLPrgm::Bind()
{
  if (!Upload())
    return false;

  m_indicesType = atType_Unknown;
  m_indexCount = INT64_MAX;
  m_vertexCount = INT64_MAX;
  glUseProgram((uint32_t)(int64_t)GFXResource());
  glBindVertexArray((GLuint)m_vao);
  return true;
}

void atGLPrgm::Reflect()
{
  uint32_t glPrgmID = (uint32_t)(int64_t)GFXResource();

  atVector<char> nameBuffer;

  GLint attrCount = 0;
  GLint attMaxLen = 0;
  glGetProgramiv(glPrgmID, GL_ACTIVE_ATTRIBUTES, &attrCount);
  glGetProgramiv(glPrgmID, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &attMaxLen);
  nameBuffer.resize(attMaxLen, 0);
  for (int64_t i = 0; i < attrCount; ++i)
  {
    GLsizei attribSize = 0;
    GLenum attribType = 0;

    glGetActiveAttrib(glPrgmID, (GLuint)i, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, nullptr, &attribSize, &attribType, nameBuffer.data());
    AttributeDesc desc;
    desc.glLoc = glGetAttribLocation(glPrgmID, nameBuffer.data());
    desc.name = nameBuffer.data();
    desc.desc = _GetTypeDesc(attribType);
    m_attributes.Add(nameBuffer.data(), desc);
  }

  GLint uniformCount = 0;  
  GLint uniMaxLen = 0;
  glGetProgramiv(glPrgmID, GL_ACTIVE_UNIFORMS, &uniformCount);
  glGetProgramiv(glPrgmID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniMaxLen);
  nameBuffer.clear();
  nameBuffer.resize(uniMaxLen, 0);
  for (int64_t i = 0; i < uniformCount; ++i)
  {
    GLsizei uniSize = 0;
    GLenum uniType = 0;

    glGetActiveUniform(glPrgmID, (GLuint)i, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, nullptr, &uniSize, &uniType, nameBuffer.data());
    
    atTypeDesc typeDesc = _GetTypeDesc(uniType);
    if (typeDesc != atGetTypeDesc<void>())
    {
      VarDesc desc;
      desc.desc = typeDesc;
      desc.name = nameBuffer.data();
      desc.glLoc = glGetUniformLocation(glPrgmID, nameBuffer.data());
      m_uniforms.Add(nameBuffer.data(), desc);
    }
    else
    {
      TexDesc desc;
      desc.type = _GetTexTypeDesc(uniType);

      if (desc.type != atGFX_TT_None)
      {
        desc.name = nameBuffer.data();
        desc.glLoc = glGetUniformLocation(glPrgmID, nameBuffer.data());
        m_textures.Add(nameBuffer.data(), desc);
      }
    }
  }
}

bool atGLPrgm::HasUniform(const atString &name) { return m_uniforms.Contains(name); }
