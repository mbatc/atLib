
// -----------------------------------------------------------------------------
// The MIT License
// 
// Copyright(c) 2020 Michael Batchelor, 
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

#include "atFileSystem.h"
#include "atMeshParser.h"
#include "atOBJParser.h"
#include "atFile.h"

static bool _WriteATM(const atFilename &filename, const atMesh &mesh)
{
  atFile file;
  if (!file.Open(filename, atFM_Write | atFM_Binary))
    return false;
  return file.Write(mesh) != 0;
}

bool atMeshWriter::Write(const atFilename &file, const atMesh &mesh)
{
  atFileSystem::CreateFolders(file.Directory());
  atFileExtension ext = atFileCommon::FileExtension(file.Extension());
  switch (ext)
  {
  case atFE_Obj: return atOBJWriter::Write(file, mesh);
  case atFE_Atm: return _WriteATM(file, mesh);
  }
  return false;
}