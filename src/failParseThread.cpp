#include "failParseThread.h"

extern muduo::BlockingQueue<MonitorData> g_monitorBQ;		//ONLY support increment NOW, fix me
extern bool g_exit;		//every thread check it when finish critical task

void MonitorThread::Run(void)
{
	int iRet = 0;
	while(1){
		MonitorData monitorFileDataObj = g_monitorBQ.take();
		//LOG_DEBUG << " MonitorThread get data left size=" << g_monitorBQ.size();	//size may NOT be the one just after this thread take!!!

		if(g_exit){
                        LOG_WARN << muduo::CurrentThread::name() << " monitor thread break according to g_exit";
                        break;
                }

		//add a stop way?? fix me
		/*if(strRecvBuf == "stop"){
			LOG_WARN << "tid=" << muduo::CurrentThread::tid() << "stopped by blockingqueue";
			break;
		}*/

		//LOG_DEBUG << "monitorFileDataObj.GetAttrID() " << monitorFileDataObj.GetAttrID();
		Attr_API(monitorFileDataObj.GetAttrID(), 1);
	}

		LOG_WARN << muduo::CurrentThread::name() << " monitor thread stopped";
}

