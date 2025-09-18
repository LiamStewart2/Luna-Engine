#include "Application.h"

#include <Python.h>

int main()
{
	Py_Initialize();

    PyObject* sysPath = PySys_GetObject("path");
    PyList_Append(sysPath, PyUnicode_FromString("Assets/Scripts"));

	PyObject* pName = PyUnicode_DecodeFSDefault("LunaBehaviour");
	PyObject* pModule = PyImport_Import(pName);
	Py_DECREF(pName);

	if (pModule != nullptr)
	{
        // Get Behaviour class
        PyObject* pClass = PyObject_GetAttrString(pModule, "LunaBehaviour");

        if (pClass && PyCallable_Check(pClass)) {
            // Create instance: Behaviour("Player")
            PyObject* pArgs = PyTuple_Pack(1, PyUnicode_FromString("Player"));
            PyObject* pInstance = PyObject_CallObject(pClass, pArgs);
            Py_DECREF(pArgs);

            if (pInstance != nullptr) {
                // Call update(dt)
                PyObject* pResult = PyObject_CallMethod(pInstance, "update", "(f)", 0.016f);
                if (!pResult) {
                    PyErr_Print();
                }
                else {
                    Py_DECREF(pResult);
                }
                Py_DECREF(pInstance);
            }
            else {
                PyErr_Print();
            }
        }
        else {
            std::cerr << "Could not find Behaviour class\n";
            PyErr_Print();
        }

        Py_XDECREF(pClass);
        Py_DECREF(pModule);
    }
    else {
        PyErr_Print();
        std::cerr << "Failed to load script.py\n";
	}
	
	Py_Finalize();

	Application application;
	return 0;
}