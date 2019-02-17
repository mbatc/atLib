
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
#include <time.h>

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

#include "atPrimitiveRenderer.h"
#include "atGraphicsModel.h"
#include "atRenderState.h"
#include "atSceneCamera.h"

void ExampleRenderMesh(atVec2I wndSize = {800, 600}, bool useLighting = true)
{
  atPrimitiveRenderer::SetFont(atFilename("assets/fonts/RomanSerif.ttf"));

  // Set the model being loaded
  const atString path = "assets/test/models/bumpmap.obj";

  // Set the windows clear colour
  const atVec4F clearColor = atVec4F(.3f, .3f, .3f, 1.f);
  
  atLight light;
  light.m_diffuseColor = atVec4F(0.8f, 0.6f, 0.5f, 1.0f);
  light.m_ambientColor = atVec4F(0.3f, 0.3f, 0.3f, 1.0f);
  light.m_specularColor = atVec4F(0.8f, 0.6f, 0.5f, 1.0f);
  
  // Import the model
  atGraphicsModel model(path);

  // Create a window
  atWindow window("Default Window", wndSize);

  // Create a camera
  atSimpleCamera camera(&window, { 0, 1, 5 });
  camera.m_moveSpeed = 1.0f;

  // Main program loop
  atRenderState rs;

  // A Render target must be set before attempting to render
  // this can be an atWindow or atRenderTarget with a color/depth texture attached
  rs.SetRenderTarget(&window);
  while (atInput::Update(true)) // Process user inputs
  {
    // Update camera
    camera.OnUpdate(0.016);
    camera.SetViewport(&window);

    // Clear window
    window.Clear(clearColor);
    rs.SetViewport(atVec4I(0, 0, window.Size()));
    rs.SetScissor(atVec4I(0, 0, window.Size()));

    // Set Lighting Data
    model.SetLighting(light);
    model.EnableLighting(useLighting);
    model.SetCamera(camera.m_translation);
    useLighting = atInput::ButtonPressed(atKC_L) ? !useLighting : useLighting;

    // Draw model
    model.Draw(camera.ProjectionMat() * camera.ViewMat());

    // Drawing Text - See ExampleRenderText() for more examples
    atPrimitiveRenderer::AddText(10, 10, "Press 'L' to toggle lighting.");
    {
      atRenderState rs; // State changes are reverted when 'rs' falls out of scope
      rs.SetDepthReadEnabled(false);
      rs.SetBlendEnabled(true);
      atPrimitiveRenderer::Draw(window);
    }
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
  atPrimitiveRenderer::SetFont(fontPath);

  // Setup correct render state
  atRenderState rs;
  rs.SetBlendEnabled(true);
  rs.SetDepthReadEnabled(false);
  rs.SetScissorEnabled(true);

  // A Render target must be set before attempting to render
  // this can be an atWindow or atRenderTarget with a color/depth texture attached
  rs.SetRenderTarget(&window);

  while (atInput::Update())
  {
    window.Clear({ 0.3f, 0.3f, 0.3f, 1.0f });
    rs.SetViewport(atVec4I(0, 0, window.Size()));
    rs.SetScissor(atVec4I(0, 0, window.Size()));

    // Enable/Disable pivots
    static bool usePivot = true;
    usePivot = atInput::ButtonPressed(atKC_P) ? !usePivot : usePivot;

    // Bake Text
    if (usePivot)
    {
      atPrimitiveRenderer::AddText(0, 0, "Top Left (Pivot: 0.0, 0.0)");
      atPrimitiveRenderer::AddText(window.Width(), 0, "(Pivot: 1.0, 0.0) Top Right", { 1.f, 0.f });
      atPrimitiveRenderer::AddText(0, window.Height(), "Bottom Left (Pivot: 0.0, 1.0)", { 0.f, 1.f });
      atPrimitiveRenderer::AddText(window.Width(), window.Height(), "(Pivot: 1.0, 1.0) Bottom Right", { 1.f, 1.f });

      atPrimitiveRenderer::PushColour(atVec4F(0.7f, 0.7f, 0.7f, 0.8f));
      atPrimitiveRenderer::AddRectangle(window.Width() / 2, window.Height() / 2, atPrimitiveRenderer::TextSize("(Pivot 0.5,0.5): Center"), { .5f, .5f });
      atPrimitiveRenderer::PopColour();

      atPrimitiveRenderer::AddText(window.Width() / 2, window.Height() / 2, "(Pivot 0.5,0.5): Center", { .5f, .5f });
    }
    else
    {
      atPrimitiveRenderer::AddText(0, 0, "Top Left");
      atPrimitiveRenderer::AddText(window.Width(), 0, "Top Right");
      atPrimitiveRenderer::AddText(0, window.Height(), "Bottom Left");
      atPrimitiveRenderer::AddText(window.Width(), window.Height(), "Bottom Right");

      atPrimitiveRenderer::PushColour(atVec4F(0.7f, 0.7f, 0.7f, 0.8f));
      atPrimitiveRenderer::AddRectangle(window.Width() / 2, window.Height() / 2, atPrimitiveRenderer::TextSize("Center"), { .5f, .5f });
      atPrimitiveRenderer::PopColour();

      atPrimitiveRenderer::AddText(window.Width() / 2, window.Height() / 2, "Center");
    }

    // Render Text
    atPrimitiveRenderer::Draw(window);

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

void ExampleRayTraceMesh()
{
  atMesh mesh;
  mesh.Import("assets/test/models/cube.obj");
  
  atBVH<atTriangle<double>> bvh(mesh.GetTriangles());
  
  double time = 0.0;
  bvh.RayTrace(atRay<double>(atVec3D(0, 0, -10), atVec3D(0, 0, 1)), atMat4D::Identity(), &time);
}

#include "atScene.h"
#include "atSceneMeshRenderable.h"

void ExampleCreateScene()
{
  atWindow window;
  atScene scene;
  
  atRenderState rs;
  rs.SetRenderTarget(&window);

  // Create camera
  atSceneNode *pNode = scene.CreateNode("Camera 1", { 2, 1, 5 });
  atSceneCamera *pCam1 = pNode->AddComponent<atSceneCamera>();
  scene.AddActiveCamera(pNode);


  // Create another camera
  pNode = scene.CreateNode("Camera 2", {0, 1, 5});
  atSceneCamera *pCam2 = pNode->AddComponent<atSceneCamera>();
  scene.AddActiveCamera(pNode);


  // Add a mesh
  pNode = scene.CreateNode("Mesh");
  atSceneMeshRenderable *pMesh = pNode->AddComponent<atSceneMeshRenderable>();
  pMesh->SetModel("assets/test/models/level.obj");

  // Add a skybox
  // pNode = scene.CreateNode("Skybox");
  // pNode->AddComponent<atSceneSkybox>();


  while(atInput::Update())
  {
    window.Clear({ 0.3, 0.3, 0.3, 1.0 });

    pCam1->SetViewport(atVec4I(0, 0, window.Width() / 2, window.Height()));
    pCam2->SetViewport(atVec4I(window.Width() / 2, 0, window.Width() / 2, window.Height()));
    scene.m_viewport = { 0, 0, window.Width(), window.Height() };
    scene.Update();
    scene.Draw();

    window.Swap();
  }
}

#include "atLua.h"

void ExampleRunLua()
{
  atLua lua;
  lua.RunText("print([[hello from lua]])");
}

#include "atImGui.h"

void ExampleImGui()
{
  atWindow window;
  atRenderState rs;
  rs.SetRenderTarget(&window);

  while (atInput::Update())
  {
    window.Clear({ 0, 0.5, 0.5, 1 });
    atImGui::BeginFrame(&window);
    static char input[512] = "initial";
    if (ImGui::Begin("Test Window"))
    {
      ImGui::Text("Hello from dear imgui");
      ImGui::InputText("", input, 512);
    }
    ImGui::End();

    if (ImGui::Begin("Text Window No. 2"))
      ImGui::Text("Hello from dear imgui no. 2");
    ImGui::End();

    atImGui::EndFrame();
    atImGui::Render();
    window.Swap();
  }
}
  
#include "atBVH.h"
#include "atIntersects.h"

#define PACK_X(x) ((x & 0xFFFF) << 31)
#define PACK_Y(y) ((y & 0xFFFF) << 15)
#define PACK_Z(z) (z)
#define PACK_XYZ(x, y, z) (PACK_X(x) | PACK_Y(y) | PACK_Z(z))
#define UNPACK_X(x) ((x >> 31) & 0xFFFF) 
#define UNPACK_Y(y) ((y >> 15) & 0xFFFF)
#define UNPACK_Z(z) (z & 0xFFFF)

class atPointBlob
{
public:
  static const int64_t _regionLayers = 3;
  static const int64_t _leafSize = 256;
  static const int64_t _leafWidth = (1 << _regionLayers);
  static const int64_t _regionSize = _leafWidth * _leafSize;

  struct Voxel
  {
    atVec3I position;
    uint32_t color;
  };

  struct BlockLeaf
  {
    atAABB<int64_t> bounds;
    atHashMap<int64_t, uint32_t> m_points;
  };

  struct Block
  {
    BlockLeaf* GetLeaf(const atVec3I &pos, const bool create = true)
    {
      if (layer == _regionLayers)
      {
        if (!pLeaf && create)
        {
          pLeaf = atNew<BlockLeaf>();
          pLeaf->bounds = atAABB<int64_t>(pos * _leafSize, (pos + 1) * _leafSize);
        }
        return pLeaf;
      }

      atVec3I blockPos = atVec3I(atVec3D(pos) / _regionSize) * (1ll << layer) - position;
      uint8_t i = blockPos.x + blockPos.y * 2 + blockPos.z * 4;
      if (i < 0 || i >= 8) return nullptr;
      if (!m_children[i] && create)
      {
        m_children[i] = atNew<Block>();
        m_children[i]->position = atVec3I(atVec3D(pos) / _regionSize) * (1ll << layer);
        m_children[i]->layer = layer + 1;
      }
      return m_children[i] ? m_children[i]->GetLeaf(pos) : nullptr;
    }
    
    atVector<Voxel> GetPoints(const atAABB<int64_t> &bounds)
    {
      atVector<Voxel> ret;
      atVec3I minLeaf = bounds.m_min / _leafWidth;
      atVec3I maxLeaf = bounds.m_max / _leafWidth;

      for (int64_t z = minLeaf.z; z <= maxLeaf.z + 1; ++z)
        for (int64_t y = minLeaf.y; y <= maxLeaf.y + 1; ++y)
          for (int64_t x = minLeaf.x; x <= maxLeaf.x + 1; ++x)
          {
            BlockLeaf *pLeaf = GetLeaf({ x * _leafWidth, y * _leafWidth, z * _leafWidth }, false);
            if(!pLeaf)
              continue;
            ret.reserve(pLeaf->m_points.Size());
            Voxel voxel;
            if (pLeaf->bounds.Contains(bounds.m_min) || pLeaf->bounds.Contains(bounds.m_max))
            {
              for (auto &kvp : pLeaf->m_points)
              {
                voxel.position = { UNPACK_X(kvp.m_key), UNPACK_Y(kvp.m_key), UNPACK_Z(kvp.m_key) };
                voxel.color = kvp.m_val;
                if (bounds.Contains(voxel.position))
                  ret.push_back(voxel);
              }
            }
            else
            {
              for (auto &kvp : pLeaf->m_points)
              {
                voxel.position = { UNPACK_X(kvp.m_key), UNPACK_Y(kvp.m_key), UNPACK_Z(kvp.m_key) };
                voxel.color = kvp.m_val;
                ret.push_back(voxel);
              }
            }
          }
      return ret;
    }

    atVec3I position = 0;
    int64_t layer = 1;
    Block *m_children[8] = { 0 };
    BlockLeaf *pLeaf = nullptr;
  };

  bool AddPoint(const atVec3I &position, const uint32_t color)
  {
    Block *pBlock = GetRegion(position / _regionSize);
    atVec3I regionPos = position % _regionSize;
    return pBlock->GetLeaf(regionPos)->m_points.TryAdd(PointID(regionPos), color);
  }

  atVector<Voxel> GetPoints(const atAABB<int64_t> &bounds)
  {
    atVector<Voxel> ret;
    atVec3I minRegion = bounds.m_min / _regionSize;
    atVec3I maxRegion = bounds.m_max / _regionSize;
    for (int64_t z = minRegion.z; z <= maxRegion.z + 1; ++z)
      for (int64_t y = minRegion.y; y <= maxRegion.y + 1; ++y)
        for (int64_t x = minRegion.x; x <= maxRegion.x + 1; ++x)
        {
          Block *pRegion = GetRegion({ x, y, z }, false);
          if (!pRegion)
            continue;
          ret.push_back(pRegion->GetPoints(bounds));
        }

    return ret;
  }

  atHashMap<atVec3I, Block*> m_regions;

protected:
  atVec3I GetPosition(const int64_t id) { return{ UNPACK_X(id), UNPACK_Y(id), UNPACK_Z(id) }; }

  int64_t PointID(const atVec3I &position) { return PACK_XYZ(position.x % _regionSize, position.y % _regionSize, position.z % _regionSize); }

  Block* GetRegion(const atVec3I &position, const bool create = true)
  {
    Block **ppBlock = m_regions.TryGet(position);
    if (!ppBlock && create)
    {
      Block *pBlock = atNew<Block>();
      m_regions.Add(position, pBlock);
      return pBlock;
    }
    return ppBlock ? *ppBlock : nullptr;
  }
};

void ExamplePointBlob()
{
  atPointBlob blob;
  for (int64_t y = 0; y < 256; ++y)
    for (int64_t x = 0; x < 256; ++x)
      blob.AddPoint({ x, y, 0ll }, atColor::Pack(x, y, 0ll));

  atWindow window;
  atSimpleCamera cam(&window, { 0, 3, 0 });

  atVector<atPointBlob::Voxel> points = blob.GetPoints(atAABB<int64_t>({ 0,0,0 }, { 256, 256, 256 }));
  atVector<atVec3F> pos(points.size());
  atVector<atVec4F> col(points.size());

  for (atPointBlob::Voxel &p : points)
  {
    pos.push_back(p.position);
    col.push_back(atColor::UnPack<float>(p.color));
  }

  atRenderable ro;
  ro.SetShader("assets/shaders/pointColor");
  ro.SetAttribute("POSITION", pos);
  ro.SetAttribute("COLOUR", col);

  while (atInput::Update())
  {
    cam.OnUpdate(0.016);
    window.Clear({ 0.3, 0.3, 0.3, 1.0 });

    ro.SetUniform("mvp", cam.ProjectionMat() * cam.ViewMat());
    ro.DrawPoints(false);

    window.Swap();
  }
}

int main(int argc, char **argv)
{
  atUnused(argc, argv);
  
  // Uncomment Something!
  
  // Functional
  
  // ExampleRenderText();
  // ExampleRenderMesh();
  // ExampleCreateScene();
  // ExampleSocketUsage();
  // ExampleNetworkStreaming();
  // ExampleImGui();

  // Not Quite Functional
  
  // ExampleImportExportMesh();
  // ExampleRayTraceMesh();
  // ExampleRunLua();
  ExamplePointBlob();
  system("pause");
  return atWindow_GetResult();
}
