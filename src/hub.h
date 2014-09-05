#ifndef HUB_SRC_HUB_H_
#define HUB_SRC_HUB_H_

#include <muduo/base/ThreadLocalSingleton.h>
#include "common.h"

class ContextInfo
{
		public:
				string m_strRole;	// sub/plub
				uint16_t m_uwTopicID;
				muduo::Timestamp m_lastWorkTime;	//for timewheel clear, fix me
				bool m_bCongestion;

				ContextInfo(void):m_strRole("unknow"), m_uwTopicID(0), m_lastWorkTime(muduo::Timestamp::now()), m_bCongestion(false){;}
				void reset(const string &strRole, const uint16_t &uwTopicID, muduo::Timestamp lastWorkTime, bool bCongestion = false)
				{
						m_strRole = strRole;
						m_uwTopicID = uwTopicID;
						m_lastWorkTime = lastWorkTime;
						m_bCongestion = bCongestion;
				}

				void uptActiveTime(const muduo::Timestamp &timestamp){
						m_lastWorkTime = timestamp;
				}
};


class UserInfo
{
		private:
				uint64_t m_uddwQQ;
				map<uint16_t, uint32_t> m_mapTopic2Index;
				//uint16_t m_uwTopicID;
				//uint32_t m_udwCurrIndex;
				muduo::TimeStamp m_lastUptTime;
				bool m_isInited;

		public:
				UserInfo(void):m_uddwQQ(0), m_lastUptTime(0), m_isInited(false){;}

				const bool &isInited(void){
						return m_isInited;	
				}

				void setInitedFlag(const bool &bInitedFlag){
						m_isInited = bInitedFlag;	
				}

				const muduo::TimeStamp &getLastUptTime(void){
						return m_lastUptTime; 
				} 

				void setUptTime(const muduo::Timestamp &timestamp){
						m_lastUptTime = timestamp;
				}

				const uint64_t& getUserID(void){
						return m_uddwQQ;	
				}
				void setUserID(const uint64_t &uddwUserID){
						m_uddwQQ = uddwUserID;	
				}

				void uptCurrIndex(const uint16_t &uwTopicID, const uint32_t &udwCurrIndex){
						m_mapTopic2Index[uwTopic] = udwCurrIndex;// new or update	
				}

				const uint32_t& getCurrIndex(const uint16_t &uwTopicID){

				}

				void setTopicID(const uint16_t &uwTopicID){
						m_uwTopicID = uwTopicID;	
				}

				void setCurrIndex(const uint32_t &udwCurrIndex){
						m_udwCurrIndex = udwCurrIndex;	
				}
};

/*typedef boost::shared_ptr<TimeWheelMem> TimeWheelMemSharedPtr; 
  typedef boost::weak_ptr<TimeWheelMem> TimeWheelMemWeakPtr;
  typedef boost::unordered_set<TimeWheelMemSharedPtr> TimeWheelBucket;
  typedef boost::circular_buffer<TimeWheelBucket> TimeWheel;

  typedef boost::weak_ptr<muduo::net::TcpConnection> WeakTcpConnectionPtr;

  struct TimeWheelMem : public muduo::copyable
  {
  WeakTcpConnectionPtr m_TcpConnWeakPtr;		
  explicit TimeWheelMem(const WeakTcpConnectionPtr &tcpConnWeakPtr):m_TcpConnWeakPtr(tcpConnWeakPtr)
  };
  */

struct topicInfo
{
		uint16_t m_uwTopicID;
		uint32_t m_udwTotalIndex;
		uint32_t m_udwRealIndex;//back up
		muduo::Timestamp m_newestIndexTime;
		//bool needUpt;

		topicInfo(void):m_uwTopicID(0), m_udwTotalIndex(0), m_udwRealIndex(0), m_newestIndexTime(0){;}

	topicInfo&	operator=(const topicInfo &topicInfoObj){
		if(&topicInfoObj != this){	

		m_uwTopicID = topicInfoObj.m_uwTopicID;
		m_udwTotalIndex = topicInfoObj.m_udwTotalIndex;
		m_udwRealIndex = topicInfoObj.m_uwRealIndex;
		m_newestIndexTime = topicInfoObj.m_newestIndexTime;
		}	
		return *this;
};


class MainThread
{
		private:
				set<muduo::net::EventLoop*> m_setEventLoop;
				muduo::net::TcpServer m_tcpSvr;
				muduo::net::TcpServer m_tcpSvr4IndexUpt;
				muduo::net::EventLoop *m_pCfgCenterEveLoop;

				muduo::MutexLock m_mutexLock;
				muduo::MutexLock m_mutexTopic2LoopListLock;
				muduo::MutexLock m_mutexTopic2SvrNumLock;

				map<uint16_t, uint16_t> m_mapTopic2PubSvrNum;	//ONLY stat NOW
				map< uint16_t, set<muduo::net::EventLoop*> > m_mapTopic2LoopList;

				static const uint8_t ucCheckTimeInter = 10;	//10 secs
				static const uint8_t ucCheckHeartBeatTimeInter = 20; //20 secs NOT receive heartbeat
				map<uint16_t, topicInfo> m_topic2Info;//use for global use

				typedef muduo::ThreadLocalSingleton< map<uint16_t, TcpConnWeakPtrList> > LocalTopic2SubConn;
				typedef muduo::ThreadLocalSingleton< set<uint64_t> > LocalQQSendBlockList;
				typedef muduo::ThreadLocalSingleton< map<uint16_t, muduo::TimeStamp> > LocalTopic2LastUptTime;

				//		typedef muduo::ThreadLocalSingleton< map< uint16_t, set<UserInfo>*  > LocalTopic2UserInfo;

				void onConnection(const muduo::net::TcpConnectionPtr &tcpConnPtr); void onMessage(const TcpConnectionPtr& conn,
								Buffer* buf,
								Timestamp receiveTime);		//timestamp pass by val, faster than ref on X86-64

				void onConn4IndexUpt(const muduo::net::TcpConnectionPtr &tcpConnPtr); void onMsg4IndexUpt(const TcpConnectionPtr& conn,
								Buffer* buf,
								Timestamp receiveTime);
				void onHighWaterMark(const boost::weak_ptr<TcpConnectionPtr> &conn);
				void threadInit(EventLoop* pEventLoop);
				void ExitTask(void);
				void doUnSub(const TcpConnectionPtr& conn, const uint16_t &uwTopicID);
				void doUnPub(const TcpConnectionPtr& conn, const uint16_t &uwTopicID);
				void doSendSelf(const uint16_t &uwTopicID, const muduo::string &content);
				void doSend(const TcpConnectionPtr& conn, const uint16_t &uwTopicID
								, const muduo::string &content, ContextInfo *pContextInfo);
				void doSub(const TcpConnectionPtr& conn, const uint16_t &uwTopicID, ContextInfo *pContextInfo);
				void doPub(const uint16_t &uwTopicID, ContextInfo *pContextInfo);
				int32_t asInt32(const char* buf);
				void onTimer(void);

		public:
				typedef boost::function<void (const muduo::string& topic,
								const muduo::string& content,
								muduo::Timestamp)> SubscribeCallback;
				MainThread(muduo::net::EventLoop *pEventLoop, muduo::net::EventLoop *pCfgCenterEveLoop, const InetAddress& listenAddr, const InetAddress &recvIndexUptListenAddr)
						: m_tcpSvr(pEventLoop, listenAddr, "MainThread"), m_tcpSvr4IndexUpt(pEventLoop, recvIndexUptListenAddr, "RecvIndexUpt")
						  , m_pCfgCenterEveLoop(pCfgCenterEveLoop){
								  m_tcpSvr.setConnectionCallback(boost::bind(&(MainThread::onConnection)
														  , this, _1));
								  m_tcpSvr.setMessageCallback(boost::bind(&(MainThread::onMessage)
														  , this, _1));
								  m_tcpSvr4IndexUpt.setConnectionCallback(boost::bind(&(MainThread::onConn4IndexUpt), this, _1));
								  m_tcpSvr4IndexUpt.setMessageCallback(boost::bind(&(MainThread::onMsg4IndexUpt), this, _1));
						  }

				~MainThread(void){
						cout << "~MainThread" << endl;
				}

				void setThreadNum(const uint8_t &ucThreadNum){
						m_tcpSvr.setThreadNum(ucThreadNum);
				}

				void start(void);

				static void SetExitFlag(int iSigNum);
};

#endif

