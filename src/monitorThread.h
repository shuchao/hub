#ifndef INTSVR_SRC_MONITORTHREAD_H_
#define INTSVR_SRC_MONITORTHREAD_H_

#include "common.h"

class MonitorThread	//use global monitor blocking queue
{
public:
	MonitorThread(void){;}
	~MonitorThread(void){
		cout << "~MonitorThread" << endl;
	}
	void Run(void);
};

#endif


