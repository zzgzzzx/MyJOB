#create by lewis
CROSS=
CXX=$(CROSS)g++
CC=$(CROSS)gcc
STRIP=$(CROSS)strip

RM= rm -f -r
CP= cp -f -r

BIN=SuperVPN
AR=ar

DFLAGS  = -w -Wall -DDEBUG  -D_RAW -g -DGENERAL_NODE_USER_APP
RFLAGS  = -w -o2 -Wall -D_RAW -DGENERAL_NODE_USER_APP

INCLUDE = -I../include -I../include/cJson -I/usr/include/curl

CFLAGS  = $(DFLAGS)
CFLAGS += $(INCLUDE)

OBJS=BaseApp.o BThread.o cJSON.o HttpClient.o HttpGeneral.o HttpGateway.o HttpUserNode.o HttpSrvNode.o MainServer.o NDFunc.o NodeBase.o NodeGateway.o NodeGeneral.o SuperVPNApp.o TimerManager.o NodeUser.o NodeSrv.o HttpFileDown.o Md5.o HttpRunEnvCK.o HttpRunEnvCKUser.o HttpRunEnvCKSrv.o

ALL:$(BIN)

$(BIN): $(OBJS)
	$(CXX) $(OBJS) -o $(BIN) -lpthread -lcurl
	$(STRIP) $(BIN)

.c.o:
	$(CC) -c $(CFLAGS) $^ -o $@
.cpp.o:
	$(CXX) -c $(CFLAGS) $^ -o $@


clean:
	$(RM) *.o *.log core* $(BIN)
