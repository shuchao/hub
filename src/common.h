#ifndef INTSVR_SRC_COMMON_H_
#define INTSVR_SRC_COMMON_H_

//C header	order by dict
#ifdef __cplusplus
extern"C"
{
#endif

#include <arpa/inet.h>
#include <assert.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/ipc.h>
#include <sys/select.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <search.h>
#include <time.h>
#include <unistd.h>

#ifdef __cplusplus
}
#endif


//C++ header
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <tr1/memory>
#include <vector>

#ifdef __cplusplus
extern"C"
{
#endif

#include "oi_str2.h"

#ifdef __cplusplus
}
#endif

#include <boost/bind.hpp>
#include <google/protobuf/message.h>

//muduo
#include <muduo/base/AsyncLogging.h>
#include <muduo/base/BlockingQueue.h>
#include <muduo/base/CountDownLatch.h>
#include <muduo/base/LogFile.h>
#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/EventLoopThread.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpServer.h>

#include "mblog_ad_image.pb.h"
#include "monitorData.h"

using namespace std;



#define RET_LN() {return -__LINE__;}

typedef boost::shared_ptr<google::protobuf::Message> MessagePtr;
typedef boost::shared_ptr<muduo::net::TcpClient> TcpClientPtr;

typedef boost::shared_ptr<muduo::net::TcpConnection> TcpConnectionPtr;
typedef set<TcpConnectionPtr> ConnectionList;

typedef boost::weak_ptr<muduo::net::TcpConnection> TcpConnWeakPtr;
typedef set<TcpConnWeakPtr> TcpConnWeakPtrList;
typedef set<TcpConnWeakPtr>::iterator TcpConnWeakPtrListIter;

enum errorType{
		PKG_INVALID = -1,
		ADD_TIMER_FAILED = -2,
		PARSE_ERR = -3,
		INCLUDE_ERR = 1,	//partial or all error for req, including oitimer timeout case
	};
	
enum cmdType{
	TEG_PROF = 1,
	TEG_INTEREST = 2,
	OMG_INTEREST = 3,
	QQGROUP_INTEREST = 4
};

enum WorkerThreadRetType{
	TEG_PROF_RET = 1,
	TEG_INTEREST_RET = 2,
	OMG_INTEREST_RET = 3,
	QQGROUP_INTEREST_RET = 4,
	ERR_RET = 5,
};



enum ATTR_ID_ENUM{
	ATTR_TEG_PROF_OITIMER_EXPIRE = 212340,
	ATTR_TEG_INTER_OITIMER_EXPIRE = 212341,
	ATTR_OMG_INTER_OITIMER_EXPIRE = 212342,
	ATTR_PB_PARSE_FAILED = 212343,
	ATTR_RECV_FAILED = 212344,
	ATTR_RECV_PKG_ERR = 212345,
	ATTR_ADD_TIMER_FAILED = 212346,
	ATTR_RECV_TEG_PROF_REQ = 212347,
	ATTR_RECV_TEG_INTEREST_REQ = 212348,
	ATTR_RECV_OMG_INTEREST_REQ = 212349,
	ATTR_RECV_ALL_REQ = 212350,
	ATTR_SEND_TEG = 212351,
	ATTR_PACK_PB_FAILED = 212352,
	ATTR_RSP = 212353,
	ATTR_RSP_FAILED = 212354,
	ATTR_DB_MAP_INFO_FAILED = 212355,
	ATTR_RECV_TEG_RSP = 212356,
	ATTR_RECV_TEG_RSP_CONT_ERR = 212357,
	ATTR_CACHE_CONT_ERR = 212358,
	
	ATTR_CACHE_HIT_NUM = 212359,
	ATTR_DB_SVR_INFO_FAILED = 209268,

	ATTR_RECV_TEG_PROF_RES = 212360,
	ATTR_RECV_TEG_PROF_ERR = 212361,
	ATTR_RECV_TEG_INTEREST_RES = 212362,
	ATTR_RECV_TEG_INTEREST_ERR = 212363,

	ATTR_TEG_PROF_CACHE_EXPIRE = 212364,
	ATTR_TEG_INTEREST_CHCHE_EXPIRE = 212365,
	ATTR_OITIMER_MISSED = 212366,
	ATTR_OITIMER_FAILED = 212367,
	ATTR_RECV_RENEW_PROF_INFO = 212368,
	ATTR_RECV_RENEW_INTEREST_INFO = 212369,

	ATTR_PARSE_TEG_PROF = 212370,
	ATTR_PARSE_TEG_INTEREST = 212371,
	ATTR_PARSE_OMG_INTEREST = 212372,
	ATTR_PARSE_QQGROUP_INTEREST = 217496,

	ATTR_REDIS_CONN_FAILED = 212374,
	
	ATTR_TEG_PROF_PARSE_FAILED = 212375,
	ATTR_TEG_INTER_PARSE_FAILED = 212376,
	ATTR_OMG_INTER_PARSE_FAILED = 212377,
	ATTR_QQGROUP_INTER_PARSE_FAILED = 217497,

	ATTR_OMG_INTER_MISS = 212373,
	ATTR_QQGROUP_INTER_MISS = 217498,
	ATTR_MAP_CONTEXT_MISS = 212926, 
	ATTR_TEG_DATA_MISS = 212927,
	ATTR_TEG_DATA_MISS_TIMEOUT = 212928,
	ATTR_TEG_INTEREST_NULL = 213972,

	ATTR_DM_USER_INFO_FETCH = 215073,
	ATTR_DM_USER_INFO_FETCH_ERROR = 215090,
	ATTR_DM_USER_INFO_FETCH_MISS = 215074,
	ATTR_USER_INTEREST_DEFAULT = 215227,
	ATTR_DB_TAG_TOP_RATE_FAILED = 215272,
	ATTR_WB_USER_REG_INFO_FAILED = 215418,
  	ATTR_WB_USER_REG_INFO_MISS = 215419,
  	ATTR_WB_USER_REG_INFO = 215417,
  	ATTR_USER_PROF_RET_MISS = 216294
};

enum REQ_TYPE{
	REQ_PROF_1 = 10001,
	REQ_PROF_2 = 10004,	
	REQ_TEG_INTEREST = 10005,	
	REQ_NO_USER_INFO = 20007,
};

#endif

