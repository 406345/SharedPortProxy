#include <asio.hpp>
#include <functional>
#include <vector>

#include "TcpConnection.hpp"
#include "ConnectionPool.hpp"

using namespace asio;
using namespace std;

class SocketWrapper
{
private:
	asio::io_context context;
	shared_ptr< ip::tcp::acceptor> acceptor_;
	vector<shared_ptr<TcpConnection>> connections;

	void start_accept()
	{
		shared_ptr<TcpConnection> new_connection = make_shared<TcpConnection>(this->context);

		this->acceptor_->async_accept(
			new_connection->socket(),
			bind(&SocketWrapper::handle_accept, this, new_connection, std::placeholders::_1));
	}

	void handle_accept(
		shared_ptr<TcpConnection> new_connection,
		const asio::error_code& error)
	{
		if (!error)
		{
			connections.push_back(new_connection);
			//ConnectionPool::get_instance()->add(new_connection);
			new_connection->start();
		}

		start_accept();
	}
public:
	SocketWrapper()
	{
		this->acceptor_ = make_shared<ip::tcp::acceptor>(context, ip::tcp::endpoint(ip::tcp::v4(), 9088));
		this->start_accept();
	};

	~SocketWrapper() {

	};

	void run() {
		this->context.run_one();
	}
};
