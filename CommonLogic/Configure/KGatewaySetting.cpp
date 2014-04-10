#include "KGatewaySetting.h"
#include <System/Log/log.h>
#include <System/Misc/StrUtil.h>

KGatewaySetting::KGatewaySetting()
{
	//
}

KGatewaySetting::~KGatewaySetting()
{
	//
}

bool KGatewaySetting::Reload(const char * szFilePath)
{
	XMLDomParser parser;
	parser.Initialize("GB2312");

	KInputFileStream fi;
	if(!fi.Open(szFilePath))
	{
		Log(LOG_ERROR, "error: open gateway setting file: %s", szFilePath);
		return false;
	}

	XMLElementNode xmlRoot;
	if(!parser.Parse(fi, xmlRoot))
	{
		Log(LOG_ERROR, "error: parse gateway setting file: %s, %s", szFilePath, parser.GetErrorMsg());
		return false;
	}

	this->Clear();

	KGatewayNode gatewayNode;

	XMLNodeColl coll = xmlRoot.QueryChildren2("Gateway/Server");

	int c = coll.Size();
	for(int i=0; i<c; i++)
	{
		memset(&gatewayNode, 0, sizeof(gatewayNode));

		XMLElementNode* xmlElem = (XMLElementNode*)coll.GetNode(i);

		int id = xmlElem->QueryAtt("id")->Integer();
		const char* publicIpStr = xmlElem->QueryAtt("public")->c_str();
		const char* privateIpStr = xmlElem->QueryAtt("private")->c_str();
		int loglevel = xmlElem->QueryAtt("loglevel")->Integer();

		KIpAddr ipaddr = str2ipaddr(publicIpStr);
		if(!ipaddr.isValid())
		{
			Log(LOG_ERROR, "error: gateway setting public ip address '%s'", publicIpStr);
			return false;
		}

		gatewayNode.id = id;
		gatewayNode.publicIp = ipaddr.ip;
		gatewayNode.publicPort = ipaddr.port;

		ipaddr = str2ipaddr(privateIpStr);
		if(!ipaddr.isValid())
		{
			Log(LOG_ERROR, "error: gateway setting private address '%s'", privateIpStr);
			return false;
		}

		gatewayNode.privateIp = ipaddr.ip;
		gatewayNode.privatePort = ipaddr.port;

		m_nodes.push_back(gatewayNode);
	}

	return true;
}

void KGatewaySetting::Clear()
{
	m_nodes.clear();
}

int KGatewaySetting::GetGatewayCount() const
{
	return m_nodes.size();
}

const KGatewayNode* KGatewaySetting::GetGateway(int idx) const
{
	int c = m_nodes.size();
	if(idx < 0 || idx >= c) return NULL;
	return &m_nodes[idx];
}
