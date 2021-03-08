#pragma once

#include <string>
#include <memory>
#include <unordered_map>

#include "../3rd/rapidjson/document.h"

using namespace std;
using namespace rapidjson;

class Configuration
{
private:
	
public:
	static shared_ptr<Configuration> _instance;
	static shared_ptr<Configuration> get_instance();
	
	Configuration() {};
	~Configuration() {};

	void initialize() {

	};

	bool load(string path) { 
		FILE* file = fopen(path.data(), "r");
		fseek(file, 0, SEEK_END);
		auto length = ftell(file);
		fseek(file, 0, SEEK_SET);

		json_buffer = new char[length ];
		fread(json_buffer, 1, length, file);
		fclose(file);

		json_doc.Parse(json_buffer,length);

		return true;
	};

	string script_path() {

		auto& node = json_doc["script"];
		return string(node["path"].GetString());
	}

	unordered_map<string, string> get_all_mod() {
		unordered_map<string, string> ret;

		auto& module_node = json_doc["script"]["module"];
		if (module_node.Empty())
			return std::move(ret);

		auto t = module_node.GetArray();


		return std::move(ret);
		//return string(json_doc["script"]["module"][mod.data()].GetString());
	}


	string module_find(string mod) {
		if (json_doc["script"]["module"].Empty())
			return "";

		if (json_doc["script"]["module"][mod.data()].Empty()) {
			return "";
		}

		return string(json_doc["script"]["module"][mod.data()].GetString());
	}
private:
	Document json_doc;
	char* json_buffer = nullptr;
	Value& root;

};
