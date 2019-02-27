
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

#include "atHashMap.h"
#include "atWindow.h"
#include "atImGui.h"
#include "atInput.h"
#include "atWinAPI.h"
#include <Windowsx.h>
#include <time.h>

atWindow::atWindow(const atString &title, const atVec2I &size, const bool hardware, const atVec2I &pos, const bool windowed, const int64_t style)
  : m_dxTarget(this, size)
  , m_hardware(hardware)
  , m_sysWindow(this)
{
  SetTitle(title);
  SetSize(size);
  SetPos(pos);
  SetStyle(style);
  SetWindowed(windowed);
  MakeWindow();
}

atWindow::~atWindow() { Destroy(); }

void atWindow::Clear(const atVec4F &color) { Clear(atColor::Pack(color)); }

void atWindow::Clear(const atCol color)
{
  if (m_hardware)
    m_dxTarget.Clear(atColor::UnPack<float>(color));
  else
    m_sysWindow.Clear(color);
}

void atWindow::Swap()
{
  if (m_hardware)
    m_dxTarget.Swap();
  else
    m_sysWindow.Swap();
}

void atWindow::SetTitle(const atString &title)
{
  m_title = title;
  m_sysWindow.SetTitle();
}

void atWindow::SetPos(const atVec2I &pos)
{
  m_pos = pos;
  m_sysWindow.SetWindowRect();
}

void atWindow::SetSize(const atVec2I &size)
{
  m_size = size;
  m_sysWindow.SetWindowRect();
}

void atWindow::OnResize()
{
  m_sysWindow.OnResize();
  m_dxTarget.Resize();
}

bool atWindow::MakeWindow() { return m_sysWindow.Create(); }
void atWindow::Destroy() { m_sysWindow.Destroy(); }

const atVec2I& atWindow::Position() const { return m_pos; }
const atVec2I& atWindow::Size() const { return m_clientSize; }
int32_t atWindow::GetX() const { return Position().x; }
int32_t atWindow::GetY() const { return Position().y; }
int32_t atWindow::Width() const { return Size().x; }
int32_t atWindow::Height() const { return Size().y; }

bool atWindow::IsWindowed() const { return m_windowed; }

void atWindow::SetStyle(const int64_t style) { m_style = style; }
void atWindow::SetWindowed(const bool windowed) { m_windowed = windowed; m_dxTarget.Resize(); }
void atWindow::SetMenu(HMENU hMenu) { m_sysWindow.SetMenu(hMenu); }
void atWindow::SetIcon(HICON hIcon) { m_sysWindow.SetIcon(hIcon); }
void atWindow::SetCursor(HCURSOR hCursor) { m_sysWindow.SetCursor(hCursor); }
void atWindow::SetParent(const atWindow &window) { m_sysWindow.SetParent(window.Handle()); }
void atWindow::SetWndProc(LRESULT(__stdcall *wndProc)(HWND, UINT, WPARAM, LPARAM)) { m_sysWindow.SetCallback(wndProc); }

atCol* atWindow::Pixels() { return (atCol*)m_sysWindow.Pixels().data(); }
const atVector<atCol>& atWindow::PixelsV() { return m_sysWindow.Pixels(); }
HWND atWindow::Handle() const { return m_sysWindow.Handle(); }

bool atWindow::PumpMessage() { return atWinAPI::PumpMessage(); }
int atWindow::GetResult() { return atWinAPI::GetResult(); }
