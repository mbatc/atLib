#ifndef _atPlatform_h__
#define _atPlatform_h__

#ifdef atPLATFORM_WIN32
#include <windows.h>
#define atSysIconHandle   HICON
#define atSysWndHandle    HWND
#define atSysCursorHandle HCURSOR
#define atSysMenuHandle 	HMENU

typedef LRESULT(*atSysWndCallback)(HWND, UINT, WPARAM, LPARAM);

#elif atPLATFORM_LINUX
#define atSysIconHandle   int
#define atSysWndHandle    int
#define atSysCursorHandle int
#define atSysMenuHandle 	int
typedef int(*atSysWndCallback)();
#else
	static_assert(false, "A platform must be specified. Can be atPLATFORM_WIN32 or atPLATFORM_LINUX");
#endif

enum atPlatform_OS
{
	atPlatform_OS_None,
	atPlatform_OS_Win32,
	atPlatform_OS_Linux,
};

class atPlatform
{
public:
	static constexpr atPlatform_OS GetOS(); 
};

#endif