
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

enum atSceneComponentType : int64_t
{
  atSCT_All = INT64_MAX
};

class atScene;
class atSceneNode;

template<typename T> void atIsValidSceneComponentType();

class atSceneComponent
{
  friend atSceneNode;

public:
  virtual bool OnCreate() { return true; }
  virtual bool OnDestroy() { return true; }
  virtual bool OnUpdate(const double dt) { atUnused(dt); return true; }
  virtual bool OnDraw(const atMat4D &vp) { atUnused(vp); return true; }

  virtual int64_t TypeID() const = 0;

  template<typename T> bool Is() const;

  atScene* Scene();
  atSceneNode* Node();

  static int64_t NextTypeID();

protected:
  static int64_t m_nextTypeID;
  atSceneNode *m_pNode;
};

#include "atSceneComponent.inl"
#endif // atSceneComponent_h__
