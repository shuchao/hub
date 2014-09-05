#include "common.h"

boost::scoped_ptr<muduo::AsyncLogging> g_asyncLog;
boost::shared_ptr<muduo::CountDownLatch> g_spCountDownLatch;

class LogSvrConfig;
LogSvrConfig g_logSvrConfigObj;

class LocalCfgFile;
LocalCfgFile g_locCfgFile;

class MonitorData;
muduo::BlockingQueue<MonitorData> g_monitorBQ;          //ONLY support increment NOW, fix me

muduo::BlockingQueue<MonitorData> g_failParseBQ;  

bool g_exit = false;            //every thread check it when finish critical task

void MyAsyncOutput(const char* msg, int len)
{
		g_asyncLog->append(msg, len);
}




