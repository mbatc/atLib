
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

#include "atButtonState.h"

static void _DoBTLogic(const bool newState, bool *pState, bool *pPressed, bool *pReleased, double *pDwnTime, double *pUpTime, int64_t *pDblClickCnt, const double dt)
{
  *pPressed = !(*pState) && newState;
  *pReleased = (*pState) && !newState;
  *pDwnTime = !(*pState) || *pReleased ? 0.0 : *pDwnTime + dt;
  *pUpTime = *pState || *pPressed ? 0.0 : *pUpTime + dt;
  *pState = newState;
  *pDblClickCnt < 0.2 && *pPressed ? *pDblClickCnt++ : *pDblClickCnt = 0;
}

void atButtonState::OnDown(const double dt) { _DoBTLogic(true, &m_down, &m_pressed, &m_released, &m_downTime, &m_upTime, &m_dblClickCount, dt); }
void atButtonState::OnUp(const double dt) { _DoBTLogic(false, &m_down, &m_pressed, &m_released, &m_downTime, &m_upTime, &m_dblClickCount, dt); }
bool atButtonState::IsPressed() const { return m_pressed; }
bool atButtonState::IsReleased() const { return m_released; }
bool atButtonState::IsDown() const { return m_down; }
bool atButtonState::IsUp() const { return !m_down; }
bool atButtonState::IsDoubleClicked() const { return m_dblClickCount == 1; }
double atButtonState::TimeUp() const { return m_upTime; }
double atButtonState::TimeDown() const { return m_downTime; }
int64_t atButtonState::DoubleClickCount() const { return m_dblClickCount; }
