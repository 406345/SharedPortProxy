#include "Configuration.h"

shared_ptr<Configuration> Configuration::_instance = nullptr;

shared_ptr<Configuration> Configuration::get_instance()
{
	if (_instance == nullptr) {
		_instance = make_shared<Configuration>();
	}

	return _instance;
}
Configuration::Configuration()
{
	_instance = nullptr;
}
Configuration::~Configuration()
{
}
;