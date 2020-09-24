#include "atDisplay.h"
#include "atWinAPI.h"

int64_t atDisplay::Count() { return GetSystemMetrics(SM_CMONITORS); }
int64_t atDisplay::MaximizedWidth() { return GetSystemMetrics(SM_CXMAXIMIZED); }
int64_t atDisplay::MaximizedHeight() { return GetSystemMetrics(SM_CYMAXIMIZED); }
atVec2I atDisplay::MaximizedResolution() { return { MaximizedWidth(), MaximizedHeight() }; }
int64_t atDisplay::ScreenWidth() { return GetSystemMetrics(SM_CXSCREEN); }
int64_t atDisplay::ScreenHeight() { return GetSystemMetrics(SM_CYSCREEN); }
atVec2I atDisplay::ScreenResolution(){ return { ScreenWidth(), ScreenHeight() }; }
