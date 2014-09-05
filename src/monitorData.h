#ifndef INTSVR_SRC_MONITORDATA_H_
#define INTSVR_SRC_MONITORDATA_H_

#include "common.h"

class MonitorData
{
private:
	uint32_t m_udwAttrID;
	std::string m_strAttrDesc;

public:
	MonitorData(void):m_udwAttrID(0){;}
	MonitorData(const uint32_t &udwAttrID, std::string strAttrDesc = ""):m_udwAttrID(udwAttrID), m_strAttrDesc(strAttrDesc){;}
	uint32_t GetAttrID(void){
		return m_udwAttrID;
	}

	std::string GetAttrDesc(void){
		return m_strAttrDesc;
	}
};


#endif
