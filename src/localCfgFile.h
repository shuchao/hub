#ifndef INTSVR_SRC_LOCALCFGFILE_H_
#define INTSVR_SRC_LOCALCFGFILE_H_

#include "common.h"
#include "oi_timer.h"

class LocalCfgFile
{
private:
	string m_strSvrIp;
	string m_strLocProcLogPath;
	
	string m_strCfgDBIp;
	uint16_t m_uwCfgDBPort;
	string m_strCfgDBUser;
	string m_strCfgDBPwd;
	string m_strCfgDBName;
	string m_strSvrInfoTabName;
	string m_strChgInfoTabName;
	uint32_t m_udwUptInterSecs;
	uint16_t m_uwManuFetchSvrInfoPort;
	uint16_t m_uwMainSvrPort;	//use for restart and other control

	string m_strTagDBIp;
	uint16_t m_uwTagDBPort;
	string m_strTagDBUser;
	string m_strTagDBPwd;
	string m_strTagDBName;
	string m_strTagTopRateDBName;
	string m_strMappingTabName;
	string m_strTagTopRateTabName;

	string m_strTEGUserProfSvrIP;
	uint16_t m_uwTEGUserProfSvrPort;

	uint16_t m_uwSvrMonitorPort;

public:
	LocalCfgFile(void):m_uwCfgDBPort(0), m_udwUptInterSecs(0)
		, m_uwManuFetchSvrInfoPort(0), m_uwMainSvrPort(0), m_uwTagDBPort(0)
		, m_uwTEGUserProfSvrPort(0), m_uwSvrMonitorPort(0){;}

	~LocalCfgFile(void){
		LOG_INFO << "~LocalCfgFile";
	}
	
	int InitWithLocCfgFile(const string &strCfgFilePath);

	void SetSvrInfoTabName(const string &strSvrInfoTabName){
		m_strSvrInfoTabName = strSvrInfoTabName;	
	}

	void SetChgInfoTabName(const string &strChgInfoTabName){
		m_strChgInfoTabName = strChgInfoTabName;	
	}

	void SetUptInterSecs(const uint32_t &udwUptInterSecs){
		m_udwUptInterSecs = udwUptInterSecs;	
	}

	void SetSvrIp(const string &strSvrIp){
		m_strSvrIp = strSvrIp;	
	}

	

	void SetTEGUserProfSvrIP(const string &strTEGUserProfSvrIP){
		m_strTEGUserProfSvrIP = strTEGUserProfSvrIP;	
	}

	void SetCfgDBIp(const string &strCfgDBIp){
		m_strCfgDBIp = strCfgDBIp;	
	}

	void SetCfgDBPort(const uint16_t &uwCfgDBPort){
		m_uwCfgDBPort = uwCfgDBPort;	
	}

	void SetCfgDBUser(const string &strCfgDBUser){
		m_strCfgDBUser = strCfgDBUser;	
	}

	void SetCfgDBPwd(const string &strCfgDBPwd){
		m_strCfgDBPwd = strCfgDBPwd;	
	}

	void SetCfgDBName(const string &strCfgDBName){
		m_strCfgDBName = strCfgDBName;	
	}

	void SetCfgDBUptInterSecs(const uint32_t &udwCfgDBUptInterSecs){
		m_udwUptInterSecs = udwCfgDBUptInterSecs;	
	}

	void SetTagDBIp(const string &strTagDBIp){
		m_strTagDBIp = strTagDBIp;	
	}

	void SetTagDBPort(const uint16_t &uwTagDBPort){
		m_uwTagDBPort = uwTagDBPort;	
	}

	void SetTagDBUser(const string &strTagDBUser){
		m_strTagDBUser = strTagDBUser;	
	}

	void SetTagDBPwd(const string &strTagDBPwd){
		m_strTagDBPwd = strTagDBPwd;	
	}

	void SetTagDBName(const string &strTagDBName){
		m_strTagDBName = strTagDBName;	
	}

	void SetTagTopRateDBName(const string &strTagTopRateDBName){
		m_strTagTopRateDBName = strTagTopRateDBName;	
	}

	
	void SetMappingTabName(const string &strMappingTabName){
		m_strMappingTabName = strMappingTabName;	
	}

	void SetTagTopRateTabName(const string &strTagTopRateTabName){
		m_strTagTopRateTabName = strTagTopRateTabName;	
	}

	void SetLocProcLogPath(const string &strLocProcLogPath){
		m_strLocProcLogPath = strLocProcLogPath;	
	}

	void SetManuFetchSvrInfoPort(const uint16_t &uwManuReadCfgDBSvrPort){
		m_uwManuFetchSvrInfoPort = uwManuReadCfgDBSvrPort;	
	}

	void SetMainSvrPort(const uint16_t &uwMainSvrPort){
		m_uwMainSvrPort = uwMainSvrPort;	
	}

	void SetSvrMonitorPort(const uint16_t &uwSvrMonitorPort){
		m_uwSvrMonitorPort = uwSvrMonitorPort;	
	}

	void SetTEGUserProfSvrPort(const uint16_t &uwUserProfSvrPort){
		m_uwTEGUserProfSvrPort = uwUserProfSvrPort;	
	}

	string GetSvrInfoTabName(void){
		return m_strSvrInfoTabName;
	}

	string GetChgInfoTabName(void){
		return m_strChgInfoTabName;
	}

	uint32_t GetUptInterSecs(void){
		return m_udwUptInterSecs;
	}

	string GetSvrIp(void){
		return m_strSvrIp;
	}

	//chaoshu
	uint16_t GetSvrPort(void){
		return m_uwSvrPort;
	}	

	

	string GetTEGUserProfSvrIP(void){
		return m_strTEGUserProfSvrIP;
	}

	uint16_t GetCfgDBPort(void){
		return m_uwCfgDBPort;
	}

	string GetCfgDBIp(void){
		return m_strCfgDBIp;
	}

	string GetCfgDBUser(void){
		return m_strCfgDBUser;
	}

	string GetCfgDBPwd(void){
		return m_strCfgDBPwd;
	}

	string GetCfgDBName(void){
		return m_strCfgDBName;
	}

	uint32_t GetCfgDBUptInterSecs(void){
		return m_udwUptInterSecs;
	}

	uint16_t GetTagDBPort(void){
		return m_uwTagDBPort;
	}

	string GetTagDBIp(void){
		return m_strTagDBIp;
	}

	string GetTagDBUser(void){
		return m_strTagDBUser;
	}

	string GetTagDBPwd(void){
		return m_strTagDBPwd;
	}

	string GetTagDBName(void){
		return m_strTagDBName;
	}

	string GetTagTopRateDBName(void){
		return m_strTagTopRateDBName;
	}

	string GetMappingTabName(void){
		return m_strMappingTabName;
	}

	string GetTagTopRateTabName(void){
		return m_strTagTopRateTabName;
	}

	string GetLocProcLogPath(void){
		return m_strLocProcLogPath;
	}

	uint16_t GetManuFetchSvrInfoPort(void){
		return m_uwManuFetchSvrInfoPort;
	}

	uint16_t GetMainSvrPort(void){
		return m_uwMainSvrPort;
	}

	uint16_t GetSvrMonitorPort(void){
		return m_uwSvrMonitorPort;
	}

	uint16_t GetTEGUserProfSvrPort(void){
		return m_uwTEGUserProfSvrPort;
	}
 };


#endif
