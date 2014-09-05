#ifndef INTSVR_SRC_CFGCENTER_H_
#define INTSVR_SRC_CFGCENTER_H_

#include "common.h"
#include "tlib_db.h"

/*Default value kept by local/db cfg, NOT by src itself*/


class HubSvrInfo
{
private:
	uint16_t m_uwSrvId;
	string m_strSvrIp;
	uint16_t m_uwSvrPort;
	uint8_t m_ucLogLevel;
	uint8_t m_ucRole;		// main/back-up
	uint8_t m_ucState;		//working/idle
	string m_strRemark;
	uint32_t m_udwLastChgTime;

	

	/*string m_strTEGInterCachePath;
	uint32_t m_udwInterCacheExpireTime;
		
	uint32_t m_udwMaxReads;
	uint32_t m_udwMaxWrites;
	
	

	string m_strOMGInterRedisIp;
	uint16_t m_uwOMGInterRedisPort;
	
	

	uint32_t m_udwLastLoadTime;*/

public:
	HubSvrInfo(void){
		Init();
	};

	~HubSvrInfo(void){
		cout << "~HubSvrInfo" << endl;
	};

	void Init(void){
		m_uwSrvId = 0;
		m_uwSvrPort = 0;
		m_ucLogLevel = 0;
		m_ucRole = 0;
		m_ucState = 0;
		m_udwLastChgTime = 0;
	}

	uint8_t GetSrvId(void){
		return m_uwSrvId;
	}

	void SetSrvId(const uint8_t &ucSrvId){
		m_uwSrvId = ucSrvId;
	}

	uint8_t GetLogLevel(void){
		return m_ucLogLevel;
	}

	uint8_t GetState(void){
		return m_ucState;
	}

	uint32_t GetLastChgTime(void){
		return m_udwLastChgTime;
	}

	
	void SetLogLevel(const uint8_t &ucLogLevel){
		m_ucLogLevel = ucLogLevel;
	}

	void SetState(const uint8_t &ucState){
		m_ucState = ucState;
	}

	void SetLastChgTime(const uint32_t &udwLastChgTime){
		m_udwLastChgTime = udwLastChgTime;
	}

	void SetLastLoadTime(const uint32_t &udwLastLoadTime){
		m_udwLastLoadTime = udwLastLoadTime;
	}
};

class PubSvrInfo
{
private:
	uint16_t m_uwSrvId;	//key
	uint16_t m_uwDispatchID;
	string m_strSvrIp;	
	uint16_t m_uwSvrPort;	
	uint8_t m_ucState;	//active/unactive
	string m_strRemark;
	uint32_t m_udwLastChgTime;

	/*string m_strTEGInterCachePath;
	uint32_t m_udwInterCacheExpireTime;

	uint16_t m_uwThreadPoolNum;
	uint32_t m_udwFrontRetPkgExpireTime;
		
	uint32_t m_udwMaxReads;
	uint32_t m_udwMaxWrites;
	uint8_t m_ucLogLevel;
	uint8_t m_ucRole;
	string m_strOMGInterRedisIp;
	uint16_t m_uwOMGInterRedisPort;
	uint32_t m_udwLastLoadTime;*/

public:
	PubSvrInfo(void){
		Init();
	};

	~PubSvrInfo(void){
		cout << "~PubSvrInfo" << endl;
	};

	void Init(void){
		m_uwSrvId = 0;
		m_ucIdcId = 0;
		m_ucSetId = 0;
		m_uwBasePort = 0;

		m_udwInterCacheExpireTime = 0;
		//m_udwStatInterval = 0;

		
		m_uwThreadPoolNum = 0;
		m_udwFrontRetPkgExpireTime = 0;
		
		m_udwMaxReads = 0;
		m_udwMaxWrites = 0;
		m_ucRole = 0;
		m_ucState = 0;
		m_udwLastChgTime = 0;
		m_udwLastLoadTime = 0;

		
		m_uwOMGInterRedisPort = 0;
	}

	uint32_t GetFrontRetPkgExpireTime(void){
		return m_udwFrontRetPkgExpireTime;
	}

	void SetFrontRetPkgExpireTime(const uint32_t &udwFrontRetPkgExpireTime){
		m_udwFrontRetPkgExpireTime = udwFrontRetPkgExpireTime;
	}

	string GetOMGInterRedisIp(void){
		return m_strOMGInterRedisIp;
	}

	void SetOMGInterRedisIp(const string &strOMGInterRedisIp){
		m_strOMGInterRedisIp = strOMGInterRedisIp;
	}

	uint16_t GetOMGInterRedisPort(void){
		return m_uwOMGInterRedisPort;
	}

	void SetOMGInterRedisPort(const uint16_t &uwOMGInterRedisPort){
		m_uwOMGInterRedisPort = uwOMGInterRedisPort;
	}

	uint8_t GetSrvId(void){
		return m_uwSrvId;
	}

	string GetTEGInterCachePath(void){
		return m_strTEGInterCachePath;
	}
		
	uint8_t GetLogLevel(void){
		return m_ucLogLevel;
	}

	uint8_t GetState(void){
		return m_ucState;
	}

	uint32_t GetInterCacheExpireTime(void){
		return m_udwInterCacheExpireTime;
	}

	/*uint32_t GetStatInterval(void){
		return m_udwStatInterval;
	}*/

	uint32_t GetLastChgTime(void){
		return m_udwLastChgTime;
	}

	uint32_t GetLastLoadTime(void){
		return m_udwLastLoadTime;
	}

	
	uint16_t GetThreadPoolNum(void){
		return m_uwThreadPoolNum;
	}
	

	void SetSrvId(const uint8_t &ucSrvId){
		m_uwSrvId = ucSrvId;
	}

	void SetInterCacheExpireTime(const uint32_t &udwInterCacheExpireTime){
			m_udwInterCacheExpireTime = udwInterCacheExpireTime;
		}

	void SetThreadPoolNum(const uint16_t &uwThreadPoolNum){
		m_uwThreadPoolNum = uwThreadPoolNum;
	}


	void SetTEGInterCachePath(const string &strInterDBPath){
		m_strTEGInterCachePath = strInterDBPath;
	}

	void SetLogLevel(const uint8_t &ucLogLevel){
		m_ucLogLevel = ucLogLevel;
	}

	void SetState(const uint8_t &ucState){
		m_ucState = ucState;
	}

	void SetLastChgTime(const uint32_t &udwLastChgTime){
		m_udwLastChgTime = udwLastChgTime;
	}

	void SetLastLoadTime(const uint32_t &udwLastLoadTime){
		m_udwLastLoadTime = udwLastLoadTime;
	}
};

class SubSvrInfo
{
private:
	uint16_t m_uwSrvId;		//key
	uint16_t m_uwDispatchID;
	string m_strSvrIp;
	uint16_t m_uwSvrPort;
	uint8_t m_ucState;		//active/unactive
	string m_strRemark;
	uint32_t m_udwLastChgTime;

	/*string m_strTEGInterCachePath;
	uint32_t m_udwInterCacheExpireTime;

	uint16_t m_uwThreadPoolNum;
	uint32_t m_udwFrontRetPkgExpireTime;
		
	uint32_t m_udwMaxReads;
	uint32_t m_udwMaxWrites;
	uint8_t m_ucLogLevel;
	uint8_t m_ucRole;
	string m_strOMGInterRedisIp;
	uint16_t m_uwOMGInterRedisPort;
	uint32_t m_udwLastLoadTime;*/

public:
	PubSvrInfo(void){
		Init();
	};

	~PubSvrInfo(void){
		cout << "~PubSvrInfo" << endl;
	};

	void Init(void){
		m_uwSrvId = 0;
		m_ucIdcId = 0;
		m_ucSetId = 0;
		m_uwBasePort = 0;

		m_udwInterCacheExpireTime = 0;
		//m_udwStatInterval = 0;

		
		m_uwThreadPoolNum = 0;
		m_udwFrontRetPkgExpireTime = 0;
		
		m_udwMaxReads = 0;
		m_udwMaxWrites = 0;
		m_ucRole = 0;
		m_ucState = 0;
		m_udwLastChgTime = 0;
		m_udwLastLoadTime = 0;

		
		m_uwOMGInterRedisPort = 0;
	}

	uint32_t GetFrontRetPkgExpireTime(void){
		return m_udwFrontRetPkgExpireTime;
	}

	void SetFrontRetPkgExpireTime(const uint32_t &udwFrontRetPkgExpireTime){
		m_udwFrontRetPkgExpireTime = udwFrontRetPkgExpireTime;
	}

	string GetOMGInterRedisIp(void){
		return m_strOMGInterRedisIp;
	}

	void SetOMGInterRedisIp(const string &strOMGInterRedisIp){
		m_strOMGInterRedisIp = strOMGInterRedisIp;
	}

	uint16_t GetOMGInterRedisPort(void){
		return m_uwOMGInterRedisPort;
	}

	void SetOMGInterRedisPort(const uint16_t &uwOMGInterRedisPort){
		m_uwOMGInterRedisPort = uwOMGInterRedisPort;
	}

	uint8_t GetSrvId(void){
		return m_uwSrvId;
	}

	string GetTEGInterCachePath(void){
		return m_strTEGInterCachePath;
	}
		
	uint8_t GetLogLevel(void){
		return m_ucLogLevel;
	}

	uint8_t GetState(void){
		return m_ucState;
	}

	uint32_t GetInterCacheExpireTime(void){
		return m_udwInterCacheExpireTime;
	}

	/*uint32_t GetStatInterval(void){
		return m_udwStatInterval;
	}*/

	uint32_t GetLastChgTime(void){
		return m_udwLastChgTime;
	}

	uint32_t GetLastLoadTime(void){
		return m_udwLastLoadTime;
	}

	
	uint16_t GetThreadPoolNum(void){
		return m_uwThreadPoolNum;
	}
	

	void SetSrvId(const uint8_t &ucSrvId){
		m_uwSrvId = ucSrvId;
	}

	void SetInterCacheExpireTime(const uint32_t &udwInterCacheExpireTime){
			m_udwInterCacheExpireTime = udwInterCacheExpireTime;
		}

	void SetThreadPoolNum(const uint16_t &uwThreadPoolNum){
		m_uwThreadPoolNum = uwThreadPoolNum;
	}


	void SetTEGInterCachePath(const string &strInterDBPath){
		m_strTEGInterCachePath = strInterDBPath;
	}

	void SetLogLevel(const uint8_t &ucLogLevel){
		m_ucLogLevel = ucLogLevel;
	}

	void SetState(const uint8_t &ucState){
		m_ucState = ucState;
	}

	void SetLastChgTime(const uint32_t &udwLastChgTime){
		m_udwLastChgTime = udwLastChgTime;
	}

	void SetLastLoadTime(const uint32_t &udwLastLoadTime){
		m_udwLastLoadTime = udwLastLoadTime;
	}
}


class LocalCfgFile;
class LogSvrConfig
{
private:
	TLIB_DB_LINK m_DBlink;
	PubSvrInfo m_TabPubSvrInfo;
	PubSvrInfo m_TabSubSvrInfo;
	muduo::net::EventLoop *m_pEveLoop;

	set<PubSvrInfo> m_pubSvrList;
	set<SubSvrInfo> m_subSvrList;
	
	/*typedef int (LogSvrConfig::*func)(void);		//func pointer
	map<string, func>	m_mapFetchTab;*/

	void onConnection(const TcpConnectionPtr &conn);
	void onMessage(const TcpConnectionPtr &conn
		, muduo::net::Buffer *buf, muduo::Timestamp receiveTime);
	
	int FetchPubSvrInfo(void);
	int FetchSubSvrInfo(void);
	
public:
	LogSvrConfig(void){;}
	~LogSvrConfig(void){
		TLib_DB_CloseDatabase(&m_DBlink);
	}
	void init(muduo::net::EventLoop *pEventLoop);

	int CheckDBChgInfo(void);
	void uptPubSubInfo(const string &strCmd, const uint16_t &uwTopicID
		, const string &strConnInfo);

	muduo::net::EventLoop* GetEventLoop(void){
		return m_pEveLoop;
	}

	
	int LoadSpecTab(const string &strLoadTabName);
	
	

	PubSvrInfo &GetTableSvrInfo(void){
		return m_TableSvrInfo;
	}

	const vector<uint32_t> &GetTagTopRateVec(void){
		return m_vecTagTopRate;
	}

	uint16_t GetThreadPoolNum(void){
                return m_TableSvrInfo.GetThreadPoolNum();
    }

	uint8_t GetSvrState(void){
		return m_TableSvrInfo.GetState();
	}

	uint8_t GetLogLevel(void){
		return m_TableSvrInfo.GetLogLevel();
	}

	uint32_t GetInterCacheExpireTime(void){
		return m_TableSvrInfo.GetInterCacheExpireTime();
	}

	string GetTEGInterCachePath(void){
		return m_TableSvrInfo.GetTEGInterCachePath();
	}

	string GetOMGInterRedisIp(void){
		return m_TableSvrInfo.GetOMGInterRedisIp();
	}

	uint16_t GetOMGInterRedisPort(void){
		return m_TableSvrInfo.GetOMGInterRedisPort();
	}

	
	uint32_t GetFrontRetPkgExpireTime(void){
			return m_TableSvrInfo.GetFrontRetPkgExpireTime();
		}
	
	
	
		
};

#endif
