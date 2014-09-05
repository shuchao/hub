#include "handle_config_file.h"
#include "localCfgFile.h"

int LocalCfgFile::InitWithLocCfgFile(const string &strCfgFilePath)
{
	HandleConfigFile handleConfigFileObj;
	int iRet = handleConfigFileObj.LoadConfigFile(strCfgFilePath);
	if(iRet){
		cerr << "handleConfigFileObj.LoadConfigFile failed with file path " << strCfgFilePath << endl;
		RET_LN();
	}

	SetSvrIp(handleConfigFileObj.FetchConfigVal("LOCAL_IP"));
	cout << "m_strSvrIp " << GetSvrIp() << endl;
	
	SetCfgDBIp(handleConfigFileObj.FetchConfigVal("CONFIG_DB_IP"));
	cout << "m_strCfgDBIp " << GetCfgDBIp() << endl;

	stringstream ss;
	ss << handleConfigFileObj.FetchConfigVal("CONFIG_DB_PORT");
	uint16_t uwTmp;
	ss >> uwTmp;
	
	SetCfgDBPort(uwTmp);
	cout << "m_uwCfgDBPort " << GetCfgDBPort() << endl;
	
	SetCfgDBUser(handleConfigFileObj.FetchConfigVal("CONFIG_DB_USER"));
	cout << "m_strCfgDBUser " << GetCfgDBUser() << endl;
	
	SetCfgDBPwd(handleConfigFileObj.FetchConfigVal("CONFIG_DB_PWD"));
	cout << "m_strCfgDBPwd " << GetCfgDBPwd() << endl;
	
	SetCfgDBName(handleConfigFileObj.FetchConfigVal("CONFIG_DB_NAME"));
	cout << "m_strCfgDBName " << GetCfgDBName() << endl;

	SetSvrInfoTabName(handleConfigFileObj.FetchConfigVal("CONFIG_DB_SVRINFO_TAB_NAME"));
	cout << "m_strSvrInfoTabName " << GetSvrInfoTabName() << endl;

	SetChgInfoTabName(handleConfigFileObj.FetchConfigVal("CONFIG_DB_CHGINFO_TAB_NAME"));
	cout << "m_strChgInfoTabName " << GetChgInfoTabName() << endl;

	ss.clear();
	ss << handleConfigFileObj.FetchConfigVal("CONFIG_DB_UPT_INTER");
	uint32_t udwUptInterSecs;
	ss >> udwUptInterSecs;

	SetUptInterSecs(udwUptInterSecs);
	cout << "m_udwUptInterSecs " << GetUptInterSecs() << endl;

	//load tag mapping DB info
	SetTagDBIp(handleConfigFileObj.FetchConfigVal("TAG_DB_IP"));
	cout << "m_strTagDBIp " << GetTagDBIp() << endl;

	ss.clear();
	ss << handleConfigFileObj.FetchConfigVal("TAG_DB_PORT");
	ss >> uwTmp;
	
	SetTagDBPort(uwTmp);
	cout << "m_uwTagDBPort " << GetTagDBPort() << endl;

	SetTagDBUser(handleConfigFileObj.FetchConfigVal("TAG_DB_USER"));
	cout << "m_strTagDBUser " << GetTagDBUser() << endl;
	
	SetTagDBPwd(handleConfigFileObj.FetchConfigVal("TAG_DB_PWD"));
	cout << "m_strTagDBPwd " << GetTagDBPwd() << endl;
	
	SetTagDBName(handleConfigFileObj.FetchConfigVal("TAG_DB_NAME"));
	cout << "m_strTagDBName " << GetTagDBName() << endl;

	SetMappingTabName(handleConfigFileObj.FetchConfigVal("TAG_DB_MAPPING_TAB_NAME"));
	cout << "m_strMappingTabName " << GetMappingTabName() << endl;

	SetLocProcLogPath(handleConfigFileObj.FetchConfigVal("LOC_PROC_LOG_PATH"));
	cout << "m_strLocProcLogPath " << GetLocProcLogPath() << endl;

	ss.clear();
	ss << handleConfigFileObj.FetchConfigVal("MANUAL_CHECK_SVR_INFO_PORT");
	ss >> uwTmp;

	SetManuFetchSvrInfoPort(uwTmp);
	cout << "m_uwManuFetchSvrInfoPort " << GetManuFetchSvrInfoPort() << endl;

	ss.clear();
	ss << handleConfigFileObj.FetchConfigVal("MAIN_SVR_PORT");
	ss >> uwTmp;

	SetMainSvrPort(uwTmp);
	cout << "m_uwMainSvrPort " << GetMainSvrPort() << endl;

	SetTEGUserProfSvrIP(handleConfigFileObj.FetchConfigVal("TEG_USER_PROFILE_SVR_IP"));
	cout << "m_strTEGUserProfSvrIP " << GetTEGUserProfSvrIP() << endl;

	ss.clear();
	ss << handleConfigFileObj.FetchConfigVal("TEG_USER_PROFILE_SVR_PORT");
	ss >> uwTmp;

	SetTEGUserProfSvrPort(uwTmp);
	cout << "m_uwTEGUserProfSvrPort " << GetTEGUserProfSvrPort() << endl;

	ss.clear();
	ss << handleConfigFileObj.FetchConfigVal("SVR_MONITOR_PORT");
	ss >> uwTmp;

	SetSvrMonitorPort(uwTmp);
	cout << "m_uwSvrMonitorPort " << GetSvrMonitorPort() << endl;

	SetTagTopRateDBName(handleConfigFileObj.FetchConfigVal("TAG_DB_TOP_RATE_TAB_DB_NAME"));
	cout << "m_strTagTopRateDBName " << GetTagTopRateDBName() << endl;

	SetTagTopRateTabName(handleConfigFileObj.FetchConfigVal("TAG_DB_TOP_RATE_TAB_NAME"));
	cout << "m_strTagTopRateTabName " << GetTagTopRateTabName() << endl;

	return 0;
}

