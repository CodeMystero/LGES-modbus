#include "pch.h"
#include "modbusTCPwrapper.h"

wrapperModbusTCP::connectionWrapper::connectionWrapper(String^ ip, int port)
{
	std::string nativeIp = msclr::interop::marshal_as<std::string>(ip);
	connection = new modbusTCP::Connection(nativeIp, port);
}

wrapperModbusTCP::connectionWrapper::~connectionWrapper()
{
	delete connection;
	connection = nullptr;
}

void wrapperModbusTCP::connectionWrapper::connectWrapper()
{
	return connection->connect();
}

void wrapperModbusTCP::connectionWrapper::disconectWrapper()
{
	return connection->disconnect();
}

void wrapperModbusTCP::connectionWrapper::setIPAddressWrapper(const string& ip)
{
	return connection->setIPAddress(ip);
}

void wrapperModbusTCP::connectionWrapper::setPortWrapper(int port)
{
	return connection->setPort(port);
}

SOCKET wrapperModbusTCP::connectionWrapper::getSocketWrapper()
{
	return connection->getSocket();
}

wrapperModbusTCP::DataQueueWrapper::DataQueueWrapper()
{
	dataqueue = new modbusTCP::DataQueue();
}

wrapperModbusTCP::DataQueueWrapper::~DataQueueWrapper()
{
	delete dataqueue;
	dataqueue = nullptr;
}

void wrapperModbusTCP::DataQueueWrapper::pushWrapper(const uint8_t* data, int length)
{
	return dataqueue->push(data, length);
}

//uint8_t* wrapperModbusTCP::DataQueueWrapper::popWrapper(int& length)
//{
//	return dataqueue->pop(length);
//}

uint8_t* wrapperModbusTCP::DataQueueWrapper::popWrapper(int% length)
{
	int len = 0;
	uint8_t* data = dataqueue->pop(len);
	length = len;
	if (data != nullptr) {
		uint8_t* managedData = new uint8_t[length];
		std::copy(data, data + length, managedData);
		delete[] data;
		return managedData;
	}
	return nullptr;
}

bool wrapperModbusTCP::DataQueueWrapper::isEmptyWrapper()
{
	return dataqueue->isEmpty();
}

size_t wrapperModbusTCP::DataQueueWrapper::sizeWrapper()
{
	return dataqueue->size();
}

//wrapperModbusTCP::DataRetrievalWrapper::DataRetrievalWrapper(modbusTCP::Connection & conn, modbusTCP::DataQueue& dq)
//{
//	dataretrieval = new modbusTCP::DataRetrieval(conn, dq);
//}

wrapperModbusTCP::DataRetrievalWrapper::DataRetrievalWrapper(connectionWrapper^ conn, DataQueueWrapper^ dq)
{
	dataretrieval = new modbusTCP::DataRetrieval(*(conn->getNativeConnection()), *(dq->getNativeDataQueue()));
}

wrapperModbusTCP::DataRetrievalWrapper::~DataRetrievalWrapper()
{
	delete dataretrieval;
	dataretrieval = nullptr;
}

void wrapperModbusTCP::DataRetrievalWrapper::retrieveDataWrapper()
{
	return dataretrieval->retrieveData();
}

void wrapperModbusTCP::DataRetrievalWrapper::startWrapper()
{
	return dataretrieval->start();
}

void wrapperModbusTCP::DataRetrievalWrapper::stopWrapper()
{
	return dataretrieval->stop();
}

void wrapperModbusTCP::DataRetrievalWrapper::setFunctionCodeWrapper(uint8_t code)
{
	return dataretrieval->setFunctionCode(code);
}

void wrapperModbusTCP::DataRetrievalWrapper::setStartAddressWrapper(uint16_t address)
{
	return dataretrieval->setStartAddress(address);
}

void wrapperModbusTCP::DataRetrievalWrapper::setNumRegistersWrapper(uint16_t num)
{
	return dataretrieval->setNumRegisters(num);
}


