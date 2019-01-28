
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

#define SOL_CHECK_ARGUMENTS 1
#include "atLua.h"
#include "sol.hpp"
#include "atImGui.h"
#include "atLuaScene.h"
#include "atVectorMath.h"

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

atLua::atLua()
{
  sol::state lua;
  m_pLua = atNew<sol::state>();
  m_pLua->set_exception_handler(_ExceptionHandler);
  m_pLua->open_libraries();
  ExposeContainers();
}

atLua::~atLua() { atDelete(m_pLua); }
sol::state * atLua::GetLua() { return m_pLua; }
bool atLua::RunText(const atString &command) { return m_pLua->script(command.c_str()).valid(); }
bool atLua::RunFile(const atString &filename) { return m_pLua->script_file(filename.c_str()).valid(); }

// DEAR IMGUI

void atLua::ExposeImGui()
{
  sol::state &lua = *m_pLua;

  auto &gui = lua["atImGui"].get_or_create<sol::table>();
  gui["Begin"] = sol::overload(
    (bool(*)(const char*, const atVec2F&, const atVec2F&))atImGui::Begin,
    (bool(*)(const char*, const atVec2F&))atImGui::Begin,
    (bool(*)(const char*))atImGui::Begin);

  gui["End"] = atImGui::End;
  
  gui["Button"] = sol::overload(
    (bool(*)(const char*, const atVec2F&, const atVec2F&))atImGui::Button,
    (bool(*)(const char*, const atVec2F&))atImGui::Button,
    (bool(*)(const char*))atImGui::Button);

  gui["Selectable"] = sol::overload(
    (bool(*)(const char*, const bool, const atVec2F&))atImGui::Selectable,
    (bool(*)(const char*, const bool))atImGui::Selectable);

  gui["Text"] = atImGui::Text;

  gui["BeginTreeNode"] = atImGui::BeginTreeNode;

  gui["EndTreeNode"] = atImGui::EndTreeNode;

  gui["PushID"] = sol::overload((void(*)(const int64_t))atImGui::PushID, (void(*)(const char*))atImGui::PushID);

  gui["PopID"] = atImGui::PopID;
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
  node.set("GetSiblings", &atLuaSceneNode::GetSiblings);
  node.set("GetSiblingID", &atLuaSceneNode::GetSiblingID);
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
    "x", &atVec2D::x,
    "x", &atVec2D::y,
    "Add", &atVec2D::Add,
    "Sub", &atVec2D::Sub,
    "Mul", &atVec2D::Mul,
    "Div", &atVec2D::Div);

  math.new_usertype<atVec3D>("Vec3",
    "x", &atVec3D::x,
    "x", &atVec3D::y,
    "z", &atVec3D::z,
    "Add", &atVec3D::Add,
    "Sub", &atVec3D::Sub,
    "Mul", &atVec3D::Mul,
    "Div", &atVec3D::Div,
    "xy", &atVec3D::xy,
    "xz", &atVec3D::xz,
    "yz", &atVec3D::yz);

  math.new_usertype<atVec4D>("Vec4",
    "x", &atVec4D::x,
    "x", &atVec4D::y,
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
