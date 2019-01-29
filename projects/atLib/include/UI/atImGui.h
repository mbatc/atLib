
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

#ifndef atImGui_h__
#define atImGui_h__

#include "atWindow.h"
#include "../../3rdParty/imgui/imgui.h"

// Consider moving towards an object based system for safer functions
// e.g. to create a window instantion an atImGui::Window() which will call Begin() on construction
// and End() in destruction.
// This will hopefully remove mismatched Begin()/End() funcitons

class atImGui
{
public:
  static bool BeginFrame(atWindow *pWnd);
  static bool EndFrame();
  static bool Render();
  static bool ProcessMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

  static bool Begin(const char *name);
  static bool Begin(const char *name, const atVec2D &size);
  static bool Begin(const char *name, const atVec2D &size, const atVec2D &pos);
  static void End();

  static const char* TextInput(const char *name, const char *initial);

  static void Text(const char *text);
  static bool Button(const char *label);
  static bool Button(const char *label, const atVec2D &size);
  static bool Button(const char *label, const atVec2D &size, const atVec2D &pos);
  static bool Selectable(const char *label, const bool selected);
  static bool Selectable(const char *label, const bool selected, const atVec2D &pos);
  static bool BeginTreeNode(const char *label);
  static void EndTreeNode();

  static bool BeginChild(const char *id, const atVec2D &size, const bool border);
  static bool BeginChild(const char *id, const atVec2D &size);
  static bool BeginChild(const char *id);
  static void EndChild();

  static void NewLine();
  static void SameLine();
  static void PushID(const char *label);
  static void PushID(const int64_t id);
  static void PopID();

  static void AddColours(const char *id, const char *colourString);
  static void RemoveColours(const char *id);

  static void AddStyle(const char *id, const char *colourString);
  static void RemoveStyle(const char *id);

  static void PushColour(const char *colourName, const atVec4D &colour);
  static void PushColour(const char *colourID);
  static void PopColour(int64_t count);
  static void PopColour();

  static void PushStyle(const char *styleName, const atVec2D &val);
  static void PushStyle(const char *styleName, const double val);
  static void PushStyle(const char *styleID);
  static void PopStyle(int64_t count);
  static void PopStyle();

};

#endif // atImGui_h__
