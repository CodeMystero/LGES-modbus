#include "pch.h"
#include "modbusTCPnative.h"
#include <mutex>
#include <cstring>

namespace modbusTCP {

    ////////////////////////////////////////// Connection class methods
    Connection::Connection(const string& ip, int port)
        : ipAddress(ip), port(port), sock(INVALID_SOCKET) {
        WSADATA wsaData;
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (result != 0) {
            throw runtime_error("WSAStartup failed with error: " + to_string(result));
        }

        sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sock == INVALID_SOCKET) {
            WSACleanup();
            throw runtime_error("Socket creation failed with error: " + to_string(WSAGetLastError()));
        }

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        inet_pton(AF_INET, ipAddress.c_str(), &serverAddr.sin_addr);
    }

    void Connection::connect() {
        int result = ::connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
        if (result == SOCKET_ERROR) {
            closesocket(sock);
            WSACleanup();
            throw runtime_error("Connection to server failed with error: " + to_string(WSAGetLastError()));
        }
        cout << "Connected to " << ipAddress << " on port " << port << endl;
    }

    void Connection::disconnect() {
        if (sock != INVALID_SOCKET) {
            closesocket(sock);
            sock = INVALID_SOCKET;
            WSACleanup();
            cout << "Disconnected from " << ipAddress << endl;
        }
    }

    SOCKET Connection::getSocket() const {
        return sock;
    }

    void Connection::setIPAddress(const string& ip) {
        ipAddress = ip;
    }

    void Connection::setPort(int port) {
        this->port = port;
    }

    Connection::~Connection() {
        disconnect();
    }

    ////////////////////////////////////// DataQueue class methods
    //::DataQueue(size_t maxSize) : maxQueueSize(maxSize) {}

    DataQueue::DataQueue() {}

    DataQueue::~DataQueue() {
        while (!dataQueue.empty()) {
            delete[] dataQueue.front();
            dataQueue.pop();
            dataLengths.pop();
        }
    }

    void DataQueue::push(const uint8_t* data, int length) {
        lock_guard<mutex> lock(queueMutex);
        uint8_t* dataCopy = new uint8_t[length];
        std::memcpy(dataCopy, data, length);
        dataQueue.push(dataCopy);
        dataLengths.push(length);
    }

    uint8_t* DataQueue::pop(int& length) {
        std::lock_guard<std::mutex> lock(queueMutex);
        if (!dataQueue.empty()) {
            uint8_t* data = dataQueue.front();
            length = dataLengths.front();
            dataQueue.pop();
            dataLengths.pop();
            return data;
        }
        length = 0;
        return nullptr;
    }

    bool DataQueue::isEmpty() const {
        std::lock_guard<std::mutex> lock(queueMutex);
        return dataQueue.empty();
    }

    size_t DataQueue::size() const {
        std::lock_guard<std::mutex> lock(queueMutex);
        return dataQueue.size();
    }

    /*size_t DataQueue::getMaxQueueSize() const {
        return maxQueueSize;
    }

    void DataQueue::setMaxQueueSize(size_t maxSize) {
        maxQueueSize = maxSize;
    }*/

    ////////////////////////////////////////////// DataRetrieval class methods
    DataRetrieval::DataRetrieval(Connection& conn, DataQueue& dq) // conn 객체를 참고함으로 통신 에 필요한 정보를 얻을 수 있음
        : connection(conn), stopFlag(false), dataQueue(dq), transactionId(0x0000),
        functionCode(0x03), startAddress(htons(0x0000)), numRegisters(htons(0x0001)) {}     // dq객체를 참고함으로써 dataQueue스레드에서 dataRetriaval 쓰레드 객체 에 접근 가능 

    void DataRetrieval::retrieveData() {
        //uint16_t transactionId = 0x0000;                          // 2byte -> 서버에 의해 echo 됨,transactionId 초기화
        // Modbus TCP 요청 메시지 생성
        uint16_t protocolId = 0x0000;                       // 2byte -> client가 설정, 항상 = 00 00
        uint16_t length = htons(0x0006);                // 2byte -> UnitDI(1) + FC(1) + DATA 길이(4)   
        uint8_t unitId = 0xFF;                         // 1byte -> SlaveID (FF 고정)
        //uint8_t functionCode =          0x03;                         // 1byte -> Holding Register 읽기
        //uint16_t startAddress =         htons(0x0000);                // 2byte-> 0x0000
        //uint16_t numRegisters =         htons(0x0064);                // 2byte -> 0x016F 0x0000~0x016F 까지 
        // 레지스터당 2byte 총 184개
        uint8_t request[12]{};                                //이니셜라이즈 {} 왜 추가 ??
        uint16_t netTransactionId = htons(transactionId);
        memcpy(&request[0], &netTransactionId, sizeof(netTransactionId));   // transactionId 복사
        memcpy(&request[2], &protocolId, sizeof(protocolId));               // protocolId 복사
        memcpy(&request[4], &length, sizeof(length));                       // length 복사
        request[6] = unitId;                                                // unitId 설정
        request[7] = functionCode;                                          // functionCode 설정
        memcpy(&request[8], &startAddress, sizeof(startAddress));           // startAddress 복사
        memcpy(&request[10], &numRegisters, sizeof(numRegisters));          // numRegisters 복사

        char buffer[512];       // Autoway 레지스터 최대 크기 0x0000~0x016F 약 400bytes 넉넉하게 512bytes 할당

        while (!stopFlag) {

            int bytesSent = send(connection.getSocket(), reinterpret_cast<const char*>(request), sizeof(request), 0);
            if (bytesSent == SOCKET_ERROR) {
                std::cerr << "Send error: " << WSAGetLastError() << std::endl;
                stopFlag = true;
                continue;
            }

            int bytesRead = recv(connection.getSocket(), buffer, sizeof(buffer), 0);
            if (bytesRead > 0) {
                //std::cout << "Data received: ";
                for (int i = 0; i < bytesRead; ++i) {
                    std::cout << std::hex << static_cast<int>(buffer[i]) << " ";
                }
                //std::cout << std::endl;

                dataQueue.push(reinterpret_cast<uint8_t*>(buffer), bytesRead);
            }
            else if (bytesRead == SOCKET_ERROR) {
                std::cerr << "Read error: " << WSAGetLastError() << std::endl;
                stopFlag = true;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(510));
        }
    }

    void DataRetrieval::start() {
        stopFlag = false;
        retrieveThread = thread(&DataRetrieval::retrieveData, this);
    }

    void DataRetrieval::stop() {
        stopFlag = true;
        if (retrieveThread.joinable()) {
            retrieveThread.join();
        }
    }

    DataRetrieval::~DataRetrieval() {
        stop();
    }

    void DataRetrieval::setFunctionCode(uint8_t code) {
        functionCode = code;
    }

    void DataRetrieval::setStartAddress(uint16_t address) {
        startAddress = htons(address);
    }

    void DataRetrieval::setNumRegisters(uint16_t num) {
        numRegisters = htons(num);
    }
}