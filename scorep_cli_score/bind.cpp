#include <Python.h>
#include "structmember.h"

#include <cstdlib>
#include <iostream>
#include <string>

#include <scorep/SCOREP_Score_Estimator.hpp>
#include <scorep/SCOREP_Score_Profile.hpp>
#include <scorep/SCOREP_Score_Types.hpp>
#include <scorep/SCOREP_Score_Group.hpp>

static PyObject* bind_getScorepTypeNum(PyObject* self, PyObject* args)
{    
    return PyLong_FromLong(SCOREP_SCORE_TYPE_NUM);
}

static PyObject*
bind_getTypeName(PyObject* self, PyObject* args)
{    
    uint64_t type;
    const char* type_name;

    if(!PyArg_ParseTuple(args, "K", &type))
    {
        return NULL;
    }

    type_name = SCOREP_Score_getTypeName(type).c_str();

    return PyUnicode_FromString(type_name);
}

static PyMethodDef BindMethods[] = {
    {"getScorepTypeNum", bind_getScorepTypeNum, METH_VARARGS,
     "get number of types"},
    {"getTypeName", bind_getTypeName, METH_VARARGS,
     "get type name"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef bindmodule = {
    PyModuleDef_HEAD_INIT,
    "bind",
    NULL,
    -1, 
    BindMethods
};

PyMODINIT_FUNC
PyInit_bind(void)
{
    PyObject *m;
    std::string file_name = "profile.cubex";

    m = PyModule_Create(&bindmodule);
    if (m == NULL){
        return NULL;
    }

    return m;
}