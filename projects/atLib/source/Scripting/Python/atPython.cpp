#include "atWinAPI.h"
#include "atPython.h"
#include "python.h"

bool atPython::CheckSuccess()
{
  PyObject *pError = PyErr_Occurred();
  if (pError == nullptr)
    return true;
  
  static PyObject *TracebackFunc = nullptr;
  PyObject *pType = nullptr, *pValue = nullptr, *pTraceback = nullptr;
  PyErr_Fetch(&pType, &pValue, &pTraceback);
  PyErr_NormalizeException(&pType, &pValue, &pTraceback);

  atVector<atString> tracebackStack;

  if (pTraceback)
  {
    if (!TracebackFunc)
    {
      PyObject *pTracebackMod = PyImport_ImportModule("traceback");
      if (pTracebackMod)
        TracebackFunc = PyObject_GetAttrString(pTracebackMod, "format_exception");
    }

    if (TracebackFunc && PyCallable_Check(TracebackFunc))
    {
      PyObject *pRet = PyObject_CallFunctionObjArgs(TracebackFunc, pType, pValue, pTraceback, NULL);
      int64_t size = PyList_GET_SIZE(pRet);
      for (int64_t i = 0; i < size; ++i)
      {
        PyObject *pItem = PyList_GetItem(pRet, i);
        tracebackStack.emplace_back(PyBytes_AsString(pItem));
        Py_DECREF(pItem);
      }
    }

    if (tracebackStack.size())
    {
      atString traceback = atString::join(tracebackStack, "\n");
      printf("%s\n", traceback.c_str());
    }
  }
  else
  {
    const char *errorStr = PyBytes_AsString(pError);
    const char *message = PyBytes_AsString(pValue);
    const char *typeStr = PyBytes_AsString(pType);
    printf("%s, %s", message, typeStr);
  }

  if (pType)      Py_DECREF(pType);
  if (pValue)     Py_DECREF(pValue);
  if (pTraceback) Py_DECREF(pTraceback);

  return false;
}

static atVector<atKeyValue<atString*, PyObject *(*)()>> _modules;

void atPython::AddModule(const atString &name, PyObject *(*InitFunc)(void))
{
  _modules.push_back({ atNew<atString>(name), InitFunc });
  atAssert(PyImport_AppendInittab(_modules.back().m_key->c_str(), InitFunc) != -1, "atPython: Unable to append init table.");
}

void atPython::Init()
{
  int argc;
  LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
  if (argv)
  {
    Py_SetProgramName(argv[0]);
    CheckSuccess();
    LocalFree(argv);
  }

  Py_Initialize();
  CheckSuccess();
}

void atPython::Shutdown()
{
  Py_Finalize();
  CheckSuccess();

  for (auto &kvp : _modules)
    atDelete(kvp.m_key);
  _modules.clear();
}

atPythonResult atPython::Run(const atString &script) { return atPythonResult(PyRun_SimpleString(script.c_str())); }
