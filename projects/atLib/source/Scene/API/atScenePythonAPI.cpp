#include "atScenePythonAPI.h"

static PyObject* _PyComponent_FromComponent(atSceneComponent *pNode) { return PyCapsule_New(pNode, "atSceneComponent", 0); }

static atSceneComponent* _PyComponent_AsComponent(PyObject *pNode) { return (atSceneComponent*)PyCapsule_GetPointer(pNode, "atSceneComponent"); }

static PyObject* _Scene_CreateNode(PyObject *pSelf, PyObject *pArgs)
{
  const char *name = nullptr;
  int64_t parentID = atHierarchy_atRootNodeID;
  if (!PyArg_ParseTuple(pArgs, "zL", &name, &parentID))
  {
    atPython::CheckSuccess();
    return nullptr;
  }

  return PyLong_FromLongLong(atSceneAPI::Scene_AddNode(name ? name : "", parentID));
}

static PyObject* _Scene_GetComponent(PyObject *pSelf, PyObject *pArgs)
{
  int64_t nodeID, componentIndex = 0, componentType = atCCT_Unknown;
  if (!PyArg_ParseTuple(pArgs, "L|LL", &nodeID, &componentIndex, &componentType))
  {
    atPython::CheckSuccess();
    return nullptr;
  }

  atSceneComponent *pComp;
  if (componentType != atCCT_Unknown)
    pComp = atSceneAPI::Node_GetComponent(nodeID, componentIndex);
  else
    pComp = atSceneAPI::Node_GetComponentTyped(nodeID, atCoreComponentType(componentType), componentIndex);

  if (!pComp)
    Py_RETURN_NONE;

  return _PyComponent_FromComponent(pComp);
}

static PyObject* _Scene_GetComponentCount(PyObject *pSelf, PyObject *pArgs)
{
  int64_t nodeID, componentType = atCCT_Unknown;
  if (!PyArg_ParseTuple(pArgs, "L|L", &nodeID, &componentType))
  {
    atPython::CheckSuccess();
    return nullptr;
  }

  int64_t compCount = 0;
  if (componentType != atCCT_Unknown)
    compCount = atSceneAPI::Node_GetComponentCount(nodeID);
  else
    compCount = atSceneAPI::Node_GetComponentCountTyped(nodeID, atCoreComponentType(componentType));

  return PyLong_FromLongLong(compCount);
}

static PyObject* _Scene_GetNode(PyObject *pSelf, PyObject *pArgs)
{
  const char *name;
  if (!PyArg_ParseTuple(pArgs, "z", &name))
  {
    atPython::CheckSuccess();
    return nullptr;
  }

  return PyLong_FromLongLong(atSceneAPI::Scene_FindNodeByName(name));
}

static PyObject* _Scene_GetNodes(PyObject *pSelf, PyObject *pArgs)
{
  const char *name;
  if (!PyArg_ParseTuple(pArgs, "z", &name))
  {
    atPython::CheckSuccess();
    return nullptr;
  }

  atVector<int64_t> nodeIDs = atSceneAPI::Scene_FindNodesByName(name);
  PyObject *pList = PyList_New(nodeIDs.size());
  for (int64_t i = 0; i < nodeIDs.size(); ++i)
    PyList_SetItem(pList, i, PyLong_FromLongLong(nodeIDs[i]));
  return pList;
}

static PyObject* _Scene_DeleteNode(PyObject *pSelf, PyObject *pArgs)
{
  int64_t nodeID;
  if (!PyArg_ParseTuple(pArgs, "L", &nodeID))
  {
    atPython::CheckSuccess();
    return nullptr;
  }

  atSceneAPI::Scene_DeleteNode(nodeID);
  Py_RETURN_NONE;
}

static PyObject* _Node_AddComponent(PyObject *pSelf, PyObject *pArgs)
{
  int64_t nodeID, componentType;
  const char *componentName = nullptr;
  if (!PyArg_ParseTuple(pArgs, "LL|z", &nodeID, &componentType, &componentName))
  {
    atPython::CheckSuccess();
    return nullptr;
  }
  
  if (atSceneAPI::Node_AddComponent(nodeID, (atCoreComponentType)componentType, componentName ? componentName : ""))
    Py_RETURN_TRUE;

  Py_RETURN_FALSE;
}

static PyObject* _Node_GetName(PyObject *pSelf, PyObject *pArgs)
{
  int64_t nodeID;
  if (!PyArg_ParseTuple(pArgs, "L", &nodeID))
  {
    atPython::CheckSuccess();
    return nullptr;
  }

  return PyBytes_FromString(atSceneAPI::Node_GetName(nodeID));
}

static PyObject* _Node_SetName(PyObject *pSelf, PyObject *pArgs)
{
  int64_t nodeID;
  const char *name;
  if (!PyArg_ParseTuple(pArgs, "Lz", &nodeID, &name))
  {
    atPython::CheckSuccess();
    return nullptr;
  }

  if (atSceneAPI::Node_SetName(nodeID, name))
    Py_RETURN_TRUE;

  Py_RETURN_FALSE;
}

static PyMethodDef _methods[] =
{
  { "CreateNode", _Scene_CreateNode, METH_VARARGS, "Create a node in the scene." },
  { "DeleteNode", _Scene_DeleteNode, METH_VARARGS, "Delete a node with the specified ID." },
  { "GetComponent", _Scene_GetComponent, METH_VARARGS, "Get a nodes component at the specified index (optionally specify the type)." },
  { "GetComponentCount", _Scene_GetComponentCount, METH_VARARGS, "Get the number of components attached to a node." },
  { "GetNodeByName", _Scene_GetNode, METH_VARARGS, "Get the first node found with the specified name." },
  { "GetNodesByName", _Scene_GetNodes, METH_VARARGS, "Get all nodes with the specified name." },
  { "AddComponent", _Node_AddComponent, METH_VARARGS, "Add a component to the specified node." },
  { "GetNodeName", _Node_GetName, METH_VARARGS, "Get the specified node's name." },
  { "SetNodeName", _Node_SetName, METH_VARARGS, "Set the specified node's name." },
  { NULL, NULL, 0, NULL }
};

static PyModuleDef _modDef =
{
  PyModuleDef_HEAD_INIT,
  "atScene",   /* name of module */
  "atScene API python module.", /* module documentation, may be NULL */
  -1,       /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
  _methods
};

PyMODINIT_FUNC atScenePythonAPI_Init()
{
  PyObject *pModule = PyModule_Create(&_modDef);

  // Add component type constants
  PyModule_AddIntConstant(pModule, "CCT_Transform", atCCT_Transform);
  PyModule_AddIntConstant(pModule, "CCT_Mesh", atCCT_Mesh);
  PyModule_AddIntConstant(pModule, "CCT_Script", atCCT_Script);
  PyModule_AddIntConstant(pModule, "CCT_Control", atCCT_Control);
  PyModule_AddIntConstant(pModule, "CCT_Camera", atCCT_Camera);

  return pModule;
}
