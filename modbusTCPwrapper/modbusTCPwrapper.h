#pragma once
#include "../modebusTCPnative/modbusTCPnative.h"

#ifdef _DEBUG
#pragma comment(lib,"C:/Users/seung/OneDrive/바탕 화면/LGES-Modbus/x64/Debug/modebusTCPnative.lib")
#else
#pragma comment(lib,"C:/Users/seung/OneDrive/바탕 화면/LGES-Modbus/x64/Release/modebusTCPnative.lib")
#endif
#include <msclr/marshal_cppstd.h>
#include <stdexcept>

using namespace System;

namespace wrapperModbusTCP {
	public ref class connectionWrapper {
	private:
		modbusTCP::Connection* connection;
	public:
		connectionWrapper(String^ ip, int port);
		~connectionWrapper();
		void connectWrapper();
		void disconectWrapper();
		void setIPAddressWrapper(const string& ip);
		void setPortWrapper(int port);
		//SOCKET getSocketWrapper() const;
		SOCKET getSocketWrapper();

	public:
		modbusTCP::Connection* getNativeConnection() { return connection; }
	};

	public ref class DataQueueWrapper {
	public:
		modbusTCP::DataQueue* dataqueue;
	public:
		DataQueueWrapper();
		~DataQueueWrapper();
		void pushWrapper(const uint8_t* data, int length);
		uint8_t* popWrapper(int% length);
		bool isEmptyWrapper();				//const
		size_t sizeWrapper();				//const

	public:
		modbusTCP::DataQueue* getNativeDataQueue() { return dataqueue; }
	};

	public ref class DataRetrievalWrapper {
	public:
		modbusTCP::DataRetrieval* dataretrieval;
	
	public:
		void retrieveDataWrapper();
		//DataRetrievalWrapper(modbusTCP::Connection& conn, modbusTCP::DataQueue& dq);
		DataRetrievalWrapper(connectionWrapper^ conn, DataQueueWrapper^ dq);
		~DataRetrievalWrapper();
		void startWrapper();
		void stopWrapper();
		void setFunctionCodeWrapper(uint8_t code);
		void setStartAddressWrapper(uint16_t address);
		void setNumRegistersWrapper(uint16_t num);
	};
	
}