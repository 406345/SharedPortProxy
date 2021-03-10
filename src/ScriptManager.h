#pragma once
#pragma comment(lib,"python3.lib")


#include <Python.h>
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

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

	shared_ptr<Filter> filter(unsigned char* buffer, int offset, int len) {
		for (auto& f : filters) {
			if (f->check(buffer, offset, len)) {
				return f;
			}
		}

		return nullptr;
	};
private:

	vector<shared_ptr<Filter>> filters;
	vector<PyObject*> mods;

	void load_all() {

		this->filters.clear();

		PyObject* obj;

		string chdir_cmd = string("sys.path.append('" + Configuration::get_instance()->script_path() + "')\n");
		PyRun_SimpleString("import sys");
		PyRun_SimpleString(chdir_cmd.c_str());

		auto t = Configuration::get_instance()->get_all_mod();

		for (auto& kv : t) {

			auto mod = PyImport_ImportModule(string(kv.first).c_str());
			this->filters.push_back(make_shared<Filter>(mod, kv.first, kv.second));
			cout << "load module " << kv.first << " successfully" << endl;
			//mods.push_back(mod);
		}

		sort(filters.begin(), filters.end(), [](const shared_ptr< Filter> p1, const shared_ptr< Filter> p2) {
			return p1->get_order() > p2->get_order();
		});
	}
};