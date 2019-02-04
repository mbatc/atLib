
// -----------------------------------------------------------------------------
// The MIT License
// 
// Copyright(c) 2018 Michael Batchelor, 
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// -----------------------------------------------------------------------------

#include "atSceneMeshRenderable.h"
#include "atSceneNode.h"

const int64_t atSceneMeshRenderable::typeID = atSceneComponent::NextTypeID();
int64_t atSceneMeshRenderable::TypeID() const { return typeID; }

bool atSceneMeshRenderable::OnDraw(const atMat4D &vp)
{
  m_model.Draw(vp, m_pNode->GlobalWorldMat());
  return true;
}

const atString& atSceneMeshRenderable::GetModelPath() { return m_path; }
bool atSceneMeshRenderable::SetModel(const atMesh &mesh) { m_path = mesh.m_sourceFile; return m_model.Import(mesh); }
bool atSceneMeshRenderable::SetModel(const atString &file) { m_path = file; return m_model.Import(file); }
