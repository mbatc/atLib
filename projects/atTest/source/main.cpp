
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

#define ATLIB_DIRECTX
#define RUN_ATTEST

#include "atBVH.h"
#include "atMesh.h"
#include "atInput.h"
#include "atLight.h"
#include "atMeshComponent.h"

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

void ExampleStrings()
{
  { // Assigning
    atString firstString = "This is a string";
    atString secondString = "This is another string";
    firstString = firstString + secondString;
    secondString += firstString;

    firstString = 'a';
    firstString = "c-string" + atString("atString");
  }

  { // Type conversions

    // Type conversions must be explicit (with the exception of the atString = operator)

    atString aNumber = atString(3);
    atString anotherNumber = atString(4.0f);
    atString addingAsStrings = aNumber + anotherNumber;

    atString addingToString = atString("I like the number ") + 4;
    double addingAsValues = double(aNumber) + double(anotherNumber);
    atUnused(addingToString, addingAsValues);
    int64_t asValue = int64_t(aNumber);

    // To wide strings
    std::wstring wideString = std::wstring(atString(std::wstring(L"jlkjsalkjasp2©ýNc☼¯")));

    atUnused(wideString);
  }
}

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

#include "at2DRenderer.h"
#include "atRenderState.h"
#include "atCamera.h"

void ExampleRenderMesh(atVec2I wndSize = {800, 600}, bool useLighting = true)
{
  at2DRenderer::SetFont(atFilename("assets/fonts/RomanSerif.ttf"));

  // Set the model being loaded
  const atString path = "assets/test/models/cube1x1.obj";

  // Set the windows clear colour
  const atVec4F clearColor = atVec4F(.3f, .3f, .3f, 1.f);
  
  atLight light;
  light.m_diffuseColor = atVec4F(0.8f, 0.6f, 0.5f, 1.0f);
  light.m_ambientColor = atVec4F(0.3f, 0.3f, 0.3f, 1.0f);
  light.m_specularColor = atVec4F(0.8f, 0.6f, 0.5f, 1.0f);
  
  // Import the model
  // atGraphicsModel model(path.c_str());

  // Create a window
  atWindow window("Default Window", wndSize);

  // Create a camera
  atFPSCamera camera(&window, { 0, 1, 5 });
  camera.m_moveSpeed = 1.0f;

  // Main program loop
  atRenderState rs;

  // A Render target must be set before attempting to render
  // this can be an atWindow or atRenderTarget with a color/depth texture attached
  // rs.SetRenderTarget(&window);
  while (atInput::Update(true)) // Process user inputs
  {
    // if (atInput::ButtonPressed(atKC_Apostraphe))
    //   atShaderPool::ReloadShaders();

    // Update camera
    camera.Update(0.016);
    camera.SetViewport(&window);

    // Clear window
    window.Clear(clearColor);
    rs.SetViewport(atVec4I(0, 0, window.Size()));
    rs.SetScissor(atVec4I(0, 0, window.Size()));

    // Set Lighting Data
    // model.SetLighting(light);
    // model.EnableLighting(useLighting);
    // model.SetCamera(camera.Translation());
    useLighting = atInput::ButtonPressed(atKC_L) ? !useLighting : useLighting;

    // Draw model
    // model.Draw(camera.ProjectionMat() * camera.ViewMat());

    // Drawing Text - See ExampleRenderText() for more examples
    at2DRenderer::AddText(10, 10, "Press 'L' to toggle lighting.");
    {
      atRenderState rs; // State changes are reverted when 'rs' falls out of scope
      rs.SetDepthReadEnabled(false);
      rs.SetBlendEnabled(true);
      at2DRenderer::Draw(window);
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
  at2DRenderer::SetFont(fontPath);

  // Setup correct render state
  atRenderState rs;
  rs.SetBlendEnabled(true);
  rs.SetDepthReadEnabled(false);
  rs.SetScissorEnabled(true);

  // A Render target must be set before attempting to render
  // this can be an atWindow or atRenderTarget with a color/depth texture attached
  // rs.SetRenderTarget(&window);

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
      at2DRenderer::AddText(0, 0, "Top Left (Pivot: 0.0, 0.0)");
      at2DRenderer::AddText(window.Width(), 0, "(Pivot: 1.0, 0.0) Top Right", { 1.f, 0.f });
      at2DRenderer::AddText(0, window.Height(), "Bottom Left (Pivot: 0.0, 1.0)", { 0.f, 1.f });
      at2DRenderer::AddText(window.Width(), window.Height(), "(Pivot: 1.0, 1.0) Bottom Right", { 1.f, 1.f });

      at2DRenderer::PushColour(atVec4F(0.7f, 0.7f, 0.7f, 0.8f));
      at2DRenderer::AddRectangle(window.Width() / 2, window.Height() / 2, at2DRenderer::TextSize("(Pivot 0.5,0.5): Center"), { .5f, .5f });
      at2DRenderer::PopColour();

      at2DRenderer::AddText(window.Width() / 2, window.Height() / 2, "(Pivot 0.5,0.5): Center", { .5f, .5f });
    }
    else
    {
      at2DRenderer::AddText(0, 0, "Top Left");
      at2DRenderer::AddText(window.Width(), 0, "Top Right");
      at2DRenderer::AddText(0, window.Height(), "Bottom Left");
      at2DRenderer::AddText(window.Width(), window.Height(), "Bottom Right");

      at2DRenderer::PushColour(atVec4F(0.7f, 0.7f, 0.7f, 0.8f));
      at2DRenderer::AddRectangle(window.Width() / 2, window.Height() / 2, at2DRenderer::TextSize("Center"), { .5f, .5f });
      at2DRenderer::PopColour();

      at2DRenderer::AddText(window.Width() / 2, window.Height() / 2, "Center");
    }

    // Render Text
    at2DRenderer::Draw(window);

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

  // Write the mesh to an OBJ file
  mesh.Export(outPath1);

  // Write to a AT's binary mesh format
  mesh.Export(outPath2);

  // Import AT's binary mesh format and export to .obj
  mesh.Import(outPath2);
  mesh.Export(outPath3);
}

void ExampleRayTraceMesh()
{
  atMesh mesh;
  mesh.Import("assets/test/models/level.obj");
  
  atBVH<atTriangle<double>> bvh(mesh.GetTriangles());
  atWindow window("Window", { 800, 600 }, false);
  atFPSCamera cam(&window);
  while (atInput::Update())
  {
    window.Clear(0xFF333333);
    cam.Update(0.016);

    atMat4F vp = cam.ProjectionMat() * cam.ViewMat();
    vp = vp.Transpose();
    atMat4F invVP = vp.Inverse();

    static int64_t res = 10;
    res += atInput::ButtonPressed(atKC_P);
    res -= atInput::ButtonPressed(atKC_O);
    res = atMax(res, 1);

    for (int64_t y = 0; y < window.Height(); y += res)
      for (int64_t x = 0; x < window.Width(); x += res)
      {
        atVec2F ssc = { (float)x / (float)window.Width(), (float)y / (float)window.Height() };
        ssc = ssc * 2 - 1;

        atVec4F n(ssc, atClipNearZ<float>(), 1.f);
        n = invVP * n;
        n /= n.w;

        atVec4F f(ssc, atClipFarZ<float>(), 1.f);
        f = invVP * f;
        f /= f.w;
        double time = 0.0;
        if (bvh.RayTrace(atRay<double>(n.xyz(), (f - n).xyz().Normalize()), atMat4D::Identity(), &time))
        {
          time *= time;
          for (int64_t y2 = y; y2 < y + res && y2 < window.Height(); ++y2)
            for (int64_t x2 = x; x2 < x + res && x2 < window.Width(); ++x2)
              window.Pixels()[x2 + y2 * window.Width()] = atColor::Pack(int(255.f - time), int(255.f - time), int(255.f - time), 255);
        }
      }

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
  // rs.SetRenderTarget(&window);

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

#include "atBPGNetwork.h"
#include "atFile.h"

void ExampleBackPropagation()
{
  atBPGNetwork network(4, 4, 2, 16);
  
  for (int64_t layer = 0; layer < network.LayerCount(); ++layer)
  {
    // Randomize weights
    atMatrixNxM<double> mat = network.GetLayerWeights(layer);
    for (double &val : mat.m_data)
      val = (float)(rand() % 100) / 100;
    network.SetLayerWeights(layer, mat);

    // Randomize biases
    // mat = network.GetLayerBiases(layer);
    // for (double &val : mat.m_data)
    //   val = (float)(rand() % 100) / 100;
    // network.SetLayerBiases(layer, mat);
  }

  {
    atFile myFile("atLib.net", atFM_Write);
    myFile.Write(network);
  }

  atBPGNetwork otherNetwork(1,1,1);
  {
    atFile myFile("atLib.net", atFM_Read);
    if (myFile.IsOpen())
      myFile.Read(&otherNetwork);
  }

  printf("Original Network results: \n");
  for (const double val : network.Run({ 0.0, 1.0, 2.0, 3.0 }))
    printf("%lf, ", val);

  printf("\n\nSaved/Loaded Network results: \n");
  for (const double val : otherNetwork.Run({ 0.0, 1.0, 2.0, 3.0 }))
    printf("%lf, ", val);

  for(int64_t i = 0; i < 10000; ++i)
    network.Train({ 10, 1, 3, 5 }, { 1, 0.75, 0.5, 0.25 });

  printf("\n\nTrained Network results: \n");
  for (const double val : network.Run({ 0.0, 1.0, 2.0, 3.0 }))
    printf("%lf, ", val);

  printf("\n");
  getchar();
}


#include "atPython.h"

// A simple example of creating a python and making it available to the Python interpreter

namespace atPythonExample
{
  PyObject* atPythonHelloWorld(PyObject* pCaller, PyObject *pArgs)
  {
    printf("Hello From Python\n");
    Py_IncRef(Py_None);
    Py_RETURN_NONE;
  }

  static PyMethodDef methods[] =
  {
    { "HelloWorld",  atPythonHelloWorld, METH_VARARGS, "Execute a shell command." },
    { NULL, NULL, 0, NULL }
  };

  static PyModuleDef modDef =
  {
    PyModuleDef_HEAD_INIT,
    "atLibTest",   /* name of module */
    "atLib test module", /* module documentation, may be NULL */
    -1,       /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    methods
  };

  PyMODINIT_FUNC PyInit_atLib()
  {
    return PyModule_Create(&modDef);
  }
}

void ExampleEmbeddedPython()
{
  atPython::AddModule("atLib", atPythonExample::PyInit_atLib);
  atPython::Init();
  atPython::Run("import atLib");
  atPython::Run("atLib.HelloWorld()");
  atPython::Shutdown();
}

#include "atBVH.h"
#include "atIntersects.h"
#include "atHashSet.h"

#include "atTest.h"

#include "atGraphics.h"
#include "atDXPrgm.h"
#include "atDXShader.h"
#include "atDXBuffer.h"
#include "atDXSampler.h"

#include "atGLTexture.h"
#include "atGLPrgm.h"
#include "atGLShader.h"
#include "atGLBuffer.h"

#include "atRenderable.h"

class Model
{
public:
  struct SubMesh
  {
    atVector<atVec3F> pos;
    atVector<atVec2F> tex;
    atVector<atVec3F> nrm;
    atVector<atVec4F> col;
  };

  Model(const atMesh &mesh, const atGraphicsAPI &api = atGfxApi_DirectX)
  {
    atVector<SubMesh> submesh;
    submesh.resize(mesh.m_materials.size());

    for (const atMesh::Triangle &tri : mesh.m_triangles)
    {
      int64_t matID = tri.mat;
      SubMesh &m = submesh[matID];
      for (const atMesh::Vertex &v : tri.verts)
      {
        m.col.push_back(mesh.m_colors[v.color]);
        m.pos.push_back(mesh.m_positions[v.position]);
        m.nrm.push_back(mesh.m_normals[v.normal]);
        m.tex.push_back(mesh.m_texCoords[v.texCoord]);
      }
    }

    switch (api)
    {
    case atGfxApi_DirectX:
    {
      std::shared_ptr<atGFXPrgmInterface> prgm = std::make_shared<atDXPrgm>();
      prgm->SetStage(std::make_shared<atDXShader>(atFile::ReadText("Assets/Shaders/color.vs"), atPS_Vertex));
      prgm->SetStage(std::make_shared<atDXShader>(atFile::ReadText("Assets/Shaders/color.ps"), atPS_Fragment));

      for (const atMaterial &mat : mesh.m_materials)
        if (mat.HasTexture(atMP_Diffuse) && !textures.Contains(mat.GetTexture(atMP_Diffuse).to_lower()))
          textures.Add(mat.GetTexture(atMP_Diffuse).to_lower(), std::make_shared<atDXTexture>(atImage(atFilename(mat.GetTexture(atMP_Diffuse)))));
      meshes.resize(submesh.size());

      for (int64_t i = 0; i < meshes.size(); ++i)
      {
        atRenderable &ro = meshes[i];
        ro.SetAttribute("COLOR", std::make_shared<atDXBuffer>(submesh[i].col));
        ro.SetAttribute("POSITION", std::make_shared<atDXBuffer>(submesh[i].pos));
        ro.SetAttribute("TEXCOORD", std::make_shared<atDXBuffer>(submesh[i].tex));
        ro.SetAttribute("NORMAL", std::make_shared<atDXBuffer>(submesh[i].nrm));

        atString texPath = mesh.m_materials[i].GetTexture(atMP_Diffuse).to_lower();
        std::shared_ptr<atGFXTexInterface> tex;
        if (texPath.length())
          tex = textures[texPath];
        else
        {
          atVector<atCol> colours;
          colours.resize(100, 0xFFFFFFFF);
          tex = std::make_shared<atDXTexture>(atImage(colours.data(), { 10, 10 }));
        }

        ro.SetTexture("texture0", tex);
        ro.SetSampler("sampler0", std::make_shared<atDXSampler>());
        ro.SetProgram(prgm);
      }
    } break;

    case atGfxApi_OpenGL:
    {
      std::shared_ptr<atGFXPrgmInterface> prgm = std::make_shared<atGLPrgm>();
      prgm->SetStage(std::make_shared<atGLShader>(atFile::ReadText("Assets/Shaders/color.vert"), atPS_Vertex));
      prgm->SetStage(std::make_shared<atGLShader>(atFile::ReadText("Assets/Shaders/color.frag"), atPS_Fragment));

      for (const atMaterial &mat : mesh.m_materials)
        if (mat.HasTexture(atMP_Diffuse) && !textures.Contains(mat.GetTexture(atMP_Diffuse).to_lower()))
          textures.Add(mat.GetTexture(atMP_Diffuse).to_lower(), std::make_shared<atGLTexture>(atImage(atFilename(mat.GetTexture(atMP_Diffuse)))));
      meshes.resize(submesh.size());

      for (int64_t i = 0; i < meshes.size(); ++i)
      {
        atRenderable &ro = meshes[i];
        ro.SetAttribute("color0", std::make_shared<atGLBuffer>(submesh[i].col));
        ro.SetAttribute("position0", std::make_shared<atGLBuffer>(submesh[i].pos));
        ro.SetAttribute("texcoord0", std::make_shared<atGLBuffer>(submesh[i].tex));
        ro.SetAttribute("normal0", std::make_shared<atGLBuffer>(submesh[i].nrm));

        atString texPath = mesh.m_materials[i].GetTexture(atMP_Diffuse).to_lower();
        std::shared_ptr<atGFXTexInterface> tex;
        if (texPath.length())
          tex = textures[texPath];
        else
          tex = std::make_shared<atGLTexture>(atImage());

        ro.SetTexture("texture0", tex);
        ro.SetProgram(prgm);
      }
    } break;
    }
  }

  atVector<atRenderable> meshes;
  atHashMap<atString, std::shared_ptr<atGFXTexInterface>> textures;
};

#include <time.h>
#include "atScene.h"

atGraphics *pGraphics = nullptr;

atScene _SceneExampleLoadMesh(const atString &path)
{
  atMesh mesh;
  if (!mesh.Import(path))
    return atScene();

  mesh.MakeValid();
  mesh.DiscoverTextures();
  mesh.FlipTextures(false, true);

  struct SubMesh
  {
    atVector<atVec3F> pos;
    atVector<atVec2F> tex;
    atVector<atVec3F> nrm;
    atVector<atVec3F> tngt;
    atVector<atVec3F> btngt;
    atVector<atVec4F> col;
  };

  atScene scene;

  atVector<SubMesh> submesh;
  submesh.resize(mesh.m_materials.size());

  for (const atMesh::Triangle &tri : mesh.m_triangles)
  {
    int64_t matID = tri.mat;
    SubMesh &m = submesh[matID];
    for (const atMesh::Vertex &v : tri.verts)
    {
      m.col.push_back(mesh.m_colors[v.color]);
      m.pos.push_back(mesh.m_positions[v.position]);
      m.nrm.push_back(mesh.m_normals[v.normal]);
      m.tex.push_back(mesh.m_texCoords[v.texCoord]);
      m.nrm.push_back(mesh.m_tangents[v.tangent]);
      m.btngt.push_back(mesh.m_bitangent[v.bitanget]);
    }
  }

  for (int64_t mat = 0; mat < mesh.m_materials.size(); ++mat)
  {
    atMeshComponent meshComp;
    meshComp.m_renderable.SetAttribute("COLOR", std::make_shared<atDXBuffer>(submesh[mat].col));
    meshComp.m_renderable.SetAttribute("POSITION", std::make_shared<atDXBuffer>(submesh[mat].pos));
    meshComp.m_renderable.SetAttribute("TEXCOORD", std::make_shared<atDXBuffer>(submesh[mat].tex));
    meshComp.m_renderable.SetAttribute("NORMAL", std::make_shared<atDXBuffer>(submesh[mat].nrm));
    meshComp.m_material = mesh.m_materials[mat];
    meshComp.m_renderable.SetMaterial(meshComp.m_material);

    int64_t nodeID = scene.Add(atSceneNode());

    scene.Get(nodeID)->AddComponent<atMeshComponent>(std::move(meshComp));
  }

  return scene;
}

void ExampleSimpleScene()
{
  atWindow window;
 
  {
    atGraphics graphics(&window);
    atScene scene;
    int64_t meshID;
    {
      atScene mesh = _SceneExampleLoadMesh("Assets/Test/models/sponza/sponza.obj");
      meshID = scene.Insert(&mesh, atHierarchy_atRootNodeID, true);
    }

    atRenderState rs;
    int64_t t = (int64_t)clock();
    int64_t camID = scene.Add(atSceneNode());
    scene.Get(camID)->AddComponent<atCameraComponent>();
    scene.Get(camID)->AddComponent<atControlComponent>();
    scene.Get(camID)->GetTransform()->SetTranslation(atVec3D{ 0, 0, -1 });
    scene.Get(camID)->GetTransform()->SetRotation(atVec3D{ 0, 0, 0 });

    while (atInput::Update())
    {
      if (atInput::ButtonPressed(atKC_F11))
        window.SetWindowed(!window.IsWindowed());

      // Process dropped files
      for (const atString &f : window.DroppedFiles())
      {
        scene.Remove(meshID);
        atScene mesh = _SceneExampleLoadMesh(f);
        meshID = scene.Insert(&mesh, atHierarchy_atRootNodeID, true);
      }

      // Draw
      rs.SetViewport(atVec4I(0, 0, window.Size()));
      scene.Get(camID)->GetComponent<atCameraComponent>()->SetAspect(window.Size());

      window.Clear(atVec4F{ 0.3f, 0.3f, 1.0f, 1 });

      scene.Update();
      scene.Render(scene.Get(camID)->GetComponent<atCameraComponent>()->ViewProjMat());

      window.Swap();
    }

    scene.Destroy();
  }
}

int main(int argc, char **argv)
{
  atUnused(argc, argv);

#ifdef RUN_ATTEST
  atTest();
#endif
  
  // Uncomment Something!

  // Functional
  
  // ExampleRenderText();
  // ExampleRenderMesh();
  // ExampleCreateScene();
  // ExampleSocketUsage();
  // ExampleNetworkStreaming();
  // ExampleImGui();
  // ExampleEmbeddedPython();
  // ExampleSimpleScene();

  // Not Quite Functional
  
  // ExampleImportExportMesh();
  // ExampleRayTraceMesh();
  // ExampleBackPropagation();
  // ExampleRunLua();
  
  system("pause");
  return atWindow::GetResult();
}
