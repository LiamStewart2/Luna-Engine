#include "Application.h"

#include <Python.h>

int main()
{
	Py_Initialize();

	PyRun_SimpleString("print(7489217 + 4781)");

	Py_Finalize();

	Application application;
	return 0;
}