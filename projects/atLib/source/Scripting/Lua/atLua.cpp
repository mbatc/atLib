
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

#include "atLua.h"
#include "sol.hpp"
#include "atImGui.h"
#include "atInput.h"
#include "atLuaScene.h"
#include "atSceneScript.h"
#include "atSceneCamera.h"
#include "atSceneMeshRenderable.h"

inline void _LuaPanic(sol::optional<std::string> maybe_msg) 
{
  printf("Lua is in a panic state and will now abort() the application\n");
  if (maybe_msg)
    printf("\terror message: %s\n", maybe_msg.value().c_str());
}

int _ExceptionHandler(lua_State* L, sol::optional<const std::exception&> exception, sol::string_view description)
{
  printf("An exception occurred in a function, here's what it says:\n\t");
  if (exception)
  {
    const std::exception& ex = *exception;
    atString str = ex.what();
    printf("exception: %s\n\n", str.c_str());
  }
  else
  {
    printf("description: %s\n\n", description.data());
  }
  return sol::stack::push(L, description);
}

bool _LuaCall(const sol::state &s, const sol::protected_function_result &res)
{
  if (!res.valid())
  {
    atString err = "sol: ";
    err += to_string(res.status()).c_str();
    err += " error";
    std::exception_ptr eptr = std::current_exception();
    if (eptr) {
      err += " with a ";
      try {
        std::rethrow_exception(eptr);
      }
      catch (const std::exception& ex) {
        err += "std::exception -- ";
        err += ex.what();
      }
      catch (const std::string& message) {
        err += "thrown message -- ";
        err += message.c_str();
      }
      catch (const char* message) {
        err += "thrown message -- ";
        err += message;
      }
      catch (...) {
        err.append("thrown but unknown type, cannot serialize into error message");
      }
    }

    if (type_of(s, res.stack_index()) == sol::type::string) {
      err += ": ";
      sol::string_view serr = sol::stack::get<sol::string_view>(s, res.stack_index());
      err += serr.data();
    }

    printf("-- LUA ERROR --\n\n%s\n\n", err.c_str());
    return false;
  }
  return true;
}

atLua::atLua()
{
  m_pLua = atNew<sol::state>();
  lua_atpanic(*m_pLua, sol::c_call<decltype(&_LuaPanic), &_LuaPanic>);
  m_pLua->set_exception_handler(_ExceptionHandler);
  m_pLua->open_libraries();
  ExposeContainers();
}

atLua::~atLua() { atDelete(m_pLua); }
sol::state * atLua::GetLua() { return m_pLua; }
bool atLua::RunText(const atString &command) { return _LuaCall(*m_pLua, m_pLua->script(command.c_str(), &sol::script_pass_on_error)); }
bool atLua::RunFile(const atString &filename) { return _LuaCall(*m_pLua, m_pLua->script_file(filename.c_str(), &sol::script_pass_on_error)); }

// DEAR IMGUI

void atLua::ExposeImGui()
{
  sol::state &lua = *m_pLua;

  auto &gui = lua["atImGui"].get_or_create<sol::table>();

  // Windows
  gui["Begin"] = sol::overload(
    (bool(*)(const char*, const atVec2D&, const atVec2D&))atImGui::Begin,
    (bool(*)(const char*, const atVec2D&))atImGui::Begin,
    (bool(*)(const char*))atImGui::Begin);
  gui["End"] = atImGui::End;
  gui["BeginChild"] = sol::overload(
    (bool(*)(const char*, const atVec2D&, const bool))atImGui::BeginChild,
    (bool(*)(const char*, const atVec2D&))atImGui::BeginChild,
    (bool(*)(const char*))atImGui::BeginChild);
  gui["EndChild"] = atImGui::EndChild;
  gui["OpenPopup"] = atImGui::OpenPopup;
  gui["BeginPopup"] = atImGui::BeginPopup;
  gui["EndPopup"] = atImGui::EndPopup;

  // Controls

  gui["Button"] = sol::overload(
    (bool(*)(const char*, const atVec2D&, const atVec2D&))atImGui::Button,
    (bool(*)(const char*, const atVec2D&))atImGui::Button,
    (bool(*)(const char*))atImGui::Button);
  gui["Selectable"] = sol::overload(
    (bool(*)(const char*, const bool, const atVec2D&))atImGui::Selectable,
    (bool(*)(const char*, const bool))atImGui::Selectable);
  gui["Text"] = atImGui::Text;
  gui["BeginTreeNode"] = atImGui::BeginTreeNode;
  gui["EndTreeNode"] = atImGui::EndTreeNode;
  gui["Checkbox"] = atImGui::Checkbox;
  
  // Input
  gui["TextInput"] = atImGui::TextInput;
  gui["Input"] = sol::overload(atImGui::InputInt, atImGui::InputFloat);
  gui["Input2"] = sol::overload(atImGui::InputInt2, atImGui::InputFloat2);
  gui["Input3"] = sol::overload(atImGui::InputInt3, atImGui::InputFloat3);
  gui["Input4"] = sol::overload(atImGui::InputInt4, atImGui::InputFloat4);

  // Helpers
  gui["IsKeyDown"] = atImGui::IsKeyDown;
  gui["IsKeyPressed"] = atImGui::IsKeyPressed;
  gui["IsKeyReleased"] = atImGui::IsKeyReleased;
  gui["IsItemHovered"] = atImGui::IsItemHovered;
  gui["IsItemActive"] = atImGui::IsItemActive;
  gui["IsItemClicked"] = atImGui::IsItemClicked;
  gui["IsItemFocused"] = atImGui::IsItemFocused;
  gui["IsWindowFocused"] = atImGui::IsWindowFocused;
  gui["IsWindowHovered"] = atImGui::IsWindowHovered;
  gui["GetWindowSize"] = atImGui::GetWindowSize;
  gui["GetWindowHeight"] = atImGui::GetWindowHeight;
  gui["GetWindowWidth"] = atImGui::GetWindowWidth;
  gui["PushID"] = sol::overload((void(*)(const int64_t))atImGui::PushID, (void(*)(const char*))atImGui::PushID);
  gui["PopID"] = atImGui::PopID;
  gui["SameLine"] = atImGui::SameLine;
  gui["NewLine"] = atImGui::NewLine;

  // Style Management

  gui["AddStyle"] = atImGui::AddStyle;
  gui["AddColours"] = atImGui::AddColours;
  gui["RemoveStyle"] = atImGui::RemoveStyle;
  gui["RemoveColours"] = atImGui::RemoveColours;
  gui["PushColour"] = sol::overload(
    (void(*)(const char *, const atVec4D&))atImGui::PushColour,
    (void(*)(const char *))atImGui::PushColour);
  gui["PopColour"] = sol::overload(
    (void(*)(int64_t))atImGui::PopColour,
    (void(*)())atImGui::PopColour);
  gui["PushStyle"] = sol::overload(
    (void(*)(const char *, const atVec2D&))atImGui::PushStyle,
    (void(*)(const char *, const double))atImGui::PushStyle,
    (void(*)(const char *))atImGui::PushStyle);
  gui["PopStyle"] = sol::overload(
    (void(*)(int64_t))atImGui::PopStyle,
    (void(*)())atImGui::PopStyle);
}

// SCENE

void atLua::ExposeScene()
{
  sol::state &lua = *m_pLua;
  
  auto &sceneNamespace = lua["atScene"].get_or_create<sol::table>();

  // LUA Scene Class
  auto &scene = sceneNamespace.create_simple_usertype<atLuaScene>();
  scene.set("AddActiveCamera", sol::overload(
    &atLuaScene::AddActiveCamera,
    &atLuaScene::AddActiveCameraFromID));
  scene.set("RemoveActiveCamera", sol::overload(
    &atLuaScene::RemoveActiveCamera,
    &atLuaScene::RemoveActiveCameraFromID));
  scene.set("IsActiveCamera", sol::overload(
    &atLuaScene::IsActiveCamera,
    &atLuaScene::IsActiveCameraFromID));
  scene.set("CreateNode", sol::overload(
      &atLuaScene::CreateNode,
      &atLuaScene::CreateNodeP,
      &atLuaScene::CreateNodePR,
      &atLuaScene::CreateNodePRS,
      &atLuaScene::CreateNodePRSP));
  scene.set("DeleteNode", sol::overload(
    &atLuaScene::DeleteNode,
    &atLuaScene::DeleteNodeFromID));
  scene.set("GetNode", &atLuaScene::GetNode);
  scene.set("GetNodeIDs", &atLuaScene::GetNodeIDs);
  scene.set("GetRootNode", &atLuaScene::GetRootNode);
  scene.set("GetRootNodeID", &atLuaScene::GetRootNodeID);
  scene.set("GetName", &atLuaScene::GetName);
  scene.set("SetName", &atLuaScene::SetName);
  scene.set("SetViewport", sol::overload(&atLuaScene::SetViewport, &atLuaScene::SetViewportA));

  // LUA Scene Node Class
  auto &node = sceneNamespace.create_simple_usertype<atLuaSceneNode>();
  node.set("GetName", &atLuaSceneNode::GetName);
  node.set("SetName", &atLuaSceneNode::SetName);
  node.set("GetParentID", &atLuaSceneNode::GetParentID);
  node.set("SiblingCount", &atLuaSceneNode::SiblingCount);
  node.set("GetSiblingID", &atLuaSceneNode::GetSiblingID);
  node.set("GetSiblings", &atLuaSceneNode::GetSiblings);
  node.set("GetSibling", &atLuaSceneNode::GetSibling);
  node.set("ChildCount", &atLuaSceneNode::ChildCount);
  node.set("GetChild", &atLuaSceneNode::GetChild);
  node.set("GetChildID", &atLuaSceneNode::GetChildID);
  node.set("GetChildren", &atLuaSceneNode::GetChildren);
  node.set("AddChild", sol::overload(
      &atLuaSceneNode::AddChild,
      &atLuaSceneNode::AddChildP,
      &atLuaSceneNode::AddChildByID,
      &atLuaSceneNode::AddChildByIDP));
  node.set("RemoveChild", sol::overload(
    &atLuaSceneNode::RemoveChild,
    &atLuaSceneNode::RemoveChildP,
    &atLuaSceneNode::RemoveChildByID,
    &atLuaSceneNode::RemoveChildByIDP));
  node.set("GetComponents", sol::overload(
    &atLuaSceneNode::GetComponents,
    &atLuaSceneNode::GetComponentsOfType));
  node.set("GetComponent", sol::overload(
    &atLuaSceneNode::GetComponent,
    &atLuaSceneNode::GetComponentOfType));
  node.set("ComponentCount", sol::overload(&atLuaSceneNode::ComponentCount, &atLuaSceneNode::ComponentCountOfType));
  node.set("GetPosition", &atLuaSceneNode::GetPosition);
  node.set("GetRotation", &atLuaSceneNode::GetRotation);
  node.set("GetScale", &atLuaSceneNode::GetScale);
  node.set("GlobalPosition", &atLuaSceneNode::GetGlobalPosition);
  node.set("GlobalRotation", &atLuaSceneNode::GetGlobalRotation);
  node.set("GlobalScale", &atLuaSceneNode::GetGlobalScale);
  node.set("SetPosition", &atLuaSceneNode::SetPosition);
  node.set("SetRotation", &atLuaSceneNode::SetRotation);
  node.set("SetScale", &atLuaSceneNode::SetScale);
  node.set("GetID", &atLuaSceneNode::GetID);
  node.set("AddComponent", &atLuaSceneNode::AddComponent);
  node.set("RemoveComponent", sol::overload(&atLuaSceneNode::RemoveComponentIndexed, &atLuaSceneNode::RemoveComponent));

  // LUA Component Class
  auto &component = sceneNamespace.create_simple_usertype<atLuaSceneComponent>();
  component.set("TypeID", &atLuaSceneComponent::TypeID);
  component.set("Valid", &atLuaSceneComponent::IsValid);
  component.set("Node", &atLuaSceneComponent::Node);

  // Built in component types
  auto &script = sceneNamespace.create_simple_usertype<atLuaSceneScript>(sol::constructors<atLuaSceneScript(const atLuaSceneComponent&)>());
  script.set("TypeID", &atLuaSceneScript::TypeID);
  script.set("Reload", &atLuaSceneScript::Reload);
  script.set("Unload", &atLuaSceneScript::Unload);
  script.set("GetScriptID", &atLuaSceneScript::ScriptID);
  script.set("GetScriptIDString", &atLuaSceneScript::ScriptIDString);
  script.set("SetPath", &atLuaSceneScript::SetScriptPath);
  script.set("SetSource", &atLuaSceneScript::SetScriptSrc);
  script.set("GetPath", &atLuaSceneScript::GetScriptPath);
  script.set("GetSource", &atLuaSceneScript::GetScriptSrc);
  script.set("Valid", &atLuaSceneScript::IsValid);
  script.set("Node", &atLuaSceneScript::Node);

  auto &camera = sceneNamespace.create_simple_usertype<atLuaSceneCamera>(sol::constructors<atLuaSceneCamera(const atLuaSceneComponent&)>());
  camera.set("GetAspect", &atLuaSceneCamera::GetAspect);
  camera.set("SetAspect", &atLuaSceneCamera::GetAspect);
  camera.set("GetDepthRange", &atLuaSceneCamera::GetDepthRange);
  camera.set("SetDepthRange", sol::overload(&atLuaSceneCamera::SetDepthRange, &atLuaSceneCamera::SetDepthRangeA));
  camera.set("GetViewport", &atLuaSceneCamera::GetViewport);
  camera.set("SetViewport", sol::overload(&atLuaSceneCamera::SetViewport, &atLuaSceneCamera::SetViewportA));
  camera.set("GetFOV", &atLuaSceneCamera::GetFOV);
  camera.set("SetFOV", &atLuaSceneCamera::SetFOV);
  camera.set("GetNearPlane", &atLuaSceneCamera::GetNearPlane);
  camera.set("SetNearPlane", &atLuaSceneCamera::SetNearPlane);
  camera.set("GetFarPlane", &atLuaSceneCamera::GetFarPlane);
  camera.set("SetFarPlane", &atLuaSceneCamera::SetFarPlane);
  camera.set("Valid", &atLuaSceneCamera::IsValid);
  camera.set("Node", &atLuaSceneCamera::Node);

  auto &mesh = sceneNamespace.create_simple_usertype<atLuaSceneMeshRenderable>(sol::constructors<atLuaSceneMeshRenderable(const atLuaSceneComponent&)>());
  mesh.set("SetModelPath", &atLuaSceneMeshRenderable::SetModelPath);
  mesh.set("GetModelPath", &atLuaSceneMeshRenderable::GetModelPath);
  mesh.set("Valid", &atLuaSceneMeshRenderable::IsValid);
  mesh.set("Node", &atLuaSceneMeshRenderable::Node);

  sceneNamespace.set_usertype("Node", node);
  sceneNamespace.set_usertype("Scene", scene);
  sceneNamespace.set_usertype("Component", component);
  sceneNamespace.set_usertype("Script", script);
  sceneNamespace.set_usertype("Camera", camera);
  sceneNamespace.set_usertype("Mesh", mesh);
  sceneNamespace["Script"]["TypeID"] = atSceneScript::typeID;
  sceneNamespace["Camera"]["TypeID"] = atSceneCamera::typeID;
  sceneNamespace["Mesh"]["TypeID"] = atSceneMeshRenderable::typeID;
  // Create scripting helpers
  RunText("\
atScene.Scripts = {}\n\
atScene.CallScript = function(scriptID, funcName, ...)\n\
  if (atScene.Scripts[scriptID] ~= nil) then\n\
    if (atScene.Scripts[scriptID][funcName] ~= nil) then\n\
      if (arg ~= nil) then\n\
        return atScene.Scripts[scriptID][funcName](unpack(arg))\n\
      else\n\
        return atScene.Scripts[scriptID][funcName]()\n\
      end\n\
    end\n\
  end\n\
end\n\
");
}

// CONTAINERS

void atLua::ExposeContainers()
{
  sol::state &lua = *m_pLua;
  auto &containers = lua["atTypes"].get_or_create<sol::table>();
}

// MATH

void atLua::ExposeMathFunctions()
{
  sol::state &lua = *m_pLua;
  auto &math = lua["atMath"].get_or_create<sol::table>();

  // math["MatrixProjection"] = atMatrixProjection<double>;
  // math["MatrixTranslation"] = atMatrixTranslation<double>;
  // math["MatrixYawPitchRoll"] = atMatrixTranslation<double>;
  // math["MatrixRotationX"] = atMatrixRotationX<double>;
  // math["MatrixRotationY"] = atMatrixRotationY<double>;
  // math["MatrixRotationZ"] = atMatrixRotationZ<double>;
  // math["MatrixScaleUniform"] = atMatrixScaleUniform<double>;
  // math["MatrixScale"] = (atMat4D(*)(const atVec3D&))atMatrixScale<double>;
  
  math["Square"] = sol::overload(atSquare<double>, atSquare<int64_t>);
  math["ATan2"] = sol::overload(
    (double(*)(const atVec2D&))atATan2, 
    (double(*)(const double&, const double&))atATan2);

  math["Sin"] = atSin<double>;
  math["Cos"] = atCos<double>;
  math["Tan"] = atTan<double>;
  math["ASin"] = atASin<double>;
  math["ACos"] = atACos<double>;
  math["ATan"] = atATan<double>;
  math["QuadraticSolve"] = atQuadraticSolve<double>;
}

void atLua::ExposeInput()
{
  auto &input = (*m_pLua)["atInput"].get_or_create<sol::table>();
  input["ButtonPressed"] = atInput::ButtonPressed;
  input["ButtonReleased"] = atInput::ButtonReleased;
  input["ButtonDown"] = atInput::ButtonDown;
  input["ButtonDownTime"] = atInput::ButtonDownTime;
  input["ButtonUpTime"] = atInput::ButtonUpTime;
  input["MousePos"] = atInput::MousePos;
  input["MouseScroll"] = atInput::MouseScroll;
  input["MouseScrollX"] = atInput::MouseScrollX;
  input["MouseScrollY"] = atInput::MouseScrollY;
  input["MouseMoved"] = atInput::MouseMoved;
  input["MouseDelta"] = atInput::MouseDelta;

  auto &codes = input["Codes"].get_or_create<sol::table>();
  for (int64_t k = 0; k < atKC_Count + atKC_MB_Count; ++k)
  {
    const atString name = atInput::ToString(k);
    if (name.length() > 0)
      codes[name] = k;
  }
}

void atLua::ExposeMathTypes()
{
  sol::state &lua = *m_pLua;
  auto &math = lua["atMath"].get_or_create<sol::table>();

  // Expose Vector Classes
  math.new_usertype<atVec2D>("Vec2",
    sol::constructors<atVec2D(double, double)>(),
    "x", &atVec2D::x,
    "y", &atVec2D::y,
    "Add", &atVec2D::Add,
    "Sub", &atVec2D::Sub,
    "Mul", &atVec2D::Mul,
    "Div", &atVec2D::Div);

  math.new_usertype<atVec3D>("Vec3",
    sol::constructors<atVec3D(double, double, double)>(),
    "x", &atVec3D::x,
    "y", &atVec3D::y,
    "z", &atVec3D::z,
    "Add", &atVec3D::Add,
    "Sub", &atVec3D::Sub,
    "Mul", &atVec3D::Mul,
    "Div", &atVec3D::Div,
    "xy", &atVec3D::xy,
    "xz", &atVec3D::xz,
    "yz", &atVec3D::yz);

  math.new_usertype<atVec4D>("Vec4",
    sol::constructors<atVec4D(double, double, double, double)>(),
    "x", &atVec4D::x,
    "y", &atVec4D::y,
    "z", &atVec4D::z,
    "w", &atVec4D::w,
    "Add", &atVec4D::Add,
    "Sub", &atVec4D::Sub,
    "Mul", &atVec4D::Mul,
    "Div", &atVec4D::Div,
    "xy", &atVec4D::xy,
    "xz", &atVec4D::xz,
    "xw", &atVec4D::xw,
    "yz", &atVec4D::yz,
    "yw", &atVec4D::yw,
    "zw", &atVec4D::zw,
    "xzw", &atVec4D::xzw,
    "xyw", &atVec4D::xyw,
    "yzw", &atVec4D::yzw);

  math.new_usertype<atVec2I>("Vec2I",
    sol::constructors<atVec2I(int32_t, int32_t)>(),
    "x", &atVec2I::x,
    "y", &atVec2I::y,
    "Add", &atVec2I::Add,
    "Sub", &atVec2I::Sub,
    "Mul", &atVec2I::Mul,
    "Div", &atVec2I::Div);

  math.new_usertype<atVec3I>("Vec3I",
    sol::constructors<atVec3I(int32_t, int32_t, int32_t)>(),
    "x", &atVec3I::x,
    "y", &atVec3I::y,
    "z", &atVec3I::z,
    "Add", &atVec3I::Add,
    "Sub", &atVec3I::Sub,
    "Mul", &atVec3I::Mul,
    "Div", &atVec3I::Div,
    "xy", &atVec3I::xy,
    "xz", &atVec3I::xz,
    "yz", &atVec3I::yz);

  math.new_usertype<atVec4I>("Vec4I",
    sol::constructors<atVec4I(int32_t, int32_t, int32_t, int32_t)>(),
    "x", &atVec4I::x,
    "y", &atVec4I::y,
    "z", &atVec4I::z,
    "w", &atVec4I::w,
    "Add", &atVec4I::Add,
    "Sub", &atVec4I::Sub,
    "Mul", &atVec4I::Mul,
    "Div", &atVec4I::Div,
    "xy", &atVec4I::xy,
    "xz", &atVec4I::xz,
    "xw", &atVec4I::xw,
    "yz", &atVec4I::yz,
    "yw", &atVec4I::yw,
    "zw", &atVec4I::zw,
    "xzw", &atVec4I::xzw,
    "xyw", &atVec4I::xyw,
    "yzw", &atVec4I::yzw);

  // Expose Vector Math Functions
  // math["Dot"] = sol::overload(
  //   (double(*)(const atVec2D&, const atVec2D&)))atVec2D::Dot,
  //   (double(*)(const atVec3D&, const atVec3D&)))atVec3D::Dot,
  //   (double(*)(const atVec4D&, const atVec4D&)))atVec4D::Dot);
  // math["Mag"] = sol::overload(
  //   (double(*)(const atVec2D&))atVec2D::Mag,
  //   (double(*)(const atVec3D&))atVec3D::Mag,
  //   (double(*)(const atVec4D&))atVec4D::Mag);
  // math["Reflect"] = sol::overload(
  //   (atVec2D(*)(const atVec2D&, const atVec2D&)))atVec2D::Reflect,
  //   (atVec3D(*)(const atVec3D&, const atVec3D&)))atVec3D::Reflect,
  //   (atVec4D(*)(const atVec4D&, const atVec4D&)))atVec4D::Reflect);
  // math["Cross"] = (atVec3D(*)(const atVec3D&, const atVec3D&))atVec3D::Cross;
  // math["Normalize"] = sol::overload(
  //   (atVec2D(*)(const atVec2D&)))atVec2D::Normalize,
  //   (atVec3D(*)(const atVec3D&)))atVec3D::Normalize,
  //   (atVec4D(*)(const atVec4D&)))atVec4D::Normalize);

  // Expose Matrix Class
  // math.new_usertype<atMat4D>("Mat4",
  //   "m", &atMat4D::m_data,
  //   "Inverse", &atMat4D::Inverse,
  //   "Determinate", &atMat4D::Determinate,
  //   "Identity", &atMat4D::Identity,
  //   "Add", &atMat4D::Add,
  //   "Sub", &atMat4D::Sub,
  //   "Mul", &atMat4D::Mul);
}
