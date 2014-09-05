#ifndef INTSVR_SRC_HANDLECONFIGFILE_H_
#define INTSVR_SRC_HANDLECONFIGFILE_H_

#include "common.h" 

class HandleConfigFile
{
private:
	map<string, string> m_mapKey2Val;
	string trim(const string &str);
public:
	int LoadConfigFile(const string &filePath);
	string FetchConfigVal(const string &strKey);
};

#endif

