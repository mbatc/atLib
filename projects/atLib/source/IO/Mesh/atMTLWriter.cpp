
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

#include "atMTLParser.h"
#include "atFile.h"
#include "atPrint.h"

bool atMTLWriter::Write(const atFilename &file, const atVector<atMaterial> &materials)
{
  atFile mtlFile;
  if (!mtlFile.Open(file, atFM_Write))
    return false;

  mtlFile.WriteText(atString("# atLib MTL File\n# Material Count: ") + materials.size() + "\n# https://github.com/mbatc/atLib\n");

  atString mtlDef;
  for (const atMaterial &mat : materials)
  {
    mtlDef = "";
    mtlDef += "\nnewmtl " + mat.m_name + "\n";
    mtlDef += "Kd " + atPrint::Vector(mat.m_cDiffuse.xyz() * mat.m_cDiffuse.w) + "\n";
    mtlDef += "Ks " + atPrint::Vector(mat.m_cSpecular.xyz() * mat.m_cSpecular.w) + "\n";
    mtlDef += "Ka " + atPrint::Vector(mat.m_cAmbient.xyz() * mat.m_cAmbient.w) + "\n";
    mtlDef += "d " + atPrint::Float(mat.m_alpha) + "\n";
    mtlDef += "Ns " + atPrint::Float(mat.m_specularPower) + "\n";
    for (const atFilename &map : mat.m_tDiffuse)
      mtlDef += "map_Ks " + map.Path() + "\n";
    for (const atFilename &map : mat.m_tAmbient)
      mtlDef += "map_Ka " + map.Path() + "\n";
    for (const atFilename &map : mat.m_tSpecular)
      mtlDef += "map_Ks " + map.Path() + "\n";
    for (const atFilename &map : mat.m_tSpecularHigh)
      mtlDef += "map_Ns " + map.Path() + "\n";
    for (const atFilename &map : mat.m_tBump)
      mtlDef += "map_Bump " + map.Path() + "\n";
    for (const atFilename &map : mat.m_tDisplacement)
      mtlDef += "disp " + map.Path() + "\n";
    for (const atFilename &map : mat.m_tAlpha)
      mtlDef += "map_d " + map.Path() + "\n";
    mtlFile.WriteText(mtlDef);
  }
  return true;
}