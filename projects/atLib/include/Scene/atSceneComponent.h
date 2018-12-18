
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

#ifndef atSceneComponent_h__
#define atSceneComponent_h__

#include "atMath.h"

class atSceneNode;

enum atSceneComponentType : int64_t
{
  atSCT_None = 1 << 0,
  atSCT_MeshRenderable = 1 << 1,
  atSCT_Script = 1 << 2,
  atSCT_Camera = 1 << 3,
  atSCT_Collidable = 1 << 4,
  atSCT_Effect = 1 << 5,
  atSCT_Skybox = 1 << 6,
  atSCT_All = INT64_MAX
};

class atSceneComponent
{
  friend atSceneNode;

public:
  virtual bool Update(const double dt) { return true; }
  virtual bool Draw(const atMat4D &vp) { return true; }

  virtual int64_t TypeID() const = 0;

protected:
  atSceneNode *m_pNode;
};

#endif // atSceneComponent_h__
