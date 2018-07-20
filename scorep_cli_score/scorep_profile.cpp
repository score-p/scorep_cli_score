#include <Python.h>
#include "structmember.h"

#include <memory>

#include <scorep/SCOREP_Score_Estimator.hpp>
#include <scorep/SCOREP_Score_Profile.hpp>
#include <scorep/SCOREP_Score_Types.hpp>

typedef struct {
    PyObject_HEAD
    const char *file_name;
    std::unique_ptr<SCOREP_Score_Profile> profile;
    std::unique_ptr<cube::Cube> cube;
} ScoreP_ProfileObject;

static void
ScoreP_Profile_dealloc(ScoreP_ProfileObject *self)
{
    Py_TYPE(self)->tp_free((PyObject *) self);
    self->profile.reset(nullptr);
    self->cube.reset(nullptr);
}

static PyObject *
ScoreP_Profile_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{   
    ScoreP_ProfileObject *self;
    self = (ScoreP_ProfileObject *) type->tp_alloc(type, 0);
    return (PyObject *) self;
}

static int
ScoreP_Profile_init(ScoreP_ProfileObject *self, PyObject *args, PyObject *kwds)
{
    static const char *kwlist[] = {"file_name", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "s", const_cast<char **>(kwlist),
                                     &self->file_name))
        return -1;

    try
    {
        self->cube = std::make_unique<cube::Cube>();
        self->cube->openCubeReport(self->file_name);
        self->profile = std::make_unique<SCOREP_Score_Profile>(self->cube.get());
    }
    catch (...)
    {
        PyErr_SetString(PyExc_RuntimeError, "Cannot open Cube report ");
    }

    return 0;
}

static PyMemberDef ScoreP_Profile_members[] = {
    {const_cast<char *>("file_name"), T_STRING, offsetof(ScoreP_ProfileObject, file_name), 0,
     const_cast<char *>("file name of profile")},
    {const_cast<char *>("profile"), T_OBJECT_EX, offsetof(ScoreP_ProfileObject, profile), 0,
     const_cast<char *>("SCOREP_Score_Profile")},
    {const_cast<char *>("cube"), T_OBJECT_EX, offsetof(ScoreP_ProfileObject, cube), 0,
     const_cast<char *>("cube")},
    {NULL}
};

static PyObject *
ScoreP_Profile_getNumberOfRegions(ScoreP_ProfileObject *self, PyObject *args)
{
    return PyLong_FromLong(self->profile->getNumberOfRegions());
}

static PyObject * 
ScoreP_Profile_getNumberOfProcesses(ScoreP_ProfileObject *self, PyObject *args)
{    
    return PyLong_FromLong(self->profile->getNumberOfProcesses());
}

static PyObject *
ScoreP_Profile_getRegionName(ScoreP_ProfileObject *self, PyObject *args)
{
    uint64_t region;
    const char* region_name;

    if(!PyArg_ParseTuple(args, "K", &region))
    {
        return NULL;
    }

    std::string tmp = self->profile->getRegionName(region);

    region_name = const_cast<char *>(tmp.c_str());

    return PyUnicode_FromFormat("%s", region_name);
}

static PyObject *
ScoreP_Profile_getMangledName(ScoreP_ProfileObject *self, PyObject *args)
{
    uint64_t region;
    const char* mangled_name;

    if(!PyArg_ParseTuple(args, "K", &region))
    {
        return NULL;
    }

    std::string tmp = self->profile->getMangledName(region);

    mangled_name = const_cast<char *>(tmp.c_str());

    return PyUnicode_FromFormat("%s", mangled_name);
}

static PyObject *
ScoreP_Profile_getGroup(ScoreP_ProfileObject *self, PyObject *args)
{
    SCOREP_Score_Type group;
    uint64_t region;
    const char* type;

    if(!PyArg_ParseTuple(args, "K", &region))
    {
        return NULL;
    }

    group = self->profile->getGroup(region);

    type = SCOREP_Score_getTypeName(group).c_str();

    return PyUnicode_FromFormat("%s", type);
}

static PyObject *
ScoreP_Profile_getVisits(ScoreP_ProfileObject *self, PyObject *args)
{
    uint64_t region;
    uint64_t process;
    uint64_t visits;

    if(!PyArg_ParseTuple(args, "KK", &region, &process))
    {
        return NULL;
    }

    visits = self->profile->getVisits(region, process);

    return PyLong_FromLong(visits);
}

static PyObject *
ScoreP_Profile_getTime(ScoreP_ProfileObject *self, PyObject *args)
{
    uint64_t region;
    uint64_t process;
    double time;
    
    if(!PyArg_ParseTuple(args, "KK", &region, &process))
    {
        return NULL;
    }

    time = self->profile->getTime(region, process);    

    return PyFloat_FromDouble(time);
}

static PyObject *
ScoreP_Profile_getInclusiveTime(ScoreP_ProfileObject *self, PyObject *args)
{
    uint64_t region;
    uint64_t process;
    double time;
    
    if(!PyArg_ParseTuple(args, "KK", &region, &process))
    {
        return NULL;
    }

    time = self->profile->getInclusiveTime(region, process);

    return PyFloat_FromDouble(time);
}

static PyObject *
ScoreP_Profile_getHits(ScoreP_ProfileObject *self, PyObject *args)
{
    uint64_t region;
    uint64_t process;
    uint64_t hits;

    if(!PyArg_ParseTuple(args, "KK", &region, &process))
    {
        return NULL;
    }

    hits = self->profile->getHits(region, process);

    return PyLong_FromLong(hits);
}

static PyMethodDef ScoreP_Profile_methods[] = {
    {"getNumberOfRegions", (PyCFunction) ScoreP_Profile_getNumberOfRegions, METH_NOARGS,
     "get number of regions"},
    {"getNumberOfProcesses", (PyCFunction) ScoreP_Profile_getNumberOfProcesses, METH_NOARGS,
     "get number of processes"},
    {"getRegionName", (PyCFunction) ScoreP_Profile_getRegionName, METH_VARARGS,
     "get region name"},
    {"getMangledName", (PyCFunction) ScoreP_Profile_getMangledName, METH_VARARGS,
     "get mangles name"},
    {"getGroup", (PyCFunction) ScoreP_Profile_getGroup, METH_VARARGS,
     "get group"},
    {"getVisits", (PyCFunction) ScoreP_Profile_getVisits, METH_VARARGS,
     "get number of visits"},
    {"getTime", (PyCFunction) ScoreP_Profile_getTime, METH_VARARGS,
     "get time"},
    {"getInclusiveTime", (PyCFunction) ScoreP_Profile_getInclusiveTime, METH_VARARGS,
     "get time"},
    {"getHits", (PyCFunction) ScoreP_Profile_getHits, METH_VARARGS,
     "get number of hits"},
    {NULL}
};

static PyTypeObject ScoreP_ProfileType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "scorep_profile.ScoreP_Profile",    /* tp_name */ //-> includes name of type (Profile) and module name (profile)
    sizeof(ScoreP_ProfileObject),       /* tp_basicsize */
    0,                                  /* tp_itemsize */
    (destructor)ScoreP_Profile_dealloc, /* tp_dealloc */
    0,                                  /* tp_print */
    0,                                  /* tp_getattr */
    0,                                  /* tp_setattr */
    0,                                  /* tp_as_async */
    0,                                  /* tp_repr */
    0,                                  /* tp_as_number */
    0,                                  /* tp_as_sequence */
    0,                                  /* tp_as_mapping */
    0,                                  /* tp_hash */
    0,                                  /* tp_call */
    0,                                  /* tp_str */
    0,                                  /* tp_getattro */
    0,                                  /* tp_setattro */
    0,                                  /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT |
        Py_TPFLAGS_BASETYPE,            /* tp_flags */
    "ScoreP_Profile objects",           /* tp_doc */
    0,                                  /* tp_traverse */
    0,                                  /* tp_clear */
    0,                                  /* tp_richcompare */
    0,                                  /* tp_weaklistoffset */
    0,                                  /* tp_iter */
    0,                                  /* tp_iternext */
    ScoreP_Profile_methods,             /* tp_methods */
    ScoreP_Profile_members,             /* tp_members */
    0,                                  /* tp_getset */
    0,                                  /* tp_base */
    0,                                  /* tp_dict */
    0,                                  /* tp_descr_get */
    0,                                  /* tp_descr_set */
    0,                                  /* tp_dictoffset */
    (initproc)ScoreP_Profile_init,      /* tp_init */
    0,                                  /* tp_alloc */
    ScoreP_Profile_new,                 /* tp_new , PyType_GenericNew */
    0,                                  /* tp_free */
    0,                                  /* tp_is_gc */
    0,                                  /* tp_bases */
    0,                                  /* tp_mro */
    0,                                  /* tp_cache */
    0,                                  /* tp_subclasses */
    0,                                  /* tp_weaklist */
    0,                                  /* tp_del */
    0,                                  /* tp_version_tag */
    0,                                  /* tp_finalize */
};

static PyModuleDef scorep_profilemodule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "scorep_profile",
    .m_doc = "Module that provides an extension type for handling Score-P profiles.",
    .m_size = -1,
};

PyMODINIT_FUNC
PyInit_scorep_profile(void)
{
    PyObject *m;
    if (PyType_Ready(&ScoreP_ProfileType) < 0)
        return NULL;

    m = PyModule_Create(&scorep_profilemodule);
    if (m == NULL)
        return NULL;

    Py_INCREF(&ScoreP_ProfileType);
    PyModule_AddObject(m, "ScoreP_Profile", (PyObject *) &ScoreP_ProfileType);
    return m;
}