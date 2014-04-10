#pragma once

class KGatewayMsgHub
{
public:
	KGatewayMsgHub();
	virtual ~KGatewayMsgHub();

public:
	virtual void onGatewayAccepted(unsigned long long p1, unsigned long long p2);
	virtual void onCharInfoOK(unsigned long long p1, unsigned long long p2);
};
