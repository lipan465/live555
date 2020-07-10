
#include <thread>
#include <string>
#include "ourRTSPClient.h"

class MyRstpClient {
public:
    MyRstpClient();
    ~MyRstpClient();
    int connect(const std::string& url);
private:
    std::thread _event_loop;
    volatile char _b_quit;
};

MyRstpClient::MyRstpClient() {

}

MyRstpClient::~MyRstpClient() {

}

int MyRstpClient::connect(const std::string& url) {
    return 0;
}
