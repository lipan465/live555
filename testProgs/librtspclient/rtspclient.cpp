
#include <thread>
#include <string>
#include <map>
#include <mutex>
#include "ourRTSPClient.h"

//typedef void (*RtspFrameCallBack)();
static TaskScheduler* scheduler = BasicTaskScheduler::createNew();
static UsageEnvironment* env = BasicUsageEnvironment::createNew(*scheduler);
static volatile char _b_quit = 0;
static std::map<ourRTSPClient*, RtspFrameCallBack> cb_list;
static std::mutex _lock;

void OnNewFrame(void* user_ptr, unsigned frameSize, u_int8_t* data) {
    //std::lock_guard<std::mutex> lock(_lock);
    _lock.lock();
    auto it = cb_list.find((ourRTSPClient*)user_ptr);
    _lock.unlock();
    if(it != cb_list.end()) {
        it->second(user_ptr, frameSize, data);
    }
}

void* connect(char const* progName, char const* rtspURL, RtspFrameCallBack cb, void* user_ptr) {

    ourRTSPClient* cli = ourRTSPClient::openURL(*env, progName, rtspURL);
    if(cli == nullptr) return nullptr;
    cli->SetFrameCallback(OnNewFrame, user_ptr);
    std::lock_guard<std::mutex> lock(_lock);
    cb_list[cli] = cb;

    return cli;
}

void DoEventLoop() {
    env->taskScheduler().doEventLoop(&_b_quit);
}

