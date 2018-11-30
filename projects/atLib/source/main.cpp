
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
#include "atBVH.h"


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
//
// Press 'L' to toggle lighting effects
//
// W - Forward
// S - Backward
// A - Left
// D - Right
// Alt - Slow Down
// Shift - Speed up
// Right Mouse + Mouse Move - Look

#include "atFontRenderer.h"
#include "atGraphicsModel.h"
#include "atRenderState.h"
#include "atCamera.h"

void ExampleRenderMesh(atVec2I wndSize = {800, 600}, bool useLighting = true)
{
  // Set the model being loaded
  const atString path = "assets/test/models/level.obj";

  // Set the windows clear colour
  const atVec4F clearColor = {0.3f, 0.3f, 0.3f, 1.0f};

  atLight light;
  
  // Import the model
  atGraphicsModel model(path);

  // Create a window
  atWindow window("Default Window", wndSize);

  // Create a camera
  atCamera camera(window, { 0, 1, 5 });
  
  // Main program loop
  while (atInput::Update(true)) // Process user inputs
  {
    // Update camera
    camera.Update(1.0);

    // Clear window
    window.Clear(clearColor);

    // Set Lighting Data
    model.SetLighting(light);
    model.EnableLighting(useLighting);
    model.SetCamera(camera.m_translation);
    useLighting = atInput::KeyPressed(atKC_L) ? !useLighting : useLighting;

    // Draw model
    model.Draw(camera.ProjectionMat() * camera.ViewMat());

    // Display rendered frame
    window.Swap();
  }
}

// -----------------------------------------------------------
// Demonstrates usage of atFontRenderer to draw text
//
// Press 'P' to toggle the use of Pivots
//
// atFontRenderer::Bake() is used to generate text draw data
//
// atFontRenderer::Draw() draws the text using the windows
// dimensions to build the orthographic projection matrix
//
// atFontRenderer provides TextSize() and TextWidth()
// to calculate the dimensions or on screen rectangle 
// of a piece of text.
//

void ExampleRenderText()
{
  const atString fontPath = "Assets/Fonts/RomanSerif.ttf";

  atWindow window("Font Renderer Example");

  // Set/Load a font (.ttf files)
  atFontRenderer::SetFont(fontPath);
  
  while (atInput::Update())
  {
    window.Clear({ 0.3f, 0.3f, 0.3f, 1.0f });

    // Enable/Disable pivots
    static bool usePivot = true;
    usePivot = atInput::KeyPressed(atKC_P) ? !usePivot : usePivot;
    
    // Bake Text
    if (usePivot)
    {
      atFontRenderer::Bake(0, 0, "Top Left (Pivot: 0.0, 0.0)");
      atFontRenderer::Bake(window.Width(), 0, "(Pivot: 1.0, 0.0) Top Right", { 1.f, 0.f });
      atFontRenderer::Bake(0, window.Height(), "Bottom Left (Pivot: 0.0, 1.0)", { 0.f, 1.f });
      atFontRenderer::Bake(window.Width(), window.Height(), "(Pivot: 1.0, 1.0) Bottom Right", { 1.f, 1.f });
      atFontRenderer::Bake(window.Width() / 2, window.Height() / 2, "(Pivot 0.5,0.5): Center", { .5f, .5f });
    }
    else
    {
      atFontRenderer::Bake(0, 0, "Top Left");
      atFontRenderer::Bake(window.Width(), 0, "Top Right");
      atFontRenderer::Bake(0, window.Height(), "Bottom Left");
      atFontRenderer::Bake(window.Width(), window.Height(), "Bottom Right");
      atFontRenderer::Bake(window.Width() / 2, window.Height() / 2, "Center");
    }

    // Render Text
    atFontRenderer::Draw(window);

    window.Swap();
  }
}

// -----------------------------------------------------------
// Sample code demonstrating how to Send and Receive data 
// using atSocket

#include "atSocket.h"

void ExampleSocketUsage()
{
  // Create a host socket to listen for connection requests
  atSocket host = atSocket::Host("1234");

  // Attempt to connect to the host
  atSocket client = atSocket::Connect("localhost", "1234");

  // Wait until the connection is available
  while (!host.CanAccept());

  // Accept the connection request from the client socket
  // and store the new connection
  atSocket con = host.Accept();

  atVector<uint8_t> msg = { 'H', 'e','l','l','o',' ', 'H','o','s','t' };
  atVector<uint8_t> recvMsg(msg.size() * 2, 0);

  // Send the host the message
  client.Write(msg.data(), msg.size());

  // Receive the client's message using the connection socket
  con.Read(recvMsg.data(), recvMsg.size());

  msg = { 'H', 'e','l','l','o',' ', 'C','l','i','e', 'n', 't' };

  // Send a response to the client
  con.Write(msg.data(), msg.size());

  // Read the host's response
  client.Read(recvMsg.data(), recvMsg.size());
}

// -----------------------------------------------------------
// Sample code demonstrating how to Send and Receive 
// streamable objects using atNetworkReader and 
// atNetworkWriter
//
// This example can be extended to stream any object in atLib
// that overloads the atStreamRead() and atStreamWrite()
// functions. 
//
// Some of these objects are:
//  - Most Primitive Types (see atReadStream.h and atWriteStream.h)
//  - atVector
//  - atString
//  - atFilename
//  - atHashMap
//  - atMesh
//  - atMaterial

#include "atNetworkReader.h"
#include "atNetworkWriter.h"

void ExampleNetworkStreaming()
{
  atString sendString = "Hello Read Stream";
  atString recvString;
  // Create a Network reader on Port 1234
  atNetworkReader reader("1234");

  // Create a Network writer connected to localhost on Port 1234
  atNetworkWriter writer("localhost", "1234");

  // Write "Hello Read Stream" using the Network Writer
  writer.Write(sendString);

  // Read "Hello Read Stream" using the Network Reader
  reader.Read(&recvString);
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

#include "atBVH.h"
#include "atIntersects.h"

int main(int argc, char **argv)
{
  atUnused(argc, argv);
  ExampleRenderText();
  // ExampleRenderMesh();
  // ExampleImportExportMesh();
  // ExampleSocketUsage();
  // ExampleNetworkStreaming();

  // atMesh mesh;
  // mesh.Import("assets/test/models/cube.obj");
  // 
  // atBVH<atTriangle<double>> bvh(mesh.GetTriangles());
  // 
  // double time = 0.0;
  // bvh.RayTrace(atRay<double>(atVec3F64(0, 0, -10), atVec3F64(0, 0, 1)), atMat4D::Identity(), &time);
  return atWindow_GetResult();
}
