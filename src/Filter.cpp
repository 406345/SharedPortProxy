#include "Filter.h"

Filter::Filter(PyObject* mod, string mod_name, string forward)
{
	this->mod = mod;
	this->mod_name = mod_name;

	auto t = PyLong_AsLong(call_function(this->mod, "order"));
	if (t < 0) t = 0;
	this->order = t;


	for (size_t i = 0; i < forward.size(); i++)
	{
		if (forward[i] == ':') {
			this->forward_addr = forward.substr(0, i);
			this->forward_port = atoi(forward.substr(i+1, forward.size()).c_str());
			break;
		}
	}
}

Filter::~Filter()
{
}
