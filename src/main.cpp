#include "SocketWrapper.hpp"
#include "ScriptManager.h"
#include "Configuration.h"

using namespace std;

int main(int argc,char* argv[]) {

	Configuration::get_instance()->load(string(argv[1]));
	ScriptManager::get_instance()->initialize();
	SocketWrapper socket;
	socket.run();
	return 0;
}