#pragma once

class KLoginMsgHub
{
public:
	KLoginMsgHub();
	virtual ~KLoginMsgHub();

public:
	virtual void onLoginServerConnected(unsigned long long p1, unsigned long long p2);
	virtual void onLoginRsp(unsigned long long p1, unsigned long long p2);
	virtual void onLoginServerList(unsigned long long p1, unsigned long long p2);
};
