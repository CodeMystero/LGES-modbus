#pragma once

#include <iostream>

#include <WinSock2.h>
#include <WS2tcpip.h>

#include <mutex>
#include <thread>
#include <queue>

#include <string>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

namespace modbucTCP {

	class Connection {
	private:
		SOCKET sock;
		string ipAddress;
		int port;
		struct sockaddr_in serverAddr;

	public:
		Connection(const string& ip, int port);
		void connect();
		void disconnection();
		SOCKET getSocket() const;
		void setIPAddress(const string& ip);
		void setPort(int port);
		~Connection();
	};

	class DataQueue {
	private:
		queue<uint8_t*> dataQueue;
		queue<int> dataLengths;
		mutable mutex queueMutex;

	public:
		DataQueue();
		~DataQueue();
		void push(const uint8_t* data, int length);
		uint8_t* pop(int& length);
		bool isEmpty() const;
		size_t size() const;
	};

	class DataRetrieval {
	private:
		Connection& connection;
		thread retrieveThread;
		atomic<bool> stopFlag;
		DataQueue& dataQueue;
		uint16_t transactionId;
		uint8_t functionCode;
		uint16_t startAddress;
		uint16_t numRegisters;

	private:
		void retrieveData();

	public:
		DataRetrieval(Connection& conn, DataQueue& dq);
		~DataRetrieval();
		void start();
		void stop();
		void setFunctionCode(uint8_t code);
		void setStartAddress(uint16_t address);
		void setNumRegisters(uint16_t num);
	};
}