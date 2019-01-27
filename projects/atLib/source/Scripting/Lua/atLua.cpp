
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
#include "atScene.h"
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
}

// SCENE

void atLua::ExposeScene()
{
  sol::state &lua = *m_pLua;

  // TODO:  Create atLuaScene to wrapper atScene calls using Lua compatible types
  //        atLuaScene should create a scene and store a pointer to it internally.
  //        The interface should use ID's which can be passed between LUA and C++
  //        to access the scene and it's contents

  // TODO:  Create atLuaSceneNode to wrapper atSceneNode class using Lua compatible types
  //        Should work similarly to atLuaScene accept with atSceneNode as apposed to 
  //        atScene

  // lua.new_usertype<atScene>("atScene",
  //   "NodeIDs", &atScene::GetNodeIDs,
  //   "CreateNode", &atScene::CreateNode,
  //   "DeleteNode", &atScene::DeleteNode,
  //   "AddActiveCamera", &atScene::AddActiveCamera,
  //   "RemoveActiveCamera", &atScene::RemoveActiveCamera);

  auto &scene = lua["atScene"].get_or_create<sol::table>();
  auto &node = scene["Node"].get_or_create<sol::table>();
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
