#ifndef _atPlatform_h__
#define _atPlatform_h__

#if _MSC_VER
#define atMSVC 1
#define atCOMPILER atMSVC
#endif

#ifdef __GNUC__
#define atGCC 2
#define atCOMPILER atGCC
#endif

#ifdef __clang__
#define atCLANG 3
#define atCOMPILER atCLANG
#endif

#ifdef __MINGW64__
#define atMINGW 4
#define atCOMPILER atMINGW
#endif

#if !defined(atCOMPILER) && !defined(atNO_COMPILER_WARNING)
static_assert(false, "Building on an unsupported compiler. Specify atNO_COMPILER_WARNING to disregard this message.");
#endif

#if atMSVC
#define atFUNCSIG __FUNCSIG__
#else
#define atFUNCSIG __PRETTY_FUNCTION__
#endif

#if defined(_WIN32) || defined(_WIN64)
#ifndef atPLATFORM_WIN32
#define atPLATFORM_WIN32
#endif
#elif defined(__linux__)
#ifndef atPLATFORM_LINUX
#define atPLATFORM_LINUX
#endif
#else
static_assert(false, "Building on an unsupported platform. Supported platforms are Windows and Linux systems.");
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

#endif // _atPlatform_h__

// This is outside the header guards in case atPlatform is included without including system headers.
// This is so that atPlatform can be included in low level headers without bloating the namespace
// or causing dependency issues
#ifndef atPLATFORM_NO_SYS_INCLUDE
#ifndef atPLATFORM_HAS_SYS_INCLUDE
#ifdef atPLATFORM_WIN32

#include <Windows.h>

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

#endif // atPLATFORM_WIN32 elif atPLATFORM_LINUX

#define atPLATFORM_HAS_SYS_INCLUDE
#endif // atPLATFORM_HAS_SYS_INCLUDE
#endif // atPLATFORM_NO_SYS_INCLUDE
