//////////////////////////////////////////////////////////////////////
//	MJim - Alpha
//	Stephens Atomics 
//	Copyright (C) 2013  Matthew James Stephens, B.Eng- Robotics(Hons)
//
//	This program is free software: you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation, either version 3 of the License, or
//	(at your option) any later version.
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//	<http://www.gnu.org/licenses/>   
//////////////////////////////////////////////////////////////////////

#include "OSG_DES.h"
#include "OSG_Event.h"
#include "OSG_XML_Parser_Tools.h"
#include "OSG_Tools.h"
#include "OSG_XML_Save_event_tree.h"

#include <string>
#include <sstream>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#undef _DEBUG 
#include <Python.h>
#define _DEBUG 


//#include <Python.h>


///
// CEvent_DE_Logistics
///
bool CEvent_Python::Create(CEventDesign* p_design, CEnvironment* p_env)
{	
	std::vector<CParsedVariable>::iterator iter;
	for(iter = p_design->mv_variable.begin(); iter < p_design->mv_variable.end(); ++iter) {	
		if(iter->m_identifier == "Key")
		{
			m_key = iter->m_str_value;
		}		
		if(iter->m_identifier == "Object")
		{
			m_object = iter->m_str_value;
		}		
	}		
	
	return true;
}

bool CEvent_Python::Update(CEnvironment* p_env)
{	
	PyObject *pName= NULL;
	PyObject *pModule;
	PyObject *pDict= NULL;
	PyObject *pFunc= NULL;
	PyObject *pValue= NULL;	
	
    Py_Initialize();
	cout << "Python initialized \n";

	PyRun_SimpleString("from time import time,ctime\n"
		"print('Today is', ctime(time()))\n"); 

	char buffer[MAX_PATH];
	DWORD a = GetCurrentDirectory(MAX_PATH, buffer);
	cout << "CurrentDirectory: " << buffer << "\n";

    pName = PyBytes_FromString(m_object.c_str());
	if(!pName) {
		cout << "PyBytes_FromString Fail: \n";
		return false; 	
	}
	cout << "Name: " << m_object << "\n";

	pModule = PyImport_Import(pName);
	if(pModule== NULL) {
		cout << "PyImport_Import Fail: \n";
		return false; 	
	}
	cout << "Module imported\n";

    pDict = PyModule_GetDict(pModule);
	if(!pDict) {
		cout << "PyModule_GetDict Fail: \n";
		return false; 	
	}
	cout << "GetDict\n";

	pFunc = PyDict_GetItemString(pDict, m_key.c_str());
	if(!pFunc) {
		cout << "PyDict_GetItemString Fail: \n";
		return false; 	
	}
	cout << "Key: " << m_key << "\n";

    if (PyCallable_Check(pFunc))  {
        PyObject_CallObject(pFunc, NULL);
    } 
	else { PyErr_Print();  }

    // Clean up
    Py_DECREF(pModule);
    Py_DECREF(pName);

    // Finish the Python Interpreter
    Py_Finalize();


	return true;
}

