#pragma once
#ifndef FILTER_HEADER_
#define FILTER_HEADER_

#include <string>
#include <Python.h>

using namespace std;

class Filter
{
public:
	Filter(PyObject* mod, string forward);
	~Filter();

private:

};

#endif // !FILTER_HEADER_

