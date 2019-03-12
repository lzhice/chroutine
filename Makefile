CPP	= @echo " g++ $@"; g++
CC	= @echo " gcc $@"; gcc
LD	= @echo " ld  $@"; ld
AR	= @echo " ar  $@"; ar
RM	= @echo " RM	$@"; rm -f

CFLAGS += -Wall -std=c++11
CFLAGS += -g3

#LDFLAGS += "-Wl" -lpthread -lc
LDFLAGS += -L. -lpthread  ##-lenc -lsyslink  -lbinder -llog    -losa -lpdi -lsdk  -lz -lrt -lsplice -lmem -ldec
AFLAGS += -r

INC = -I./engin #-I./rpc

BINDIR = .


SRCS_PATH = . ./engin #./rpc ./proto_code/grpc_code ./proto_code/msg_code

LIB_SRCS += $(foreach dir,$(SRCS_PATH),$(wildcard $(dir)/*.cpp))

LIB_OBJS += $(patsubst %.cpp,%.o,$(LIB_SRCS))

LIB_APP = test.a

DEMO_NAME	= test
TARGET_DEMO = $(BINDIR)/$(DEMO_NAME)

all: $(TARGET_DEMO)

$(TARGET_DEMO) : $(LIB_APP)
	$(CPP) -o $@ $(LDFLAGS) $(INC) $^

$(LIB_APP): $(LIB_OBJS)
	$(RM) $@;
	$(AR) $(AFLAGS) $@ $^

.c.o:
	$(CC) -c $(CFLAGS) $(INC) $^ -o $@

.cpp.o:
	$(CPP) -c $(CFLAGS) $(INC) $^ -o $@

clean:
	$(RM) $(LIB_OBJS) $(LIB_APP) $(TARGET_DEMO)

cleanobj:
	$(RM) $(LIB_OBJS)
