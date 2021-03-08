#include "Configuration.h"

shared_ptr<Configuration> Configuration::_instance;

shared_ptr<Configuration> Configuration::get_instance()
{
	if (_instance == nullptr) {
		_instance = make_shared<Configuration>();
	}

	return _instance;
};