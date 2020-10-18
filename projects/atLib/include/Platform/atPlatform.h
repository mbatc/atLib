#ifndef _atPlatform_h__
#define _atPlatform_h__

// Compiler definitions
#if _MSC_VER // MS Visual studio / MS build
#define atMSVC 1
#define atCOMPILER atMSVC

#define atVS2019 5
#define atVS2017 4
#define atVS2015 3
#define atVS2013 2
#define atVS2012 1
#define atVS2010 0

// Determine the visual studio IDE version
#if _MSC_VER >= 1920
#define atMSVC_VER atVS2019
#define atIDE "vs2019"
#elif _MSC_VER >= 1910
#define atMSVC_VER atVS2017
#define atIDE "vs2017"
#elif _MSC_VER >= 1900
#define atMSVC_VER atVS2015
#define atIDE "vs2015"
#elif _MSC_VER >= 1800
#define atMSVC_VER atVS2013
#define atIDE "vs2013"
#elif _MSC_VER >= 1700
#define atMSVC_VER atVS2012
#define atIDE "vs2012"
#elif _MSC_VER >= 1600
#define atMSVC_VER atVS2010
#define atIDE "vs2010"
#endif
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

// Compiler specific Function/File/Line definitions
#if atMSVC
#define atFUNCSIG __FUNCSIG__
#else
#define atFUNCSIG __PRETTY_FUNCTION__
#endif

#define atLINE __LINE__
#define atFILE __FILE__

// OS Defines
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
