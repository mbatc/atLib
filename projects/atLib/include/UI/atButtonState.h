
// -----------------------------------------------------------------------------
// The MIT License
// 
// Copyright(c) 2020 Michael Batchelor, 
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

#ifndef _atButtonState_h__
#define _atButtonState_h__

#include "atTypes.h"

class atButtonState
{
public:
  void OnDown(const double dt);
  void OnUp(const double dt);

  bool IsPressed() const;
  bool IsReleased() const;
  bool IsDown() const;
  bool IsUp() const;
  bool IsDoubleClicked() const;

  int64_t DoubleClickCount() const;
  double TimeDown() const;
  double TimeUp() const;

  void Update(const double dt);

protected:
  bool m_pressed = false;
  bool m_released = false;
  bool m_down = false;
  int64_t m_dblClickCount = 0;
  double m_downTime = 0.0;
  double m_upTime = 0.0;
};

#endif