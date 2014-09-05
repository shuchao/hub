CC = gcc
CX = g++
#C_ARG = -g --verbose -DMUDUO_STD_STRING -DCHECK_PTHREAD_RETURN_VALUE -D_FILE_OFFSET_BITS=64 -Wall -Wextra -Wconversion -Wno-unused-parameter -Woverloaded-virtual -Wpointer-arith -Wwrite-strings -march=nocona -rdynamic -O0 -gdwarf-2
C_ARG = -g -DCHECK_PTHREAD_RETURN_VALUE -D_FILE_OFFSET_BITS -Wall -Wextra -Werror -Wconversion -Wno-unused-parameter -Wold-style-cast -Woverloaded-virtual -Wpointer-arith -Wshadow -Wwrite-strings -march=native -rdynamic
BIN = hub
#TOOL = sendTool tcpClientTest loopLevelDB

ZKNAME_INC= -I/usr/include
ZKNAME_LIB= /usr/local/zk_agent/lib/libnameapi_64.so.1.0

GOOGLE_INC= -I/usr/local/include/google/protobuf
GOOGLE_LIB= /data/webroot/chaoshu/tool_lib/protobuf-2.5.0/src/.libs/libprotobuf.a

MYSQL_INC = -I/data/webroot/chaoshu/tool_lib/mysql/include
MYSQL_LIB = /data/webroot/chaoshu/tool_lib/mysql/lib/libmysqlclient.a

BOOST_INC = -I/data/webroot/chaoshu/lib/boost_1_56_0
BOOST_LIB = -L/data/webroot/chaoshu/lib/boost_1_56_0/stage/lib

LEVELDB_INC = -I/data/webroot/chaoshu/tool_lib/leveldb-1.9.0/include
LEVELDB_LIB = /data/webroot/chaoshu/tool_lib/leveldb-1.9.0/libleveldb.a

MUDUO_INC = -I/data/webroot/chaoshu/lib/muduo_1.0.3/build/debug-install/include  
MUDUO_LIB = /data/webroot/chaoshu/lib/muduo_1.0.3/build/debug-install/lib/libmuduo_net.a  /data/webroot/chaoshu/tool_lib/build/debug-install/lib/libmuduo_base.a

REDIS_INC = -I/data/webroot/chaoshu/tool_lib/hiredis-master
REDIS_LIB = /data/webroot/chaoshu/tool_lib/hiredis-master/libhiredis.a

COMM_INC = -I/data/webroot/chaoshu/tool_lib/commlib64/baselib
COMM_LIB = /data/webroot/chaoshu/tool_lib/commlib64/baselib/libcomm_oi.a

PROTO_INC = -I/data/webdev/chaoshu/hub/proto
PROTO_LIB = /data/webdev/chaoshu/hub/proto/libhubProto.a

#TOTAL_INC = -I/data/webroot/chaoshu/hub/src $(GOOGLE_INC) $(MYSQL_INC) $(BOOST_INC) $(MUDUO_INC) $(COMM_INC) $(REDIS_INC) $(PROTO_INC) $(LEVELDB_INC) $(ZKNAME_INC)
TOTAL_INC = -I/data/webroot/chaoshu/github/work/hub/src $(BOOST_INC) $(MUDUO_INC)
#TOTAL_DYNAMIC_LIB = -Wl,-dy $(ZKNAME_LIB)
TOTAL_STATIC_LIB = -L/data/webroot/chaoshu/github/work/hub/src -lpthread -lz -lrt $(BOOST_LIB) $(MUDUO_LIB)

.cpp.o:
	$(CX) $(C_ARG) -c $^ $(TOTAL_INC)
.c.o:
	$(CC) $(C_ARG) -c $^ $(TOTAL_INC)
.cc.o:
	$(CX) $(C_ARG) -c $^ $(TOTAL_INC)
.o:
	$(CX) $(C_ARG) -o $@ $^ $(TOTAL_DYNAMIC_LIB) -Xlinker "-(" $(TOTAL_STATIC_LIB) -Xlinker "-)"
	#$(CX) $(C_ARG) -o $@ $^ -Xlinker "-(" $(TOTAL_STATIC_LIB) -Xlinker "-)"

all: $(BIN) $(TOOL)

install:
	ln -snf /data/webdev/chaoshu/hub/src/hub /data/webdev/chaoshu/hub/bin/hub
	ln -snf /data/webdev/chaoshu/hub/src/sendTool /data/webdev/chaoshu/hub/bin/sendTool

hub: hub.o common.o configCenter.o monitorThread.o tlib_db.o localCfgFile.o handle_config_file.o

clean:
	rm -f $(BIN) $(TOOL) *.o
