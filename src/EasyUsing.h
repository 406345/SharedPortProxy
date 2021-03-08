#pragma once
#include <memory>
using namespace std;
#define DEFINE_SINGLETON(T)	public: static shared_ptr<T> get_instance(){\
								if( _instance == nullptr) _instance = make_shared<T>();\
								return _instance; \
							}\
