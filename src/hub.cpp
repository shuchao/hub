#include <fcntl.h>

#include <boost/ptr_container/ptr_vector.hpp>

#include <muduo/base/Logging.h>
#include <muduo/base/Thread.h>
#include <muduo/base/Types.h>
#include <muduo/net/Channel.h>
#include <muduo/net/EventLoopThreadPool.h>
#include <muduo/net/Socket.h>
#include <muduo/net/SocketsOps.h>

#include "configCenter.h"
#include "interest_record.h"
#include "mblog_ad_image.pb.h"
#include "monitorThread.h"
#include "my_macro.h"
#include "packPB.h"
#include "workThread.h"

#include "hub.h"
#include "hubCodec.h"

extern boost::scoped_ptr<muduo::AsyncLogging> g_asyncLog;
extern boost::shared_ptr<muduo::CountDownLatch> g_spCountDownLatch;
extern LogSvrConfig g_logSvrConfigObj;
extern muduo::BlockingQueue<MonitorData> g_monitorBQ;		//ONLY support increment NOW, fix me
extern bool g_exit;		//every thread check it when finish critical task

void MainThread::ExitTask(void)
{
		LOG_INFO << "ExitTask in main thread";
		set<muduo::net::EventLoop*>::iterator it = m_setEventLoop.begin();
		while(it != m_setEventLoop.end()){

				(*it)->quit();
				it++;
		}

		g_spCountDownLatch.reset(1);	//ONLY make sure cfgcenter finish all work
		g_spCountDownLatch.wait();

		(m_tcpSvr.getLoop())->quit();
}

void MainThread::CheckHeartBeat(void)
{

}



void MainThread::start(void)
{
		m_tcpSvr.setThreadInitCallback(boost::bind(&MainThread::threadInit, this, _1));
		m_tcpSvr.setWriteCompleteCallback(boost::bind(&(MainThread::onWriteComplete), this));

		m_tcpSvr.start();
		m_tcpSvr4IndexUpt.start();
}

void MainThread::doUnSub(const TcpConnectionPtr& conn, const uint16_t &uwTopicID)
{
		//monitor, fix me
		m_pCfgCenterEveLoop->runInLoop(boost::bind(&(LogSvrConfig::uptPubSubInfo), "unsub", uwTopicID
								, (conn->peerAddress()).toIpPort()));	//upt in configcenter, fix me

		ConnectionList &connList = (LocalTopic2SubConn::instance())[uwTopicID];	//add into local conn
		connList.erase(conn);

		if(0 == connList.size()){
				(LocalTopic2SubConn::instance()).erase(uwTopicID);

				{
						MutexLockGuard lock(m_mutexTopic2LoopListLock);
						set<muduo::net::EventLoop*> &setLoop = m_mapTopic2LoopList[uwTopicID];
						setLoop.erase(conn->getLoop());		
				}
		}

		conn->shutdown();
}

void MainThread::doUnPub(const TcpConnectionPtr& conn, const uint16_t &uwTopicID)
{
		//monitor, fix me
		{
				MutexLockGuard lock(m_mutexTopic2SvrNumLock);
				if(m_mapTopic2PubSvrNum.end() != m_mapTopic2PubSvrNum.find(uwTopicID)
								&& 0 < (m_mapTopic2PubSvrNum[uwTopicID])){
						(m_mapTopic2PubSvrNum[uwTopicID])--;	//m_mapTopic2PubSvrNum[uwTopicID] should NOT be 0 before --
				}
		}

		m_pCfgCenterEveLoop->runInLoop(boost::bind(&(LogSvrConfig::uptPubSubInfo), "unpub", uwTopicID
								, (conn->peerAddress()).toIpPort()));	//upt in configcenter, fix me

		conn->shutdown();
}


void MainThread::onConn4IndexUpt(const TcpConnectionPtr& conn)
{

		LOG_INFO << conn->localAddress().toIpPort() << " -> "
				<< conn->peerAddress().toIpPort() << " is "
				<< (conn->connected() ? "UP" : "DOWN");
}

void MainThread::onConnection(const TcpConnectionPtr& conn)
{
		LOG_INFO << conn->localAddress().toIpPort() << " -> "
				<< conn->peerAddress().toIpPort() << " is "
				<< (conn->connected() ? "UP" : "DOWN");

		/*//check cfg center, if backup machine, just deny all connections
		  if(LogSvrConfig.getSvrRole()){
		  LOG_DEBUG << "backup machine, NOT accept connect";
		  conn->shutdown();	//fix me, if change role, client need to reconnect to backup svr
		  return;
		  }*/


		if(__builtin_expect(true == g_exit, 0)){	//hardly happen
				ExitTask();
		}else if(conn->connected()){	
				conn->setContext(new UserInfo());
				conn->setHighWaterMarkCallback(boost::bind(&(MainThread::onHighWaterMark), this),  64*1024); //every conn, 64K buff
				//monitor, fix me
		}else{					//pub svr down, clear all 
				LOG_WARN << "Svr Down";
				//monitor, fix me

				//del conn from block list
				if(!((conn->getContext()).empty())){
						UserInfo *pUserInfo = boost::any_cast<UserInfo*>(conn->getContext());
						(LocalQQSendBlockList::instance())->erase(pUserInfo->getUserID());
				}

				/*
				   if(!((conn->getContext()).empty())){	//fetch topicID
				   ContextInfo *pContextInfo = boost::any_cast<ContextInfo*>(conn->getContext());
				   LOG_DEBUG << "uwTopicID " << pContextInfo->uwTopicID;

				   doUnPub(conn, pContextInfo->uwTopicID);
				   }*/
		}
}


/*bool parseRequest(Buffer* buf, HttpContext* context, Timestamp receiveTime)
  {
  bool ok = true;
  bool hasMore = true;
  while (hasMore)
  {
  if (context->expectRequestLine())
  {
  const char* crlf = buf->findCRLF();
  if (crlf)
  {
  ok = processRequestLine(buf->peek(), crlf, context);
  if (ok)
  {
  context->request().setReceiveTime(receiveTime);
  buf->retrieveUntil(crlf + 2);
  context->receiveRequestLine();		//chaoshu: change state to ExpectHeader to continue
  }
  else
  {
  hasMore = false;
  }
  }
  else
  {
  hasMore = false;
  }
  }
  else if (context->expectHeaders())
  {
  const char* crlf = buf->findCRLF();
  if (crlf)
  {
  const char* colon = std::find(buf->peek(), crlf, ':');
  if (colon != crlf)
  {
  context->request().addHeader(buf->peek(), colon, crlf);
  }
  else
  {
// empty line, end of header
context->receiveHeaders();
hasMore = !context->gotAll();
}
buf->retrieveUntil(crlf + 2);
}
else
{
hasMore = false;
}
}
else if (context->expectBody())
{
// FIXME:
}
}
return ok;
}*/

void MainThread::onWriteComplete(const TcpConnectionPtr& conn)  
{
		//del conn from block list
		if(!((conn->getContext()).empty())){
				UserInfo *pUserInfo = boost::any_cast<UserInfo*>(conn->getContext());
				(LocalQQSendBlockList::instance())->erase(pUserInfo->getUserID());
		}

		void MainThread::onHighWaterMark(const TcpConnectionPtr& conn, size_t len){
				LOG_INFO << "HighWaterMark " << len << "for " << conn->peerAddress().toIpPort();

				if(!((conn->getContext()).empty())){
						UserInfo *pUserInfo = boost::any_cast<UserInfo*>(conn->getContext()); 

						LOG_INFO << "qq is " << pUserInfo->getUserID();

						//add conn into black list 
						(LocalQQSendBlockList::instance())->insert(pUserInfo->getUserID());
				}


		}
		/*void MainThread::onHighWaterMark(const boost::weak_ptr<TcpConnectionPtr> &conn)
		  {
		//monitor, fix me
		boost::shared_ptr<TcpConnectionPtr> shpConn(conn.lock());
		if(shpConn){
		<< shpConn->localAddress().toIpPort() << " -> "
		<< shpConn->peerAddress().toIpPort() << " is "
		<< (conn->connected() ? "UP" : "DOWN");
		ContextInfo *pContextInfo = boost::any_cast<ContextInfo*>(shpConn->getContext());
		LOG_WARN << "High water for " << shpConn->peerAddress().toIpPort() << " topic ID " << pContextInfo->m_uwTopicID;

		shpConn->m_bCongestion = true;		//use for control send
		//check true is set, fix me
		//then what we do, fix me 
		}
		}*/


		void MainThread::doSendSelf(const uint16_t &uwTopicID, const muduo::string &content)
		{
				map<uint16_t, ConnectionList>::iterator it = (LocalTopic2SubConn::Instance()).find(uwTopicID);
				if((LocalTopic2SubConn::Instance()).end() == it){
						LOG_INFO << "No topic ID " << uwTopicID << " find in LocalTopic2SubConn, maybe no sub from local thread";
				}else{
						set<TcpConnectionPtr>::iterator itSet = it->begin();
						while(itSet != it->end()){
								//monitor, fix me
								(get_pointer(*itSet))->send(content);
								itSet++;
						}
				}
		}

		void MainThread::doSend(const TcpConnectionPtr& conn, const uint16_t &uwTopicID
						, const muduo::string &content, ContextInfo *pContextInfo)
		{
				//check conn info with msg
				if("pub" != pContextInfo->m_strRole || pContextInfo->m_uwTopicID != uwTopicID){
						LOG_ERROR << "pContextInfo->m_strRole " << pContextInfo->m_strRole 
								<< " pContextInfo->m_uwTopicID " << pContextInfo->m_uwTopicID
								<< " uwTopicID " << uwTopicID;
				}else{
						doSendSelf(uwTopicID, content);

						//send to other loop
						set<muduo::net::EventLoop*> setLoop;
						{
								MutexLockGuard lock(m_mutexTopic2LoopListLock);
								setLoop = m_mapTopic2LoopList[uwTopicID];	//copy, decrease the mutex area
						}

						set<muduo::net::EventLoop*>::iterator itSet = setLoop.begin();	//loop will NOT invalid, so safe
						while(itSet != setLoop.end()){
								if(*itSet != conn.getLoop()){
										(*itSet)->runInLoop(boost::bind(&(MainThread::doSendSelf), this, uwTopicID, content));
								}
								itSet++;
						}
				}
		}

		void MainThread::doSub(const TcpConnectionPtr& conn, const uint16_t &uwTopicID, ContextInfo *pContextInfo)
		{
				//monitor, fix me
				conn->setTcpNoDelay(true);		//NO negly
				conn->setHighWaterMarkCallback(					//slow down send
								boost::bind(&(MainThread::onHighWaterMark), (boost::weak_ptr<TcpConnectionPtr>)(conn)), 10*1024*1024);

				pContextInfo->reset("sub", uwTopicID, muduo::Timestamp::now());

				ConnectionList &connList = (LocalTopic2SubConn::instance())[uwTopicID];	//add into local conn
				connList.insert(conn);

				{
						MutexLockGuard lock(m_mutexTopic2LoopListLock);
						set<muduo::net::EventLoop*> &setLoop = m_mapTopic2LoopList[uwTopicID];
						setLoop.insert(conn->getLoop());		
				}

				m_pCfgCenterEveLoop->runInLoop(boost::bind(&(LogSvrConfig::uptPubSubInfo), "sub", uwTopicID
										, (conn->peerAddress()).toIpPort()));	//upt in configcenter, fix me
		}

		void MainThread::doPub(const TcpConnectionPtr& conn, const uint16_t &uwTopicID, ContextInfo *pContextInfo)
		{
				//monitor, fix me
				{
						MutexLockGuard lock(m_mutexTopic2SvrNumLock);						
						(m_mapTopic2PubSvrNum[uwTopicID]++);
				}

				pContextInfo->reset("pub", uwTopicID, muduo::Timestamp::now());

				m_pCfgCenterEveLoop->runInLoop(boost::bind(&(LogSvrConfig::uptPubSubInfo), "pub", uwTopicID
										, (conn->peerAddress()).toIpPort()));	//upt in configcenter, fix me
		}


		int32_t MainThread::asInt32(const char* buf)
		{

				int32_t be32 = 0;

				::memcpy(&be32, buf, sizeof(be32));

				return muduo::net::sockets::networkToHost32(be32);
		}


		void MainThread::onMsg4IndexUpt(const TcpConnectionPtr& conn,
						muduo::net::Buffer* buf,
						muduo::Timestamp receiveTime)
		{
				topicInfo topicInfoObj;
				//parse http json, fix me??

				MutexLockGuard lock(m_mutexLock);	
				m_topic2Info[topicInfoObj.m_uwTopicID] = topicInfoObj;	

		}

		void MainThread::onMessage(const TcpConnectionPtr& conn,
						muduo::net::Buffer* buf,
						muduo::Timestamp receiveTime)
		{
				//monitor, fix me
				if(__builtin_expect(true == g_exit, 0)){	//hardly happen
						ExitTask();
				}else{
						UserInfo *pUserInfo = boost::any_cast<UserInfo*>(conn->context());
						pUserInfo->uptActiveTime(receiveTime);//update active time for timewheel
						//parse http json data, fix me??
						uint64_t uddwQQ = 0;
						uint16_t uwTopicID = 0;
						uint32_t udwCurrIndex = 0;

						if(false == pUserInfo->isInited()){
								LOG_DEBUG << "Init user info";

								pUserInfo->setUserID(uddwQQ);	
								pUserInfo->uptCurrIndex(uwTopicID, udwCurrIndex);
								pUserInfo->setInitedFlag(true);
						}else if(__builtin_expect(uddwQQ !=pUserInfo->getUserID() , 0)){
								LOG_ERROR << "curr QQ " << uddwQQ << " != context.qq " << pUserInfo->getUserInfo();	

								conn->shutdown(); //clear other index in onConnection
						}	



						/////////////////back
						ContextInfo *pContextInfo = boost::any_cast<ContextInfo*>(conn->context());
						pContextInfo->uptActiveTime(receiveTime);		//update alive time for timewheel

						//check completely
						uint32_t udwLen = 0;
						if(buf->readableBytes() < udwLen){
								return;
						}

						memcpy(&udwLen, buf->peek(), sizeof(udwLen));
						udwLen = sockets::networkToHost32(udwLen);

						LOG_DEBUG << "udwLen " << udwLen;

						if(buf->readableBytes() - sizeof(udwLen) < udwLen){
								return;
						}

						//check adler32
						int32_t expectedCheckSum = asInt32(buf + udwLen - sizeof(int32_t));
						int32_t checkSum = static_cast<int32_t>(adler32(1, reinterpret_cast<const Bytef*>(buf)
												,static_cast<int>(udwLen - sizeof(int32_t))));		//checkSum for all but itself
						if(expectedCheckSum != checkSum){
								LOG_ERROR << "expectedCheckSum " << expectedCheckSum << " NOT match checkSum " << checkSum;
								//monitor, fix me

								//disconnect??
						}

						LOG_DEBUG << "Recv buf " << StringPiece(buf->peek(), static_cast<int>(udwLen + sizeof(udwLen))).data();

						buf->retrieveInt32();

						hub::ParseResult result = hub::kSuccess;

						muduo::string cmd;
						uint16_t uwtopicID = 0;
						muduo::string topic;
						muduo::string content;
						result = hub::parseMessage(buf, &cmd, &topic, &content);

						if (result == hub::kSuccess){
								stringstream ss;
								ss << topic;
								uint16_t uwTopicID = 0;
								ss >> uwTopicID;
								LOG_DEBUG << "uwTopicID " << uwTopicID;

								if("send" == cmd){
										doSend(conn, uwTopicID, content, pContextInfo);

								}else if("sub" == cmd){		//cmd valid already checked in parseMessage
										doSub(conn, uwTopicID, pContextInfo);
								}else if("pub" == cmd){
										doPub(uwTopicID, pContextInfo);
								}else if("unsub" == cmd){
										doUnSub(conn, uwTopicID);
								}else if("unpub" == cmd){
										doUnPub(conn, uwTopicID);
								}

						}else if (result == hub::kError){
								LOG_ERROR << "parseMessage error";
						}

						buf->retrieve(udwLen);

				}
		}

		void MainThread::onTimer(void)
		{
				map<uint16_t, topicInfo> mapTopic2Info;
				{
						MutexLockGuard lock(m_mutexLock);		
						mapTopic2Info = m_topic2Info;
				}

				map<uint16_t, topicInfo>::iterator it = mapTopic2Info.begin();
				while(it != mapTopic2Info.end()){
						const topicInfo &topicInfoRef = it->second;
						if((LocalTopic2LastUptTime::insance())[topicInfo.getTopicID()] < topicInfo.m_newestIndexTime){ //if new insert, local upt time should be 0, test me,fix me??
								map<uint16_t, TcpConnWeakPtrList> *pMap = LocalTopic2SubConn::instance();
								map<uint16_t, TcpConnWeakPtrList>::iterator itMap = pMap->find(topicInfo.getTopicID());
								if(itMap != pMap->end()){
										TcpConnWeakPtrListInter itSet = (itMap->second).begin();
										while(itSet != (itMap->second).end()){
												TcpConnectionPtr tcpConnSharedPtr = itSet->lock();
												if(!tcpConnSharedPtr || (tcpConnSharedPtr->getContext()).empty()){
														LOG_DEBUG << "Dead conn or context null";
														itSet = (itMap->second).erase(itSet);//test fix me??
														continue;
												}else if(!((tcpConnSharedPtr->getContext()).empty())){
														UserInfo *pUserInfo = boost::any_cast<UserInfo*>(tcpConnSharedPtr->getContext());	
														//compare index
														if(pUserInfo->getUserID() < topicInfo.getTotalIndex()){
																//pack http, and send, fix me??		
														}
												}

												itSet++;
										}	
								}

								(LocalTopic2LastUptTime::instance())[topicInfo.getTopicID()] = topicInfo.m_newestIndexTime;
						}

						it++;
				}
		}


		void MainThread::threadInit(muduo::net::EventLoop* loop)
		{
				assert(LocalTopic2SubConn::pointer() == NULL);
				LocalTopic2SubConn::instance();
				assert(LocalTopic2SubConn::pointer() != NULL);

				loop->runAt(0.1, boost::bind(&(MainThread::onTimer), this));

				MutexLockGuard lock(m_mutexLock); 
				m_setEventLoop.insert(loop); 
		} 

		static void MainThread::SetExitFlag(int iSigNum)			
		{
				if(SIGTERM == iSigNum || SIGINT == iSigNum){
						cout << "g_exit = true;" << endl;
						g_exit = true;
				}
		}

		int main(int argc, char **argv)
		{
				if(2 != argc){
						cerr << "hub <cfg file>" << endl;
						return -1;
				}

				int iRet = g_locCfgFile.InitWithLocCfgFile(argv[1]);
				if(iRet){
						cerr << "g_locCfgFile.InitWithLocCfgFile failed with file path " << argv[1] << endl;
						exit(-1);
				}

				char szName[256] = {0};
				strncpy(szName, g_locCfgFile.GetLocProcLogPath().c_str()
								, strlen(g_locCfgFile.GetLocProcLogPath().c_str()));

				//default LOG_INFO
				g_asyncLog.reset(new muduo::AsyncLogging(szName, 500*1000*1000));
				muduo::Logger::setOutput(MyAsyncOutput);
				g_asyncLog->start();


				signal(SIGTERM,MainThread::SetExitFlag);			//fix me
				signal(SIGINT,MainThread::SetExitFlag);			//fix me

				//config center
				muduo::net::EventLoopThread cfgCenterEventLoopThread(boost::bind(&(LogSvrConfig::init, &g_logSvrConfigObj
												, _1)));
				(cfgCenterEventLoopThread.startLoop())->runEvery( 30
								, boost::bind(&LogSvrConfig::CheckDBChgInfo, &g_logSvrConfigObj) );

				//monitor thread
				MonitorThread monitorThreadObj;
				muduo::thread(boost::bind(&(MonitorThread::Run), &monitorThreadObj)
								, muduo::string("Monitor Thread"));

				//fail parse thread
				FailParseThread failParseThreadObj;			//fix me
				muduo::thread(boost::bind(&(FailParseThread::Run), &failParseThreadObj)
								, muduo::string("FailParse Thread"));

				muduo::net::EventLoop loop;
				muduo::net::InetAddress svrAddr(g_locCfgFile.GetSvrIp()
								, g_locCfgFile.GetSvrPort());
				muduo::net::InetAddress svrAddr4IndexUpt(g_locCfgFile.GetSvrIp4IndexUpt()
								, g_locCfgFile.GetSvrPort4IndexUpt());
				MainThread mainThreadObj(&loop, svrAddr, svrAddr4IndexUpt);

				mainThreadObj.setThreadNum(g_locCfgFile.GetWorkerNum());
				mainThreadObj.start();

				if(cfgCenterEventLoopThread.isBackupSvr()){
						LOG_INFO << "backup svr";

						loop.runEvery(MainThread::ucCheckTimeInter
										, boost::bind(&(MainThread::CheckHeartBeat), &mainThreadObj));
				}

				loop.loop();

				LOG_WARN << "proc exit";
				cerr << "proc exit" << endl;
				return 0;
		}


