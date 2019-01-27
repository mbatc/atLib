
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

class atImGui
{
public:
  static bool BeginFrame(atWindow *pWnd);
  static bool EndFrame();
  static bool Render();
  static bool ProcessMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

  static bool Begin(const char *name);
  static bool Begin(const char *name, const atVec2F &size);
  static bool Begin(const char *name, const atVec2F &size, const atVec2F &pos);
  static void End();

  static void Text(const char *text);
  static bool Button(const char *label);
  static bool Button(const char *label, const atVec2F &size);
  static bool Button(const char *label, const atVec2F &size, const atVec2F &pos);
  static bool Selectable(const char *label, const bool selected);
  static bool Selectable(const char *label, const bool selected, const atVec2F &pos);
};

#endif // atImGui_h__
