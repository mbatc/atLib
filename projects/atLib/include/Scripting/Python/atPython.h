#ifndef atPython_h__
#define atPython_h__

#include "atString.h"
#include "atHashMap.h"
#include "..\3rdParty\python\include\Python.h"

enum atPythonResult
{
  atPyR_Success,
  atPyR_Exception,
  atPyR_InvalidArg,
  atPyR_Unknown,
};

class atPython
{
public:
  // Modules must be added before Init() is called
  static void AddModule(const atString &name, PyObject* (*InitFunc)(void));
  
  // Initialize the python interpreter
  static void Init();

  // Destroy the python interpreter
  static void Shutdown();

  // Run a python script. Init() must have been called sometime prior to this
  static atPythonResult Run(const atString &script);

  // Check if the previous python operation succeeded.
  // Will report any errors, including a stack trace
  static bool CheckSuccess();
};

#endif // atPython_h__
