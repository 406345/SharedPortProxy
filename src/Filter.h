#pragma once
#ifndef FILTER_HEADER_
#define FILTER_HEADER_

#include <string>
#include <Python.h>
#include <vector>

#define DETECT_METHOD_NAME "detect"

using namespace std;

class Filter
{
public:
	Filter(PyObject* mod, string mod_name, string forward);
	~Filter();

	unsigned long get_order() {
		return this->order;
	}
	bool check(unsigned char* buffer, int offset, int length) {
		auto result = PyLong_AsLong(this->call_function(this->mod, DETECT_METHOD_NAME, {
			this->to_py_list(buffer,0,length)
			}));

		return result > 0;
	};
	string get_forward_address() {
		return this->forward_addr;
	};

	int get_forward_port() {
		return this->forward_port;
	};

	string get_name() {
		return this->mod_name;
	};
private:
	unsigned long order = 0;
	PyObject* mod;
	string mod_name;
	string forward_addr = "";
	int forward_port = 0;

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

};

#endif // !FILTER_HEADER_

