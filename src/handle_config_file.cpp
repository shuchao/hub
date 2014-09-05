//#include "src/handle_config_file.h"
#include "handle_config_file.h"

string HandleConfigFile::trim(const string &str)
{
	if(str.empty()) {
		return str;
	}
	string::size_type pos = str.find_first_not_of(" \t\n\r\0\x0B");
	if(pos == string::npos) {
		return str;
	}
	
	string::size_type pos2 = str.find_last_not_of(" \t\n\r\0\x0B");
	return str.substr(pos, pos2 - pos + 1);
}


int HandleConfigFile::LoadConfigFile(const string &filePath)
{
	if(filePath.empty()){
		cerr << "filePath is empty" << endl;
		RET_LN()
	}

	ifstream ifs(filePath.c_str());
	if(!ifs){
		cerr << "open " << filePath << " failed" << endl;
		RET_LN()
	}

	string strLine;
	string strKey;
	string strVal;
	string::size_type pos;
	uint16_t uwLineNum = 0;
	while(!(ifs.eof())){
		uwLineNum++;
		getline(ifs, strLine);
		
		strLine = trim(strLine);
		if(strLine.empty() || '#' == strLine.at(0)){
			continue;
		}

		if(string::npos == (pos = strLine.find('='))){
			cerr << "format NOT valid at strLine" << uwLineNum << endl;
			continue;
		}

		strKey = trim(strLine.substr(0, pos));
		strVal = trim(strLine.substr(pos + 1
			, strLine.size() - pos - 1));
		if(strKey.empty() || strVal.empty()){
			cerr << "key/value is empty at line " << uwLineNum << endl;
		}else{
			m_mapKey2Val[strKey] = strVal;
		}
		
	}

	ifs.close();

	map<string, string>::iterator it = m_mapKey2Val.begin();
	while(it != m_mapKey2Val.end()){
		cout << "key is " << it->first << " value is " << it->second << endl;
		it++;
	}

	return 0;
}

string HandleConfigFile::FetchConfigVal(const string & strKey)
{
	map<string, string>::iterator it = m_mapKey2Val.find(strKey);
	if(m_mapKey2Val.end() == it){
		return "";
	}

	return it->second;
}


