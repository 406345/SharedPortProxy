#include <asio.hpp>
#include <memory>
#include <functional>

#include "ScriptManager.h"

using namespace asio;
using namespace std;

class TcpConnection
	: enable_shared_from_this<TcpConnection>
{
public:
	TcpConnection(io_context& context)
		: sck(context), context(context)
	{
		
	};
	~TcpConnection() {};

	void start() {
		this->sck.async_receive(
			asio::buffer(this->buffer, BUFFER_SIZE),
			bind(&TcpConnection::read_handler, this, std::placeholders::_1, std::placeholders::_2));
	};
	ip::tcp::socket& socket() {
		return this->sck;
	};

private:
	ip::tcp::socket sck;
	io_context& context;
	const int BUFFER_SIZE = 1024 * 5;
	unsigned char* buffer = new unsigned char[BUFFER_SIZE];

	void read_handler(const asio::error_code& error, size_t bytes_transferred) {

		if (error) {
			printf("error: %d message: %s\n", error.value(), error.message().data());
			return;
		}

		auto ret = ScriptManager::get_instance()->detect(this->buffer,0,bytes_transferred);

		if (ret) {
			printf("Is HTTP\n");
		}
		else {
			printf("Not HTTP\n");
		}
		//printf("received data: %d", bytes_transferred);

		//this->sck.async_receive(
		//	asio::buffer(this->buffer, BUFFER_SIZE),
		//	bind(&TcpConnection::read_handler, this, std::placeholders::_1, std::placeholders::_2));
	};
};
