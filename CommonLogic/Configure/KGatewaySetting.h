#pragma once
#include <System/KPlatform.h>
#include <System/tinyxml/xmldom.h>

struct KGatewayNode
{
	int id;
	DWORD publicIp;
	DWORD privateIp;
	WORD publicPort;
	WORD privatePort;
	int count;
};
DECLARE_SIMPLE_TYPE(KGatewayNode);

class KGatewaySetting
{
public:
	KGatewaySetting();
	~KGatewaySetting();

public:
	virtual bool Reload(const char * szFilePath);
	void Clear();

public:
	int GetGatewayCount() const;
	const KGatewayNode* GetGateway(int idx) const;

public:
	::System::Collections::DynArray<KGatewayNode,64> m_nodes;
};
