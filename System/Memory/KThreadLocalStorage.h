#ifndef _K_THREAD_LOCAL_STORAGE_H_
#define _K_THREAD_LOCAL_STORAGE_H_

#include "../KPlatform.h"

class KThreadLocalStorage
{
private:
	KThreadLocalStorage();

public:
	~KThreadLocalStorage();

public:
	static KThreadLocalStorage& GetInstance();

public:
	// 返回的是 TLS index， -1分配失败
	int AllocStorage();

	// 设置TLS
	BOOL SetValue(int tlsIndex, void* pData);
	
	// 获得TLS中的值
	BOOL GetValue(int tlsIndex, void*& pData);
};

#endif