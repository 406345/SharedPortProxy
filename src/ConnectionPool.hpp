//#pragma once
//
//#include <vector>
//#include <unordered_set>
//#include <mutex>
//#include <memory>
//
//#include "../inc/EasyUsing.h"
//#include "TcpConnection.hpp"
//
//class ConnectionPool
//{
//	DEFINE_SINGLETON(ConnectionPool);
//
//public: 
//	ConnectionPool() {};
//	~ConnectionPool() {};
//
//	unordered_set<shared_ptr<TcpConnection>> cnn_collection;
//
//	void add(shared_ptr<TcpConnection> connection) {
//		locker.lock();
//		cnn_collection.insert(connection);
//		locker.unlock();
//	};
//
//	void remove(shared_ptr<TcpConnection> connection) {
//		locker.lock();
//		auto element_iter = cnn_collection.find(connection);
//
//		while (element_iter != cnn_collection.end()) {
//			cnn_collection.erase(element_iter++);
//		}
//
//		locker.unlock();
//	};
//private:
//	mutex locker; 
//};
