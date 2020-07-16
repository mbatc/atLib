
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

#include "atGraphics.h"
#include "atHashMap.h"
#include "atWindow.h"
#include "atWinAPI.h"
#include "atImGui.h"
#include "atInput.h"

atWindow::atWindow(const atWindowCreateInfo &info)
  : m_sysWindow(this)
  , m_createInfo(info)
{
  MakeWindow(m_createInfo);
}

atWindow::~atWindow() { Destroy(); }

void atWindow::Clear(const atVec4F &color) { Clear(atColor::Pack(color)); }

void atWindow::Clear(const atCol color)
{
  if (m_pGfx)
    m_pGfx->Clear(atColor::UnPack<float>(color), 1.0f);
  else
    m_sysWindow.Clear(color);
}

void atWindow::Swap()
{
  if (m_pGfx)
    m_pGfx->Swap();
  else
    m_sysWindow.Swap();
}

void atWindow::SetTitle(const atString &title) { m_sysWindow.SetTitle(title); }
void atWindow::SetPos(const atVec2I &pos) { m_sysWindow.SetWindowRect(atVec4I(pos, Size())); }
void atWindow::SetSize(const atVec2I &size) { m_sysWindow.SetWindowRect(atVec4I(Position(), size)); }

void atWindow::OnResize()
{
  m_sysWindow.OnResize();
  if (m_pGfx)
    m_pGfx->Resize();
}

void atWindow::SetWindowed(const bool &windowed) { m_sysWindow.SetWindowed(windowed); }
void atWindow::SetVisible(const bool &visible) { m_sysWindow.SetVisible(visible); }
void atWindow::Maximize() { m_sysWindow.Maximize(); }
void atWindow::Minimize() { m_sysWindow.Minimize(); }
void atWindow::Restore() { m_sysWindow.Restore(); }

atWindowStyle atWindow::GetStyle() const { return m_sysWindow.GetStyle(); }
atString atWindow::GetTitle() const { return m_sysWindow.GetTitle(); }

void atWindow::Destroy()
{
  SetWindowed(true);
  m_sysWindow.Destroy();
}

bool atWindow::MakeWindow(const atWindowCreateInfo &info) { return m_sysWindow.Create(info); }

atVec2I atWindow::Position() const { return m_sysWindow.GetPos(); }
atVec2I atWindow::GetScreenPos(const atVec2I &pos) const { return m_sysWindow.GetScreenPos(pos); }
atVec2I atWindow::Size() const { return m_sysWindow.GetClientSize(); }
atVec2I atWindow::FullSize() const { return m_sysWindow.GetSize(); }
int32_t atWindow::GetX() const { return Position().x; }
int32_t atWindow::GetY() const { return Position().y; }
bool atWindow::IsMaximized() const { return m_sysWindow.IsMaximized(); }
bool atWindow::IsMinimized() const { return m_sysWindow.IsMinimized(); }
bool atWindow::IsRestored() const { return !(IsMinimized() || IsMaximized()); }

int32_t atWindow::Width() const { return Size().x; }
int32_t atWindow::Height() const { return Size().y; }

bool atWindow::IsWindowed() const { return m_sysWindow.IsWindowed(); }
bool atWindow::IsVisible() const { return m_sysWindow.IsVisible(); }
void atWindow::SetStyle(const atWindowStyle &style) { m_sysWindow.SetStyle(style); }

void atWindow::SetMenu(atSysMenuHandle hMenu) { m_sysWindow.SetMenu(hMenu); }
void atWindow::SetIcon(atSysIconHandle hIcon) { m_sysWindow.SetIcon(hIcon); }
void atWindow::SetCursor(atSysCursorHandle hCursor) { m_sysWindow.SetCursor(hCursor); }
void atWindow::SetParent(const atWindow &window) { m_sysWindow.SetParent(window.Handle()); }
void atWindow::SetWndProc(atSysWndCallback callback) { m_sysWindow.SetCallback(callback); }

atCol* atWindow::Pixels() { return m_sysWindow.Pixels().data(); }
const atCol* atWindow::Pixels() const { return m_sysWindow.Pixels().data(); }

atCol& atWindow::Pixel(const atVec2I &coord) { return Pixel(atToIndex<int64_t>(coord, Size())); }
atCol& atWindow::Pixel(const int64_t &index) { return Pixels()[index]; }
const atCol& atWindow::Pixel(const atVec2I &coord) const { return Pixel(atToIndex<int64_t>(coord, Size())); }
const atCol& atWindow::Pixel(const int64_t &index) const { return Pixels()[index]; }

atVector<atCol>& atWindow::PixelsV() { return m_sysWindow.Pixels(); }
const atVector<atCol>& atWindow::PixelsV() const { return m_sysWindow.Pixels(); }

HWND atWindow::Handle() const { return m_sysWindow.Handle(); }

atVector<atString> atWindow::DroppedFiles() { return std::move(m_droppedFiles); }

bool atWindow::PumpMessage() { return PumpMessage(this); }
bool atWindow::PumpMessage(atWindow *pWindow) { return atWinAPI::PumpMessage(pWindow); }
int atWindow::GetResult() { return atWinAPI::GetResult(); }

void atWindow::SetHardwareCtx(atGraphics *pGfx) { m_pGfx = pGfx; }
void atWindow::AddDroppedFile(const atString &file) { m_droppedFiles.push_back(file); }
