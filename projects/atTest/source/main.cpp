
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

#include "atInput.h"
#include "atBVH.h"
#include <time.h>
#include "atShaderPool.h"
#include "atPrint.h"

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
#include "atGraphicsModel.h"
#include "atRenderState.h"
#include "atSceneCamera.h"

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
  atGraphicsModel model(path.c_str());

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
    if (atInput::ButtonPressed(atKC_Apostraphe))
      atShaderPool::ReloadShaders();

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
    model.SetCamera(camera.Translation());
    useLighting = atInput::ButtonPressed(atKC_L) ? !useLighting : useLighting;

    // Draw model
    model.Draw(camera.ProjectionMat() * camera.ViewMat());

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
  mesh.Import("assets/test/models/bumpmap.obj");

  atBVH<atTriangle<double>> bvh(mesh.GetTriangles());
  atWindow window("Window", { 800, 600 }, false);
  atSimpleCamera cam(&window);
  while (atInput::Update())
  {
    window.Clear(0xFF333333);
    cam.OnUpdate(0.016);
    cam.SetViewport(&window);
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

#include "atBPGNetwork.h"
#include "atFile.h"

void ExampeBackPropagation()
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

#include "atPhysicsObject.h"
#include "atCollision.h"
#include "atPhysicsScene.h"

void ExamplePhysics()
{
  // Set up window + draw target
  atWindow window("Physics Example");
  atRenderState rs;
  rs.SetRenderTarget(&window);
  
  // Setup models
  {
    atGraphicsModel cubeModel("assets/test/models/cube1x1_textured.obj");
    atGraphicsModel sphereModel("assets/test/models/sphereR.5_textured.obj");

    atLight l;
    l.m_diffuseColor = atVec4D(1, 1, 1, 1.0);
    l.m_ambientColor = atVec4D(0.4, 0.4, 0.4, 1);
    l.m_specularColor = atVec4D(1, 1, 1, 1);
    l.m_direction = atVec3D(0, 1, 0).Normalize();

    sphereModel.SetLighting(l);
    cubeModel.SetLighting(l);

    // Create camera
    atSimpleCamera camera(&window, { 0, 4, 4 });

    atPhysicsScene scene;
    scene.AddBody(atRigidBody(atVec3D(1, 1, 1)), { 0, 5, -2 });
    scene.AddBody(atRigidBody(1.0), { 0, 5, 2 });
    scene.AddBody(atRigidBody(atVec3D(50, 1, 50), 1, true), { 0, 0, 0 }, { 0, 0, 0 });

    int64_t camSphere = scene.AddBody(atRigidBody(1.0), { 0, 1, 4 });
    scene.GetBody(camSphere).elasticity = 1;

    // https://cse442-17f.github.io/Gilbert-Johnson-Keerthi-Distance-Algorithm/ // LOOK AT THIS

    at2DRenderer::SetFont("D:/Programming Projects/atLib/builds/bin/Assets/Fonts/RomanSerif.ttf");

    while (atInput::Update())
    {
      window.Clear(0xFF440000);
      rs.SetViewport({ 0,0, window.Size() });
      camera.SetViewport(&window);
      camera.OnUpdate(0.016);

      // Gravity
      scene.AddAcceleration(9.8, {0, -1, 0});

      // Camera acceleration
      atVec3D move = 0;
      if (atInput::ButtonDown(atKC_W)) move.z -= 2;
      if (atInput::ButtonDown(atKC_S)) move.z += 2;
      if (atInput::ButtonDown(atKC_D)) move.x += 2;
      if (atInput::ButtonDown(atKC_A)) move.x -= 2;
      if (atInput::ButtonDown(atKC_E)) move.y += 2;
      if (atInput::ButtonDown(atKC_X)) move.y -= 2;
      move = camera.Orientation().Rotate(move);
      move *= (1 + atInput::ButtonDown(atKC_Shift));
      if (atInput::ButtonDown(atKC_Space)) move.y += 9.8 * 2;

      // Apply camera movement
      scene.GetBody(camSphere).AddAcceleration(move);

      // Update physics scene
      scene.Update();

      // Set camera position
      camera.SetTranslation(scene.GetBody(camSphere).Translation());

      // Update models camera position
      sphereModel.SetCamera(camera.Translation());
      cubeModel.SetCamera(camera.Translation());

      // Draw scene
      atMat4D vp = camera.ProjectionMat() * camera.ViewMat();
      for (const atRigidBody &body : scene.Bodies())
      {
        if (&body == &scene.GetBody(camSphere))
          continue;

        switch (body.Type())
        {
        case atRBT_Sphere: sphereModel.Draw(vp, atMatrixTranslation(body.Translation()) * atMatrixYawPitchRoll(body.RotationEuler()) * atMatrixScaleUniform(body.AsSphere().m_radius * 2)); break;
        case atRBT_Cube: cubeModel.Draw(vp, atMatrixTranslation(body.Translation()) * atMatrixYawPitchRoll(body.RotationEuler()) * atMatrixScale(body.AsOBB().Dimensions())); break;
        }
      }

      {
        atRenderState textState;
        textState.SetDepthReadEnabled(false);
        textState.SetDepthWriteEnabled(false);
        textState.SetScissorEnabled(true);
        textState.SetBlendEnabled(true);
        at2DRenderer::Draw(window);
      }

      window.Swap();
    }
  }
}

#include "atBVH.h"
#include "atIntersects.h"
#include "atTest.h"

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
  
  ExamplePhysics();

  // Not Quite Functional
  
  // ExampleImportExportMesh();
  // ExampleRayTraceMesh();
  // ExampeBackPropagation();
  // ExampleRunLua();
  
  system("pause");
  return atWindow::GetResult();
}
