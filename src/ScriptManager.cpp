#include "ScriptManager.h"
shared_ptr<ScriptManager> ScriptManager::_instance;

shared_ptr<ScriptManager> ScriptManager::get_instance()
{
	if (_instance == nullptr) {
		_instance = make_shared<ScriptManager>();
	}

	return _instance;
}