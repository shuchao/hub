#include "configCenter.h"
#include "localCfgFile.h"
#include "monitorData.h"

extern tr1::shared_ptr<muduo::CountDownLatch> g_spCountDownLatch;
extern tr1::shared_ptr<LogSvrConfig> g_spLogSvrConfigObj;
extern muduo::BlockingQueue<MonitorData> g_monitorBQ;		//ONLY support increment NOW, fix me
extern bool g_exit;		//every thread check it when finish critical task


int LogSvrConfig::FetchSvrInfo(void)
{
	if(!( (g_locCfgFile->GetSvrInfoTabName()).size() ) 
		|| !(g_locCfgFile->GetSvrIp()).size() || 0 == g_locCfgFile->GetSvrPort()){
		LOG_ERROR  << "g_locCfgFile->GetSvrInfoTabName() " << g_locCfgFile->GetSvrInfoTabName()
			<< " g_locCfgFile->GetSvrIp() " << g_locCfgFile->GetSvrIp() 
			<< " g_locCfgFile->GetSvrPort() " << g_locCfgFile->GetSvrPort();
		RET_LN();
	}

	memset(m_DBlink.sQuery, 0, sizeof(m_DBlink.sQuery));
	sprintf(m_DBlink.sQuery, "SELECT svrID, svrIP, svrPort, logLevel, role, state \
		, remark, UNIX_TIMESTAMP(lastModTime) FROM %s WHERE svrIP ='%s' AND svrPort = '%s'"
		, const_cast<char*>((g_locCfgFile->GetSvrInfoTabName()).c_str())
		, g_locCfgFile->GetSvrIp(), g_locCfgFile->GetSvrPort());
	LOG_INFO << "m_DBlink.sQuery is " << m_DBlink.sQuery;

	//m_DBlink.iQueryType = 1;				?????
	static char sErrMsg[10240];
	memset(sErrMsg, 0, sizeof(sErrMsg));
	
	int iRet = TLib_DB_ExecSQL(&m_DBlink, sErrMsg);
	if(iRet || 0 > (m_DBlink.iResNum)){
		LOG_ERROR << "Can't Get Systen Config: errmsg=" << string(sErrMsg);
		g_monitorBQ.put(MonitorData(ATTR_DB_SVR_INFO_FAILED));
		RET_LN();
	}

	if(!(m_DBlink.iResNum)){
		LOG_FATAL << "No System Config Info !!";	//FATAL will crash
	}

	if(1 != m_DBlink.iResNum){		//ONE IP ONE SVR ROW
		LOG_ERROR << "m_DBlink.iResNum " << m_DBlink.iResNum << " != 1";
		g_monitorBQ.put(MonitorData(ATTR_DB_SVR_INFO_FAILED));
		RET_LN();
	}

	uint32_t udwLastLoadTime = m_TableSvrInfo.GetLastLoadTime();	
	m_TableSvrInfo.Init();
	m_TableSvrInfo.SetLastLoadTime(udwLastLoadTime);	//rollback last load time which changed by CheckDBChgInfo 
	
	memset(sErrMsg, 0, sizeof(sErrMsg));
	iRet = TLib_DB_FetchRow(&m_DBlink, sErrMsg);
	if(iRet){
		LOG_ERROR << "TlibDB fetch row error: errmsg=" << sErrMsg;
		g_monitorBQ.put(MonitorData(ATTR_DB_SVR_INFO_FAILED));
		TLib_DB_FreeResult(&m_DBlink);
		RET_LN();
	}

	m_TableSvrInfo.SetSrvId((uint8_t)(atoi(m_DBlink.stRow[0])));
	LOG_INFO << "m_TableSvrInfo.SetSrvId " << int(m_TableSvrInfo.GetSrvId());
	
	m_TableSvrInfo.SetTEGInterCachePath(m_DBlink.stRow[1]);
	LOG_INFO << "m_TableSvrInfo.GetTEGInterCachePath " << m_TableSvrInfo.GetTEGInterCachePath(); 
	
	m_TableSvrInfo.SetInterCacheExpireTime(atoi(m_DBlink.stRow[2]));
	LOG_INFO << "m_TableSvrInfo.GetInterCacheExpireTime " << m_TableSvrInfo.GetInterCacheExpireTime(); 

	m_TableSvrInfo.SetThreadPoolNum((uint16_t)(atoi(m_DBlink.stRow[3])));
	LOG_INFO << "m_TableSvrInfo.SetThreadPoolNum " << m_TableSvrInfo.GetThreadPoolNum(); 
	
	m_TableSvrInfo.SetFrontRetPkgExpireTime(atoi(m_DBlink.stRow[4]));
	LOG_INFO << "m_TableSvrInfo.GetFrontRetPkgExpireTime " << m_TableSvrInfo.GetFrontRetPkgExpireTime(); 

	m_TableSvrInfo.SetLogLevel((uint8_t)(atoi(m_DBlink.stRow[5])));
	LOG_INFO << "m_TableSvrInfo.SetLogLevel " << m_TableSvrInfo.GetLogLevel();
	LOG_DEBUG << "m_TableSvrInfo.SetLogLevel " << m_TableSvrInfo.GetLogLevel();
	muduo::Logger::setLogLevel(m_TableSvrInfo.GetLogLevel());
	
	m_TableSvrInfo.SetState((uint8_t)(atoi(m_DBlink.stRow[6])));
	LOG_INFO << "m_TableSvrInfo.SetState " << m_TableSvrInfo.GetState(); 

	m_TableSvrInfo.SetOMGInterRedisIp(m_DBlink.stRow[7]);
	LOG_INFO << "m_TableSvrInfo.GetOMGInterRedisIp " << m_TableSvrInfo.GetOMGInterRedisIp(); 
	
	m_TableSvrInfo.SetOMGInterRedisPort(uint16_t(atoi(m_DBlink.stRow[8])));
	LOG_INFO << "m_TableSvrInfo.GetOMGInterRedisPort " << m_TableSvrInfo.GetOMGInterRedisPort(); 
	
	m_TableSvrInfo.SetLastChgTime(atoi(m_DBlink.stRow[9]));
	LOG_INFO << "m_TableSvrInfo.SetLastChgTime " << m_TableSvrInfo.GetLastChgTime(); 
	
	LOG_INFO << "m_TableSvrInfo.GetLastLoadTime() " << m_TableSvrInfo.GetLastLoadTime();
	if(0 == m_TableSvrInfo.GetLastLoadTime()){
		m_TableSvrInfo.SetLastLoadTime(m_TableSvrInfo.GetLastChgTime());
		LOG_INFO << "m_TableSvrInfo.SetLastLoadTime " << m_TableSvrInfo.GetLastLoadTime();
	}
	
	
	TLib_DB_FreeResult(&m_DBlink);
	return 0;
}

int LogSvrConfig::LoadSpecTab(const string &strLoadTabName)		
{
	int iRet = (this->*m_mapFetchTab[strLoadTabName])();
	if(iRet){
		LOG_ERROR << "LoadSpecTab failed with table name " << strLoadTabName;
		RET_LN();
	}
	
	return 0;
}


void LogSvrConfig::onConnection(const muduo::net::TcpConnectionPtr &conn)
{
	 LOG_INFO << conn->localAddress().toIpPort() << " -> "
             << conn->peerAddress().toIpPort() << " is "
             << (conn->connected() ? "UP" : "DOWN");

	 if(g_exit){
		LOG_WARN << "config thread quit according to g_exit";
		m_pEveLoop->quit();
	}
}

void LogSvrConfig::onMessage(const muduo::net::TcpConnectionPtr &conn
	, muduo::net::Buffer *buf, muduo::Timestamp receiveTime)
{
	buf->retrieveAll();				//Just ignore, fix me

	if(g_exit){
		LOG_WARN << "config thread quit according to g_exit";
		m_pEveLoop->quit();
	}
}


void LogSvrConfig::init(muduo::net::EventLoop *pEventLoop)
{
	TLIB_DB_LINK *pstDBlink = &m_DBlink;
	TLib_DB_Init(pstDBlink, 0);
	
	char sErrMsg[256] = {0};

	cerr << "g_locCfgFile->GetCfgDBIp() " 
		<< g_locCfgFile->GetCfgDBIp() 
		<< " g_locCfgFile->GetCfgDBUser() "
		<< g_locCfgFile->GetCfgDBUser() << endl;
	
	int iRet = TLib_DB_SetDB(pstDBlink
		, const_cast<char*>((g_locCfgFile->GetCfgDBIp()).c_str())
		, g_locCfgFile->GetCfgDBPort()
		, const_cast<char*>((g_locCfgFile->GetCfgDBUser()).c_str())
		, const_cast<char*>((g_locCfgFile->GetCfgDBPwd()).c_str())
		, const_cast<char*>((g_locCfgFile->GetCfgDBName()).c_str()), sErrMsg);
	if(iRet){
		TLib_DB_CloseDatabase(pstDBlink);
		cerr << "ExecSQL g_stDBLink err:" << sErrMsg << endl;
		
		pEventLoop->quit();
	}

	pstDBlink->iQueryType = 1;	//default return query result set !!
		
	FetchSvrInfo();

	m_mapFetchTab[g_locCfgFile->GetSvrInfoTabName()] = &LogSvrConfig::FetchSvrInfo;
	iRet = LoadSpecTab(g_locCfgFile->GetSvrInfoTabName());	//usually use this func instead of call map directly
	if(iRet){
		cerr << "fetch table " << g_locCfgFile->GetSvrInfoTabName() << " failed" << endl;
		//g_monitorBQ.put(MonitorData(ATTR_DB_SVR_INFO_FAILED));
		pEventLoop->quit();
	}

	//check server state
	if(GetTableSvrInfo().GetState()){
		cerr << "SERVER " << g_locCfgFile->GetSvrIp() << " STATE is IDLE" << endl;;
		pEventLoop->quit();
	}

	
	m_pEveLoop->runEvery( 60, boost::bind(&LogSvrConfig::CheckDBChgInfo, this) );
	m_pEveLoop->runEvery( 60 * 60, boost::bind(&LogSvrConfig::CheckTagTopRateInfo, this) );
	
}

const char* findFirstColon(const string &buf) const
 {
	const void* colon = memchr(buf.c_str(), ':', buf.size());	//test the edge condition, fix me??
   	return static_cast<const char*>(colon);
 }


void LogSvrConfig::uptPubSubInfo(const string &strCmd, const uint16_t &uwTopicID
	, const string &strConnInfo)
{
	const char *pToken = findFirstColon(strConnInfo);
	if(!pToken){
		LOG_ERROR << "strConnInfo " << strConnInfo << " NOT invalid";
		return;
	}

	string strIp;
	strIp.assign(strConnInfo.c_str(), pToken);	//check edge condition, fix me??
	LOG_DEBUG << "strIp " << strIp;
	
	string strPort;
	strPort.assign(pToken + 1, strConnInfo.c_str() + strConnInfo.size());	//check edge condition, fix me??
	LOG_DEBUG << "strPort " << strPort;
	
	memset(m_DBlink.sQuery, 0, sizeof(m_DBlink.sQuery));

	if("sub" == strCmd){
		LOG_INFO << "sub";

		sprintf(m_DBlink.sQuery, "INSERT INTO %s(dispatchID, ip, port'
			, state, remark) VALUES(%d, %s, %s, 1, '')"
			, const_cast<char*>((g_locCfgFile->GetSubSvrInfoTabName()).c_str())
			, uwTopicID, strIp, strPort);
	}else if("pub" == strCmd){
		LOG_INFO << "pub";

		sprintf(m_DBlink.sQuery, "INSERT INTO %s(dispatchID, ip, port'
			, state, remark) VALUES(%d, %s, %s, 1, '')"
			, const_cast<char*>((g_locCfgFile->GetPubSvrInfoTabName()).c_str())
			, uwTopicID, strIp, strPort);

	}else if("unpub" == strCmd){
		LOG_INFO << "unpub";

		sprintf(m_DBlink.sQuery, "UPDATE %s SET state = 0 WHERE dispatchID = %d '
			AND ip = %s AND port = %s"
			, const_cast<char*>((g_locCfgFile->GetPubSvrInfoTabName()).c_str())
			, uwTopicID, strIp, strPort);
	}else if("unsub" == strCmd){
		LOG_INFO << "unsub";

		sprintf(m_DBlink.sQuery, "UPDATE %s SET state = 0 WHERE dispatchID = %d '
			AND ip = %s AND port = %s"
			, const_cast<char*>((g_locCfgFile->GetSubSvrInfoTabName()).c_str())
			, uwTopicID, strIp, strPort);
	}

	LOG_INFO << "m_DBlink.sQuery is " << m_DBlink.sQuery;

	char sErrMsg[10240] = {0};
	int iRet = TLib_DB_ExecSQL(&m_DBlink, sErrMsg);
	if(iRet){
		
		LOG_ERROR << "Can't exec mysql cmd " << m_DBlink.sQuery << " errmsg=" << string(sErrMsg);
		g_monitorBQ.put(MonitorData(ATTR_DB_SVR_INFO_FAILED));
		//rollback in worker thread, fix me ??
	}

	TLib_DB_FreeResult(&m_DBlink);
}

int LogSvrConfig::CheckDBChgInfo(void)		//main frameword timer recall 
{	
	if(g_exit){
		LOG_WARN << "config thread quit according to g_exit";
		m_pEveLoop->quit();
		return 0;
	}

	if((g_locCfgFile->GetChgInfoTabName()).empty()){
		LOG_ERROR << "g_locCfgFile->GetChgInfoTabName() is empty";
		RET_LN();
	}
	
	//First fetch svr info to make sure log info tablename is correct
	memset(m_DBlink.sQuery, 0, sizeof(m_DBlink.sQuery));
	sprintf(m_DBlink.sQuery, "SELECT UNIX_TIMESTAMP(MAX(FChangeTime)) FROM %s WHERE FTableName = '%s'"
		, const_cast<char*>((g_locCfgFile->GetChgInfoTabName()).c_str())
		, const_cast<char*>((g_locCfgFile->GetSvrInfoTabName()).c_str()));
	LOG_INFO << "m_DBlink.sQuery is " << m_DBlink.sQuery;

	static char sErrMsg[10240];
	memset(sErrMsg, 0, sizeof(sErrMsg));
	
	int iRet = TLib_DB_ExecSQL(&m_DBlink, sErrMsg);
	if(iRet || 0 > m_DBlink.iResNum || 1 < m_DBlink.iResNum){
		LOG_ERROR << "Can't Get Systen Config: errmsg=" << string(sErrMsg);
		g_monitorBQ.put(MonitorData(ATTR_DB_SVR_INFO_FAILED));
		RET_LN();
	}

	uint32_t udwChgTime = 0;
	if(m_DBlink.iResNum){
		memset(sErrMsg, 0, sizeof(sErrMsg));
		iRet = TLib_DB_FetchRow(&m_DBlink, sErrMsg);
		if(iRet){
			LOG_ERROR << "TlibDB fetch row error: errmsg=" << sErrMsg;
			g_monitorBQ.put(MonitorData(ATTR_DB_SVR_INFO_FAILED));
			
			TLib_DB_FreeResult(&m_DBlink);
			RET_LN();
		}

		if(m_DBlink.iResNum){
			udwChgTime = atoi(m_DBlink.stRow[0]);
			if(udwChgTime > m_TableSvrInfo.GetLastLoadTime()){
				LOG_INFO << "udwChgTime " << udwChgTime << " > m_TableSvrInfo.GetLastLoadTime() " << m_TableSvrInfo.GetLastLoadTime();
				m_TableSvrInfo.SetLastLoadTime(udwChgTime);
				LOG_INFO << "m_TableSvrInfo.GetLastLoadTime() " << m_TableSvrInfo.GetLastLoadTime();

				TLib_DB_FreeResult(&m_DBlink);
				
				iRet = LoadSpecTab(g_locCfgFile->GetSvrInfoTabName());
				if(iRet){
					LOG_ERROR << "fetch table " << g_locCfgFile->GetSvrInfoTabName() << " failed";
					g_monitorBQ.put(MonitorData(ATTR_DB_SVR_INFO_FAILED));
					RET_LN();
				}
			}
		}
	}

	
	if(g_exit){
		LOG_WARN << "config thread quit according to g_exit";
		m_pEveLoop->quit();
	}
	
	return 0;
}

