#include "atRenderable.h"

atRenderable::atRenderable(atRenderable &&move) : m_core(move.m_core) {}
atRenderable::atRenderable(const atRenderable &copy) : m_core(copy.m_core) {}

bool atRenderable::DrawLines(const bool indexed) { return m_core.Draw(indexed ? atRPT_LineList : atRPT_LineListAdj); }
bool atRenderable::DrawPoints(const bool indexed) { return m_core.Draw(indexed ? atRPT_PointList : atRPT_PointListAdj); }
bool atRenderable::DrawTriangles(const bool indexed) { return m_core.Draw(indexed ? atRPT_TriangleList : atRPT_TriangleListAdj); }
bool atRenderable::DrawTriangleStrip() { return m_core.Draw(atRPT_TriangleStrip); }

void atRenderable::SetShader(const atString &name) { m_core.SetShader(name); }
void atRenderable::SetShaderFromSource(const atString &pixel, const atString &vert, const atString &geom, const atString &hull) { m_core.SetShaderFromSource(pixel, vert, geom, hull); }
void atRenderable::SetSampler(const atString & name, const int64_t id) { m_core.SetChannel(name, id, atRRT_Sampler); }

void atRenderable::SetTexture(const atString &name, const int64_t id) { m_core.SetChannel(name, id, atRRT_Texture); }
void atRenderable::SetTexture(const atString &name, const atString &path) { m_core.SetChannel(name, path, atRRT_Texture); }

void atRenderable::SetAttribute(const atString &name, const atVector<float> &data) { m_core.SetChannel(name, data, atRRT_VertexData); }
void atRenderable::SetAttribute(const atString &name, const atVector<int64_t> &data) { m_core.SetChannel(name, data, atRRT_VertexData); }
void atRenderable::SetAttribute(const atString &name, const atVector<int32_t> &data) { m_core.SetChannel(name, data, atRRT_VertexData); }
void atRenderable::SetAttribute(const atString &name, const atVector<atVec2F> &data) { m_core.SetChannel(name, data, atRRT_VertexData); }
void atRenderable::SetAttribute(const atString &name, const atVector<atVec3F> &data) { m_core.SetChannel(name, data, atRRT_VertexData); }
void atRenderable::SetAttribute(const atString &name, const atVector<atVec4F> &data) { m_core.SetChannel(name, data, atRRT_VertexData); }

void atRenderable::SetIndices(const atString &name, const atVector<uint32_t> &data) { m_core.SetChannel(name, data, atRRT_Indices); }

void atRenderable::SetUniform(const atString &name, const float val) { m_core.SetChannel(name, val, atRRT_Variable); }
void atRenderable::SetUniform(const atString &name, const int32_t val) { m_core.SetChannel(name, val, atRRT_Variable); }
void atRenderable::SetUniform(const atString &name, const int64_t val) { m_core.SetChannel(name, val, atRRT_Variable); }
void atRenderable::SetUniform(const atString &name, const uint64_t val) { m_core.SetChannel(name, val, atRRT_Variable); }
void atRenderable::SetUniform(const atString &name, const uint32_t val) { m_core.SetChannel(name, val, atRRT_Variable); }
void atRenderable::SetUniform(const atString &name, const atVec2F &val) { m_core.SetChannel(name, val, atRRT_Variable); }
void atRenderable::SetUniform(const atString &name, const atVec3F &val) { m_core.SetChannel(name, val, atRRT_Variable); }
void atRenderable::SetUniform(const atString &name, const atVec4F &val) { m_core.SetChannel(name, val, atRRT_Variable); }
void atRenderable::SetUniform(const atString &name, const atMat4F &val) { m_core.SetChannel(name, val, atRRT_Variable); }
void atRenderable::SetUniform(const atString &name, const double val) { m_core.SetChannel(name, (float)val, atRRT_Variable); }
void atRenderable::SetUniform(const atString &name, const atVec2D &val) { m_core.SetChannel(name, atVec2F(val), atRRT_Variable); }
void atRenderable::SetUniform(const atString &name, const atVec3D &val) { m_core.SetChannel(name, atVec3F(val), atRRT_Variable); }
void atRenderable::SetUniform(const atString &name, const atVec4D &val) { m_core.SetChannel(name, atVec4F(val), atRRT_Variable); }
void atRenderable::SetUniform(const atString &name, const atMat4D &val) { m_core.SetChannel(name, atMat4F(val), atRRT_Variable); }

bool atRenderable::HasChannel(const atString &name) { return m_core.HasResource(name); }
bool atRenderable::HasUniform(const atString &name) { return HasChannel(name) ? m_core.GetResource(name).type == atRRT_Variable : false; }
bool atRenderable::HasTexture(const atString &name) { return HasChannel(name) ? m_core.GetResource(name).type == atRRT_Texture : false; }
bool atRenderable::HasAttribute(const atString &name) { return HasChannel(name) ? m_core.GetResource(name).type == atRRT_VertexData : false; }
