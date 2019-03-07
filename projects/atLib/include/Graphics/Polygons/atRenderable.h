#ifndef atRenderable_h__
#define atRenderable_h__

#include "atRenderableCore.h"

class atRenderable
{
public:
  atRenderable() = default;
  ~atRenderable() = default;
  atRenderable(atRenderable &&move);
  atRenderable(const atRenderable &copy);

  bool DrawLines(const bool indexed = true);
  bool DrawTriangles(const bool indexed = true);
  bool DrawPoints(const bool indexed = true);
  bool DrawTriangleStrip();

  // Set Active Shader
  void SetShader(const atString &name);
  void SetShaderFromSource(const atString &pixel, const atString &vert, const atString &geom = "", const atString &hull = "");
  void SetSampler(const atString &name, const int64_t id);

  // Set texture units
  void SetTexture(const atString &name, const int64_t id);
  void SetTexture(const atString &name, const atString &path);

  // Set Vertex Data
  void SetAttribute(const atString &name, const atVector<float> &data);
  void SetAttribute(const atString &name, const atVector<int64_t> &data);
  void SetAttribute(const atString &name, const atVector<int32_t> &data);
  void SetAttribute(const atString &name, const atVector<atVec2F> &data);
  void SetAttribute(const atString &name, const atVector<atVec3F> &data);
  void SetAttribute(const atString &name, const atVector<atVec4F> &data);

  void SetIndices(const atString &name, const atVector<uint32_t> &data);

  // Set Shader Variables
  void SetUniform(const atString &name, const float val);
  void SetUniform(const atString &name, const int32_t val);
  void SetUniform(const atString &name, const int64_t val);
  void SetUniform(const atString &name, const uint64_t val);
  void SetUniform(const atString &name, const uint32_t val);

  void SetUniform(const atString &name, const atVec2F &val);
  void SetUniform(const atString &name, const atVec3F &val);
  void SetUniform(const atString &name, const atVec4F &val);
  void SetUniform(const atString &name, const atMat4F &val);

  // Set Shader Variables
  // Note: Converts to float32 internally
  void SetUniform(const atString &name, const double val);
  void SetUniform(const atString &name, const atVec2D &val);
  void SetUniform(const atString &name, const atVec3D &val);
  void SetUniform(const atString &name, const atVec4D &val);
  void SetUniform(const atString &name, const atMat4D &val);

  bool HasChannel(const atString &name);
  bool HasUniform(const atString &name);
  bool HasTexture(const atString &name);
  bool HasAttribute(const atString &name);

  atRenderableCore m_core;
};

#endif // atRenderable_h__
