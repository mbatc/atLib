
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

#include "atInput.h"
#include "atRenderState.h"
#include "atCamera.h"
#include "atGraphicsModel.h"
#include "atFile.h"
#include "atMeshParser.h"


//---------------------------------------------------------------------------------
// NOTE: This file is used for testing but does contain a few pieces of sample code
//       
//       main() is not going to stay well formatted as it will be used to test
//       snippets of code.
//
//       all other functions in this file should only exist to demonstrate 
//       functionality, be well formatted and easy to understand (meaning comments 
//       should be added if lines are unclear)
//
//---------------------------------------------------------------------------------


// -----------------------------------------------------------
// Demonstrates loading and rendering a model with simple user 
// input provided by atCamera

void ExampleRenderMesh(atVec2I wndSize = {800, 600})
{
  // Set the model being loaded
  const atString path = "assets/test/models/level.obj";

  // Set the windows clear colour
  const atVec4F clearColor = {0.3f, 0.3f, 0.3f, 1.0f};

  // Import the model
  atGraphicsModel model(path);

  // Create a window
  atWindow window("Default Window", wndSize);

  // Create a camera
  atCamera camera(window);

  // Main program loop
  while (atInput::Update(true)) // Process user inputs
  {
    // Update camera
    camera.Update(1.0);

    // Clear window
    window.Clear(clearColor);
    
    // Draw model
    model.Draw(camera.ProjectionMat() * camera.ViewMat());

    // Display rendered frame
    window.Swap();
  }
}

// -----------------------------------------------
// BELOW HERE IS GUARANTEED TO BE AN ABSOLUTE MESS
// -----------------------------------------------

// -------------------------------------------------------------
// Demonstrates simple imports/exports between different formats

// This shit doesn't deserve to be in the top half of the file as
// OBJ Writing doesn't work yet

void ExampleImportExportMesh()
{
  const atString path = "assets/test/models/level.obj";
  const atString outPath1 = "example/export.obj";
  const atString outPath2 = "example/export.atm";
  const atString outPath3 = "example/atmToObj.obj";

  atMesh mesh;

  // Import OBJ file
  mesh.Import(path);

  // OBJ Writing isn't supported yet but will be shortly
  // mesh.Export(outPath1);

  // Write to a AT's binary mesh format
  mesh.Export(outPath2);

  // Import AT's binary mesh format and export to .obj
  mesh.Import(outPath2);
  // mesh.Export(outPath3);
}

int main(int argc, char **argv)
{
  atUnused(argc, argv);
  // ExampleRenderMesh({1820, 820});
  // ExampleImportExportMesh();
  return atWindow_GetResult();
}
