
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
    mtlDef += "\nnewmtl " + mat.GetName() + "\n";
    mtlDef += "Kd " + atPrint::Vector(mat.GetColour(atMP_Diffuse).xyz() * mat.GetColour(atMP_Diffuse).w) + "\n";
    mtlDef += "Ks " + atPrint::Vector(mat.GetColour(atMP_Specular).xyz() * mat.GetColour(atMP_Specular).w) + "\n";
    mtlDef += "Ka " + atPrint::Vector(mat.GetColour(atMP_Ambient).xyz() * mat.GetColour(atMP_Ambient).w) + "\n";
    mtlDef += "d " + atPrint::Float(mat.GetValue(atMP_Alpha)) + "\n";
    mtlDef += "Ns " + atPrint::Float(mat.GetValue(atMP_SpecularPower)) + "\n";

    for (int64_t i = 0; i < mat.LayerCount(); ++i)
    {
      if (mat.HasTexture(atMP_Diffuse, i))           mtlDef += "map_Kd " + mat.GetTexture(atMP_Diffuse, i) + "\n";
      if (mat.HasTexture(atMP_Ambient, i))           mtlDef += "map_Ka " + mat.GetTexture(atMP_Ambient, i) + "\n";
      if (mat.HasTexture(atMP_Specular, i))          mtlDef += "map_Ks " + mat.GetTexture(atMP_Specular, i) + "\n";
      if (mat.HasTexture(atMP_SpecularHighlight, i)) mtlDef += "map_Ns " + mat.GetTexture(atMP_SpecularHighlight, i) + "\n";
      if (mat.HasTexture(atMP_Bump, i))              mtlDef += "map_Bump " + mat.GetTexture(atMP_Bump, i) + "\n";
      if (mat.HasTexture(atMP_Displacement, i))      mtlDef += "disp " + mat.GetTexture(atMP_Displacement, i) + "\n";
      if (mat.HasTexture(atMP_Alpha, i))             mtlDef += "map_d " + mat.GetTexture(atMP_Alpha, i) + "\n";
    }

    mtlFile.WriteText(mtlDef);
  }
  return true;
}