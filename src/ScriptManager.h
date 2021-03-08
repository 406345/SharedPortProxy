#pragma once
#pragma comment(lib,"python3.lib")


#include <Python.h>
#include <memory>
#include <string>
#include <vector>

#include "Configuration.h"
#include "Filter.h"

using namespace std;

class ScriptManager
{
private:
	static shared_ptr<ScriptManager> _instance;
public:
	static shared_ptr<ScriptManager> get_instance();

	ScriptManager() {
	};

	~ScriptManager() {
		Py_Initialize();
	};

	void initialize() {
		Py_Initialize();

		this->load_all();
	};

	bool detect(unsigned char* buffer, int offset, int len) {
		for (auto mod : mods) {
			auto ret = this->call_function(mod, "detect", { this->to_py_list(buffer,offset,len)});
			auto value = PyLong_AsLong(ret);
			if (ret != nullptr) {
				return value > 0;
			}
		}

		return false;
	}
	
private:

	vector<shared_ptr<Filter>> filters;
	vector<PyObject*> mods;

	template<typename T>
	PyObject* to_py_list(T buffer, int offset, int size) {
		auto list = PyList_New(0);
		for (size_t i = offset; i < size; i++)
		{
			PyList_Append(list, Py_BuildValue("i", (T)buffer[i]));
		}
		return list;
	}

	inline PyObject* call_function(PyObject* mod, string name) {
		return this->call_function(mod, name, {});
	}

	PyObject* call_function(PyObject* mod, string name, vector<PyObject*> parameters) {
		auto pfunc = PyObject_GetAttrString(mod, name.c_str());

		if (pfunc == nullptr)return nullptr;

		int size = parameters.size();
		auto args = PyTuple_New(size);

		for (size_t i = 0; i < size; i++)
		{
			PyTuple_SetItem(args, i, parameters[i]);
		}

		return  PyObject_CallObject(pfunc, args);
	}

	inline void call_procedure(PyObject* mod, string name) {
		this->call_procedure(mod, name, {});
	}

	void call_procedure(PyObject* mod, string name, vector<PyObject*> parameters) {
		auto pfunc = PyObject_GetAttrString(mod, name.c_str());

		if (pfunc == nullptr)return;


		int size = parameters.size();
		auto args = PyTuple_New(size);

		for (size_t i = 0; i < size; i++)
		{
			PyTuple_SetItem(args, i, parameters[i]);
		}

		PyEval_CallObject(pfunc, args);
	}

	void load_all() {
		PyObject* obj;

		string chdir_cmd = string("sys.path.append('"+ Configuration::get_instance()->script_path()+"')\n");
		PyRun_SimpleString("import sys");
		PyRun_SimpleString(chdir_cmd.c_str());

		
		auto t = Configuration::get_instance()->get_all_mod();
		auto mod = PyImport_ImportModule("httpfilter");
		mods.push_back(mod);

		unsigned char buffer[128];
		this->call_procedure(mod, "hello");
		this->call_function(mod, "detect", { this->to_py_list(buffer, 0, 128) });
			//PyEval_CallObject(PyObject_GetAttrString(mod, "hello"), 0);
	}
};