#pragma once

#include <iostream>
#include <string>
//#include <stdexcept>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <atomic>
#include <mutex>
#include <queue>
//#include <condition_variable>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

namespace modbusTCP {

    class Connection {
    private:
        SOCKET sock;
        string ipAddress;
        int port;
        struct sockaddr_in serverAddr;

    public:

        Connection(const string& ip, int port);             // Connection connection(ipAddress, Port)
        void connect();                                     // connection.connet()
        void disconnect();                                  // Disconnect from the server
        SOCKET getSocket() const;                           // Get socket
        void setIPAddress(const string& ip);
        void setPort(int port);
        ~Connection();                                      // Destructor
    };

    class DataQueue {
    private:
        /*queue<string> dataQueue;
        size_t maxQueueSize;
        mutable mutex queueMutex;*/

        queue<uint8_t*> dataQueue;
        queue<int> dataLengths; // 각 데이터의 길이를 저장하는 큐
        mutable mutex queueMutex;
        //size_t maxQueueSize;

    public:
        DataQueue();
        ~DataQueue();

        void push(const uint8_t* data, int length);
        uint8_t* pop(int& length);
        bool isEmpty() const;
        size_t size() const;
        /*      size_t getMaxQueueSize() const;
              void setMaxQueueSize(size_t maxSize);*/
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


    public:
        void retrieveData();
        DataRetrieval(Connection& conn, DataQueue& dq);             // Constructor
        void start();                                               // Start data retrieval
        void stop();                                                // Stop data retrieval
        void setFunctionCode(uint8_t code);
        void setStartAddress(uint16_t address);
        void setNumRegisters(uint16_t num);
        ~DataRetrieval();                                           // Destructor
    };
}

