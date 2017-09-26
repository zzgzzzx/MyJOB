#create by lewis
CROSS=
CXX=$(CROSS)g++
CC=$(CROSS)gcc
STRIP=$(CROSS)strip

RM= rm -f -r
CP= cp -f -r

BIN=SuperVPN
AR=ar

DFLAGS  = -w -Wall -DDEV_ENCRYPT -DDEBUG  -D_RAW -g -DNAT_PORT_RESERVED
RFLAGS  = -w -o2 -Wall -DDEV_ENCRYPT -D_RAW -DNAT_PORT_RESERVED

INCLUDE = -I../include -I/usr/include/curl -I/usr/local/ssl/include

CFLAGS  = $(DFLAGS)
CFLAGS += $(INCLUDE)

OBJS=BaseApp.o BThread.o cJSON.o HttpClient.o HttpGeneral.o HttpUserNode.o HttpSrvNode.o MainServer.o NDFunc.o NodeBase.o NodeGeneral.o SuperVPNApp.o TimerManager.o NodeUser.o NodeSrv.o

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
