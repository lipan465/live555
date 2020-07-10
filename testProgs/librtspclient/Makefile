
INCLUDES = -I../../UsageEnvironment/include -I../../groupsock/include -I../../liveMedia/include -I../../BasicUsageEnvironment/include
CC       = gcc 
CFLAGS   = -Wall -g -fPIC #需要加上 -fPIC
CXX      = g++
CXXFLAGS = -Wall -g -fPIC $(INCLUDES)
LINK     = g++

LIBS = ../../liveMedia/libliveMedia.a ../../groupsock/libgroupsock.a \
	../../BasicUsageEnvironment/libBasicUsageEnvironment.a ../../UsageEnvironment/libUsageEnvironment.a -lssl -lcrypto

TARGET   = librtspclient.so
OBJS     = DummySink.o ourRTSPClient.o rtspclient.o

# 链接用的,可以看到后面加了$(LIBS),因为只有链接时候才需要引用库文件
$(TARGET) : $(OBJS) 
	$(LINK) -shared -o $@ $^ $(LIBS)

# 编译cpp代码用这个目标
.cpp.o:
	$(CXX) -c $(CXXFLAGS) -fpermissive -o $@ $<

# 编译c代码用这个
.c.o:
	$(CC) -c $(CFLAGS) -fpermissive -o $@ $<

clean:
	-rm -rf *.o $(TARGET)