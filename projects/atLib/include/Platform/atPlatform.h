#ifndef _atPlatform_h__
#define _atPlatform_h__

#ifdef atPLATFORM_WIN32
#include <windows.h>
typedef HICON   atSysIconHandle;
typedef HWND    atSysWndHandle;
typedef HCURSOR atSysCursorHandle;
typedef HMENU   atSysMenuHandle;

typedef LRESULT(__stdcall *atSysWndCallback)(HWND, UINT, WPARAM, LPARAM);

#elif atPLATFORM_LINUX

#include <unistd.h>
#include <errno.h>

typedef int atSysIconHandle;
typedef int atSysWndHandle;
typedef int atSysCursorHandle;
typedef int atSysMenuHandle;
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