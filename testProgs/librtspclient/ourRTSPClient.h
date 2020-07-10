#pragma once
#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"

class StreamClientState {
public:
  StreamClientState();
  virtual ~StreamClientState();

public:
  MediaSubsessionIterator* iter;
  MediaSession* session;
  MediaSubsession* subsession;
  TaskToken streamTimerTask;
  double duration;
};


// If you're streaming just a single stream (i.e., just from a single URL, once), then you can define and use just a single
// "StreamClientState" structure, as a global variable in your application.  However, because - in this demo application - we're
// showing how to play multiple streams, concurrently, we can't do that.  Instead, we have to have a separate "StreamClientState"
// structure for each "RTSPClient".  To do this, we subclass "RTSPClient", and add a "StreamClientState" field to the subclass:

class ourRTSPClient: public RTSPClient {
public:
  static ourRTSPClient* createNew(UsageEnvironment& env, char const* rtspURL,
				  int verbosityLevel = 0,
				  char const* applicationName = NULL,
				  portNumBits tunnelOverHTTPPortNum = 0);

    #define RTSP_CLIENT_VERBOSITY_LEVEL 1

    void openURL(UsageEnvironment& env, char const* progName, char const* rtspURL) {
        // Begin by creating a "RTSPClient" object.  Note that there is a separate "RTSPClient" object for each stream that we wish
        // to receive (even if more than stream uses the same "rtsp://" URL).
        RTSPClient* rtspClient = createNew(env, rtspURL, RTSP_CLIENT_VERBOSITY_LEVEL, progName);
        if (rtspClient == NULL) {
            env << "Failed to create a RTSP client for URL \"" << rtspURL << "\": " << env.getResultMsg() << "\n";
            return;
        }

        // Next, send a RTSP "DESCRIBE" command, to get a SDP description for the stream.
        // Note that this command - like all RTSP commands - is sent asynchronously; we do not block, waiting for a response.
        // Instead, the following function call returns immediately, and we handle the RTSP response later, from within the event loop:
        rtspClient->sendDescribeCommand(continueAfterDESCRIBE); 
    }

    
protected:
    static void continueAfterDESCRIBE(RTSPClient* rtspClient, int resultCode, char* resultString);
    static void setupNextSubsession(RTSPClient* rtspClient);
    static void continueAfterSETUP(RTSPClient* rtspClient, int resultCode, char* resultString);
    static void continueAfterPLAY(RTSPClient* rtspClient, int resultCode, char* resultString);
    static void subsessionAfterPlaying(void* clientData);
    static void subsessionByeHandler(void* clientData, char const* reason);
    static void streamTimerHandler(void* clientData);
    static void shutdownStream(RTSPClient* rtspClient, int exitCode = 1);
    static int rtspClientCount;

    ourRTSPClient(UsageEnvironment& env, char const* rtspURL, int verbosityLevel, char const* applicationName, portNumBits tunnelOverHTTPPortNum);
    // called only by createNew();
    virtual ~ourRTSPClient();

public:
    StreamClientState scs;
};
