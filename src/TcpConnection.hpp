#include <asio.hpp>
#include <memory>
#include <functional>
#include <signal.h>

#include "ScriptManager.h"
#include "Filter.h"
#include "Semaphore.h"

using namespace asio;
using namespace std;

class TcpConnection
	: enable_shared_from_this<TcpConnection>
{
public:
	TcpConnection(io_context& context)
		: sck(context), context(context), forward_sck(context),
		connect_signal(1), forward_signal(1)
	{
	};
	~TcpConnection() {};

	void start() {
		this->start_receive();
	}
	
	ip::tcp::socket& socket() {
		return this->sck;
	};

private:
	ip::tcp::socket sck;
	ip::tcp::socket forward_sck;
	io_context& context;
	const int BUFFER_SIZE = 1024 * 5;
	unsigned char* buffer = new unsigned char[BUFFER_SIZE];
	unsigned char* buffer_foreard = new unsigned char[BUFFER_SIZE];
	shared_ptr<Filter> filter = nullptr;
	Semaphore connect_signal;
	Semaphore forward_signal;
	
	void start_receive() {
		this->sck.async_receive(
			asio::buffer(this->buffer, BUFFER_SIZE),
			bind(&TcpConnection::read_handler, this, std::placeholders::_1, std::placeholders::_2));
	};

	void read_handler(const asio::error_code& error, size_t bytes_transferred) {
		if (error) {
			cout << "Error " << error.value() << ": " << error.message() << endl;
			return;
		}

		cout << "received data " << bytes_transferred << " bytes" << endl;

		if (filter == nullptr) {
			filter = ScriptManager::get_instance()->filter(this->buffer, 0, bytes_transferred);
			
			if (filter != nullptr) { 

				cout << "using filter " << filter->get_name() << endl;

				this->start_forward();
				this->forward_start_read();
				this->forward_data(this->buffer, 0, bytes_transferred);
			}
		} 
		else {
			this->forward_data(this->buffer, 0, bytes_transferred);
		} 

		this->start_receive();
	};

	void forward_data(unsigned char * buffer,int offset,int len) {


		try {
			this->forward_sck.send(asio::buffer(
				(void*)buffer,
				len
			));

			cout << "forward data " << len << " bytes" << endl;
		}
		catch (asio::system_error error) {
			cout << "Error " << error.code() << ": " << error.what() << endl;

		}
	}

	void forward_start_read() {
		this->forward_sck.async_receive(
			asio::buffer(this->buffer_foreard, BUFFER_SIZE),
			[this](const asio::error_code& error, size_t bytes_transferred) {

			if (error) {
				cout << "Error " << error.value() << ": " << error.message().c_str() << endl;
				return;
			}
			try {
				this->socket().send(asio::buffer(this->buffer_foreard, bytes_transferred));
				this->forward_start_read();
			}
			catch (asio::system_error er) {
				this->socket().close();
				cout << "Forward connection error " << er.code() << " : " << er.what() << endl;
			}
		});
	}

	void start_forward() {
		if (filter == nullptr && this->filter->get_forward_port() > 0)return;

		asio::ip::tcp::resolver resolver(this->context);
		asio::error_code ec;

		string port_str = "99999";
		_itoa(this->filter->get_forward_port(), (char*)port_str.data(), 10);
		asio::ip::tcp::resolver::results_type results = resolver.resolve(
			this->filter->get_forward_address(), 
			port_str,
			ec);

		if (ec) {
			this->socket().close();
			cout << "Error " << ec.value() << ": " << ec.message() << endl;
			return;
		}

		try {
			forward_sck.connect(results->endpoint());
		}
		catch (asio::system_error er) {
			cout << "Forward connection error " << er.code() << " : " << er.what() << endl;

		}
		 
	}
};
