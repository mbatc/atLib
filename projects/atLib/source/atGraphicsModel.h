
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

#ifndef atGraphicsModel_h__
#define atGraphicsModel_h__

#include "atRenderable.h"

class atGraphicsModel
{
public:
  atGraphicsModel();
  atGraphicsModel(const atMesh &mesh);
  atGraphicsModel(const atFilename &filename, const bool genTangents = true);
  atGraphicsModel(const atGraphicsModel &copy);
  atGraphicsModel(atGraphicsModel &&move);
  ~atGraphicsModel();

  void Draw(const atMat4D &MVP);

  bool Import(const atMesh &mesh);
  bool Import(const atFilename &filename, const bool genTangents = true);

  atVector<atRenderable> m_mesh;
};

#endif // atGraphicsModel_h__
