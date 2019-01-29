
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
#include "atLuaScene.h"
#include "atVectorMath.h"

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
  gui["Begin"] = sol::overload(
    (bool(*)(const char*, const atVec2D&, const atVec2D&))atImGui::Begin,
    (bool(*)(const char*, const atVec2D&))atImGui::Begin,
    (bool(*)(const char*))atImGui::Begin);

  gui["End"] = atImGui::End;
  
  gui["Button"] = sol::overload(
    (bool(*)(const char*, const atVec2D&, const atVec2D&))atImGui::Button,
    (bool(*)(const char*, const atVec2D&))atImGui::Button,
    (bool(*)(const char*))atImGui::Button);

  gui["Selectable"] = sol::overload(
    (bool(*)(const char*, const bool, const atVec2D&))atImGui::Selectable,
    (bool(*)(const char*, const bool))atImGui::Selectable);

  gui["Text"] = atImGui::Text;

  gui["TextInput"] = atImGui::TextInput;

  gui["BeginTreeNode"] = atImGui::BeginTreeNode;

  gui["EndTreeNode"] = atImGui::EndTreeNode;

  gui["PushID"] = sol::overload((void(*)(const int64_t))atImGui::PushID, (void(*)(const char*))atImGui::PushID);

  gui["PopID"] = atImGui::PopID;

  gui["SameLine"] = atImGui::SameLine;

  gui["NewLine"] = atImGui::NewLine;

  gui["BeginChild"] = sol::overload(
    (bool(*)(const char*, const atVec2D&, const bool))atImGui::BeginChild,
    (bool(*)(const char*, const atVec2D&))atImGui::BeginChild,
    (bool(*)(const char*))atImGui::BeginChild);

  gui["EndChild"] = atImGui::EndChild;

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
    &atLuaSceneNode::GetComponentsOyType));
  node.set("GetComponent", sol::overload(
    &atLuaSceneNode::GetComponent,
    &atLuaSceneNode::GetComponentOfType));
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

  // LUA Component Class
  auto &component = sceneNamespace.create_simple_usertype<atLuaSceneComponent>();
  component.set("TypeID", &atLuaSceneComponent::TypeID);

  sceneNamespace.set_usertype("Node", node);
  sceneNamespace.set_usertype("Scene", scene);
  sceneNamespace.set_usertype("Component", component);
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
    (double(*)(double, double))atATan2);

  math["Sin"] = atSin<double>;
  math["Cos"] = atCos<double>;
  math["Tan"] = atTan<double>;
  math["ASin"] = atASin<double>;
  math["ACos"] = atACos<double>;
  math["ATan"] = atATan<double>;
  math["QuadraticSolve"] = atQuadraticSolve<double>;
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

  // Expose Vector Math Functions
  math["Dot"] = sol::overload(
    (double(*)(const atVec2D&, const atVec2D&))atVectorMath::Dot,
    (double(*)(const atVec3D&, const atVec3D&))atVectorMath::Dot,
    (double(*)(const atVec4D&, const atVec4D&))atVectorMath::Dot);

  math["Mag"] = sol::overload(
    (double(*)(const atVec2D&))atVectorMath::Mag,
    (double(*)(const atVec3D&))atVectorMath::Mag,
    (double(*)(const atVec4D&))atVectorMath::Mag);

  math["Reflect"] = sol::overload(
    (atVec2D(*)(const atVec2D&, const atVec2D&))atVectorMath::Reflect,
    (atVec3D(*)(const atVec3D&, const atVec3D&))atVectorMath::Reflect,
    (atVec4D(*)(const atVec4D&, const atVec4D&))atVectorMath::Reflect);

  math["Cross"] = (atVec3D(*)(const atVec3D&, const atVec3D&))atVectorMath::Cross;

  math["Normalize"] = sol::overload(
    (atVec2D(*)(const atVec2D&))atVectorMath::Normalize,
    (atVec3D(*)(const atVec3D&))atVectorMath::Normalize,
    (atVec4D(*)(const atVec4D&))atVectorMath::Normalize);

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
