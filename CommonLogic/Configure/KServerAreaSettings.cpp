#include "KServerAreaSettings.h"

//// KServerAreaSettings::Service

KServerAreaSettings::Service::Service()
{
	m_serviceName[0] = '\0';
	memset(&m_publicAddr, 0, sizeof(m_publicAddr));
	memset(&m_privateAddr, 0, sizeof(m_privateAddr));
	m_instanceCount = 1;
}

KServerAreaSettings::Service::~Service()
{

}

KServerAreaSettings::Service::Service(const Service& o)
{
	strcpy_k(m_serviceName, sizeof(m_serviceName), o.m_serviceName);
	m_publicAddr = o.m_publicAddr;
	m_privateAddr = o.m_privateAddr;
	m_instanceCount = o.m_instanceCount;
}

void KServerAreaSettings::Service::clear()
{
	m_serviceName[0] = '\0';
	memset(&m_publicAddr, 0, sizeof(m_publicAddr));
	memset(&m_privateAddr, 0, sizeof(m_privateAddr));
	m_instanceCount = 1;
}

BOOL KServerAreaSettings::Service::setName(const char* name)
{
	strcpy_k(m_serviceName, sizeof(m_serviceName), name);
	return TRUE;
}

BOOL KServerAreaSettings::Service::setAddress(const char* address)
{
	m_publicAddr = KSocketAddress::FromString(address);
	if(!m_publicAddr.m_ip && !m_publicAddr.m_port)
		return FALSE;
	return TRUE;
}

size_t KServerAreaSettings::Service::serialize(KMemoryStream* so)
{
	size_t pos = so->size();
	if(!so->WriteString8(m_serviceName))
		return -1;
	if(!so->WriteByteArray(&m_publicAddr, sizeof(m_publicAddr)))
		return -1;
	return so->size() - pos;
}

BOOL KServerAreaSettings::Service::deserialize(KMemoryStream* si)
{
	this->clear();
	if(!si->ReadString8(m_serviceName, sizeof(m_serviceName)))
		return FALSE;
	if(!si->ReadByteArray(&m_publicAddr, sizeof(m_publicAddr)))
		return FALSE;
	return TRUE;
}

////// 

KServerAreaSettings::Server::Server()
	: m_id(-1)
	, m_minVer(0.0f)
	, m_maxVer(0.0f)
	, m_showState(enumESSS_Fluency)
	, m_playerCount(0)
	, m_maxPlayerCount(10240)
	, m_recommented(FALSE)
{
	m_serverName[0] = '\0';
}

KServerAreaSettings::Server::Server(const KServerAreaSettings::Server& o)
	: m_id(o.m_id)
	, m_minVer(o.m_minVer)
	, m_maxVer(o.m_maxVer)
	, m_showState(o.m_showState)
	, m_playerCount(o.m_playerCount)
	, m_maxPlayerCount(o.m_maxPlayerCount)
	, m_recommented(o.m_recommented)
	, m_services(o.m_services)
{
	memcpy(&m_serverName, &o.m_serverName, sizeof(m_serverName));
}

KServerAreaSettings::Server::~Server()
{

}

BOOL KServerAreaSettings::Server::load(XMLElementNode* xml)
{
	const StringValue* sval;
	
	sval = xml->QueryAtt("id");
	if(!sval)
	{
		Log(LOG_ERROR|LOG_CONSOLE, "error: LoginServerList.xml <server> must has an id");
		return FALSE;
	}
	m_id = sval->Integer();

	sval = xml->QueryAtt("name");
	if(!sval)
	{
		Log(LOG_ERROR|LOG_CONSOLE, "error: LoginServerList.xml <server> must has a name");
		return FALSE;
	}
	strcpy_k(m_serverName, sizeof(m_serverName), sval->c_str());

	sval = xml->QueryAtt("maxPlayer");
	if(sval) m_maxPlayerCount = sval->Integer();

	sval = xml->QueryAtt("ver");
	if(sval)
	{
	#pragma warning(disable:4996)
		if(sscanf(sval->c_str(), "%f-%f", &m_minVer, &m_maxVer) != 2)
		{
			Log(LOG_ERROR|LOG_CONSOLE, "error: LoginServerList.xml <server> has an invalid ver attribute '%s'", sval->c_str());
			return FALSE;
		}
	#pragma warning(default:4996)
	}

	XMLNodeColl coll = xml->QueryChildren("Service");
	int n = coll.Size();
	if(n < 3)
	{
		Log(LOG_ERROR|LOG_CONSOLE, "error: LoginServerList.xml <server %s> must has 3 services at least", m_serverName);
		return FALSE;
	}

	KServerAreaSettings::Service svc;
	for(int i=0; i<n; i++)
	{
		svc.clear();

		XMLElementNode* svcXml = (XMLElementNode*)coll.GetNode(i);
		sval = svcXml->QueryAtt("name");
		if(!sval)
		{
			Log(LOG_ERROR|LOG_CONSOLE, "error: LoginServerList.xml <server %s> service must has a name", m_serverName);
			return FALSE;
		}
		svc.setName(sval->c_str());

		sval = svcXml->QueryAtt("addr");
		if(!sval)
		{
			Log(LOG_ERROR|LOG_CONSOLE, "error: LoginServerList.xml <server %s> service must has a addr attribute", m_serverName);
			return FALSE;
		}
		if(!svc.setAddress(sval->c_str()))
		{
			Log(LOG_ERROR|LOG_CONSOLE, "error: LoginServerList.xml <server %s> service %s invalid addr attribute %s",
				m_serverName, svc.m_serviceName, sval->c_str());
			return FALSE;
		}

		sval = svcXml->QueryAtt("privateAddr");
		if(sval) svc.m_privateAddr = KSocketAddress::FromString(sval->c_str());

		sval = svcXml->QueryAtt("count");
		if(sval) svc.m_instanceCount = sval->Integer();

		m_services.push_back(svc);
	}

	return TRUE;
}

void KServerAreaSettings::Server::clear()
{
	m_id = -1;
	m_minVer = 0.0f;
	m_maxVer = 0.0f;
	m_showState = enumESSS_Fluency;
	m_playerCount = 0;
	m_maxPlayerCount = 10240;
	m_recommented = FALSE;
	m_services.clear();
}

float KServerAreaSettings::Server::minVersion() const
{
	return m_minVer;
}

float KServerAreaSettings::Server::maxVersion() const
{
	return m_maxVer;
}

bool KServerAreaSettings::Server::foreach_service(KServerAreaSettings::Server::IVisitor& visitor)
{
	int n = m_services.size();
	for(int i=0; i<n; i++)
	{
		const KServerAreaSettings::Service& svc = m_services.at(i);
		if(!visitor.visit(&svc)) return false;
	}
	return true;
}

int KServerAreaSettings::Server::getServiceCount() const
{
	return m_services.size();
}

const KServerAreaSettings::Service* KServerAreaSettings::Server::getServiceAt(int idx) const
{
	if(idx < 0 || idx >= m_services.size())
		return NULL;
	return &m_services.at(idx);
}

const KServerAreaSettings::Service* KServerAreaSettings::Server::getService(const char* name) const
{
	int n = m_services.size();
	for(int i=0; i<n; i++)
	{
		const KServerAreaSettings::Service& svc = m_services.at(i);
		if(stricmp(svc.m_serviceName, name) == 0)
			return &svc;
	}
	return NULL;
}

int KServerAreaSettings::Server::getServices(const char* name, const KServerAreaSettings::Service** ppService, int count) const
{
	int num = 0;
	int n = m_services.size();
	for(int i=0; i<n; i++)
	{
		const KServerAreaSettings::Service& svc = m_services.at(i);
		if(stricmp(svc.m_serviceName, name) == 0)
			ppService[num++] = &svc;
	}
	return num;
}

size_t KServerAreaSettings::Server::serialize(KMemoryStream* so)
{
	size_t pos = so->size();

	if(!so->WriteInt(m_id)) return -1;
	if(!so->WriteString8(m_serverName)) return -1;
	if(!so->WriteFloat(m_minVer)) return -1;
	if(!so->WriteFloat(m_maxVer)) return -1;
	if(!so->WriteByte((BYTE)m_showState)) return -1;
	if(!so->WriteWord((WORD)m_playerCount)) return -1;
	if(!so->WriteWord((WORD)m_maxPlayerCount)) return -1;
	if(!so->WriteByte((BYTE)m_recommented)) return -1;
	if(!so->WriteShort((short)m_services.size())) return FALSE;

	for(int i=0; i<m_services.size(); i++)
	{
		KServerAreaSettings::Service& svc = m_services.at(i);
		if(!svc.serialize(so))
			return -1;
	}

	return so->size() - pos;
}

BOOL KServerAreaSettings::Server::deserialize(KMemoryStream* si)
{
	this->clear();

	if(!si->ReadInt(m_id)) return FALSE;
	if(!si->ReadString8(m_serverName, sizeof(m_serverName))) return FALSE;
	if(!si->ReadFloat(m_minVer)) return FALSE;
	if(!si->ReadFloat(m_maxVer)) return FALSE;

	BYTE serverState;
	if(!si->ReadByte(serverState)) return FALSE;
	m_showState = (ENUM_SERVERLIST_SHOW_STATE)serverState;

	WORD wTmp; BYTE bTmp;
	if(!si->ReadWord(wTmp)) return FALSE; m_playerCount = (int)(DWORD)wTmp;
	if(!si->ReadWord(wTmp)) return FALSE; m_maxPlayerCount = (int)(DWORD)wTmp;
	if(!si->ReadByte(bTmp)) return FALSE; m_recommented = (BOOL)bTmp;

	short n;
	if(!si->ReadShort(n)) return FALSE;

	KServerAreaSettings::Service svc;
	for(int i=0; i<(int)n; i++)
	{
		if(!svc.deserialize(si)) return FALSE;
		m_services.push_back(svc);
	}

	return TRUE;
}

//// KServerAreaSettings::Area ////

KServerAreaSettings::Area::Area():m_areaID(-1)
{
	m_areaName[0] = '\0';	
}

KServerAreaSettings::Area::Area(const Area& o)
	: m_areaID(o.m_areaID)
	, m_servers(o.m_servers)
{
	memcpy(m_areaName, o.m_areaName, sizeof(m_areaName));
}

KServerAreaSettings::Area::~Area()
{
	
}

BOOL KServerAreaSettings::Area::load(XMLElementNode* xml)
{
	const StringValue* sval;

	sval = xml->QueryAtt("id");
	if(!sval)
	{
		Log(LOG_ERROR|LOG_CONSOLE, "error: LoginServerList.xml area must has an id attribute");
		return FALSE;
	}
	m_areaID = sval->Integer();

	sval = xml->QueryAtt("name");
	if(!sval)
	{
		Log(LOG_ERROR|LOG_CONSOLE, "error: LoginServerList.xml area must has a name attribute");
		return FALSE;
	}
	strcpy_k(m_areaName, sizeof(m_areaName), sval->c_str());

	KServerAreaSettings::Server svr;

	XMLNodeColl coll = xml->QueryChildren("Server");
	int n = coll.Size();
	for(int i=0; i<n; i++)
	{
		svr.clear();
		XMLElementNode* svrXml = (XMLElementNode*)coll.GetNode(i);
		if(!svr.load(svrXml))
			return FALSE;
		m_servers.push_back(svr);
	}

	return TRUE;
}

void KServerAreaSettings::Area::clear()
{
	m_areaID = -1;
	m_areaName[0] = '\0';
	m_servers.clear();
}

bool KServerAreaSettings::Area::foreach_server(KServerAreaSettings::Area::IVisitor& visitor)
{
	int n = m_servers.size();
	for(int i=0; i<n; i++)
	{
		const KServerAreaSettings::Server& svr = m_servers.at(i);
		if(!visitor.visit(&svr))
			return false;
	}
	return true;
}

int KServerAreaSettings::Area::getServerCount() const
{
	return m_servers.size();
}

const KServerAreaSettings::Server* KServerAreaSettings::Area::getServerAt(int idx) const
{
	if(idx < 0 || idx >= m_servers.size())
		return NULL;
	return &m_servers.at(idx);
}

const KServerAreaSettings::Server* KServerAreaSettings::Area::getServer(int serverId) const
{
	int n = m_servers.size();
	for(int i=0; i<n; i++)
	{
		const KServerAreaSettings::Server& svr = m_servers.at(i);
		if(svr.m_id == serverId)
			return &svr;
	}
	return NULL;
}

const KServerAreaSettings::Server* KServerAreaSettings::Area::getServer(const char* serverName) const
{
	int n = m_servers.size();
	for(int i=0; i<n; i++)
	{
		const KServerAreaSettings::Server& svr = m_servers.at(i);
		if(stricmp(svr.m_serverName, serverName) == 0)
			return &svr;
	}
	return NULL;
}

size_t KServerAreaSettings::Area::serialize(KMemoryStream* so)
{
	size_t pos = so->size();
	
	int n = m_servers.size();
	if(!so->WriteInt(m_areaID)) return -1;
	if(!so->WriteString8(m_areaName)) return -1;
	if(!so->WriteShort((short)n)) return -1;
	for(int i=0; i<n; i++)
	{
		KServerAreaSettings::Server& svr = m_servers.at(i);
		if(!svr.serialize(so)) return -1;
	}

	return so->size() - pos;
}

BOOL KServerAreaSettings::Area::deserialize(KMemoryStream* si)
{
	this->clear();
	
	short n;
	if(!si->ReadInt(m_areaID)) return FALSE;
	if(!si->ReadString8(m_areaName, sizeof(m_areaName))) return FALSE;
	if(!si->ReadShort(n)) return FALSE;

	KServerAreaSettings::Server svr;
	for(int i=0; i<(int)n; i++)
	{
		if(!svr.deserialize(si)) return FALSE;
		m_servers.push_back(svr);
	}

	return TRUE;
}

//// KServerAreaSettings ////

KServerAreaSettings::KServerAreaSettings()
{

}

KServerAreaSettings::~KServerAreaSettings()
{

}

BOOL KServerAreaSettings::load(StreamInterface* si)
{
	this->clear();

	XMLDomParser parser;
	XMLElementNode xmlRoot;
	if(!parser.Parse(*si, xmlRoot))
	{
		Log(LOG_ERROR|LOG_CONSOLE, "error: parse LoginSereverList.xml %s", parser.GetErrorMsg());
		return FALSE;
	}

	Area area;
	XMLNodeColl coll = xmlRoot.QueryChildren2("Settings/Area");
	int n = coll.Size();
	for(int i=0; i<n; i++)
	{
		area.clear();
		XMLElementNode* areaXml = (XMLElementNode*)coll.GetNode(i);
		if(!area.load(areaXml))
			return FALSE;
		m_areaMap[area.m_areaID] = area;
	}

	return TRUE;
}

void KServerAreaSettings::clear()
{
	m_areaMap.clear();
}

bool KServerAreaSettings::foreach_area(KServerAreaSettings::IVisitor& visitor)
{
	AreaMap::iterator it = m_areaMap.begin();
	AreaMap::iterator eit = m_areaMap.end();
	for(; it != eit; it++)
	{
		const Area& area = it->second;
		if(!visitor.visit(&area))
			return false;
	}
	return true;
}

int KServerAreaSettings::getAreaCount() const
{
	return m_areaMap.size();
}

const KServerAreaSettings::Area* KServerAreaSettings::getAreaAt(int idx) const
{
	if(idx < 0 || idx >= m_areaMap.size())
		return NULL;
	return &m_areaMap.at(idx)->second;
}

const KServerAreaSettings::Area* KServerAreaSettings::getArea(int areaID) const
{
	AreaMap::const_iterator it = m_areaMap.begin();
	AreaMap::const_iterator eit = m_areaMap.end();
	for(; it != eit; it++)
	{
		const Area& area = it->second;
		if(area.m_areaID == areaID)
			return &area;
	}
	return NULL;
}

const KServerAreaSettings::Server* KServerAreaSettings::findServerByWorldAddress(const KSocketAddress& publicAddr) const
{
	int n = m_areaMap.size();
	for(int i=0; i<n; i++)
	{
		const Area& area = m_areaMap.at(i)->second;
		int m = area.getServerCount();
		for(int k=0; k<m; k++)
		{
			const Server* server = area.getServerAt(k);
			const Service* service = server->getService("world");
			if(!service) continue;
			if(service->m_publicAddr == publicAddr)
				return server;
		}
	}
	return NULL;
}

int KServerAreaSettings::findGatewayServices(const KSocketAddress& worldPublicAddr, KGatewayNode* gwNodes, int count) const
{
	const Server* server = this->findServerByWorldAddress(worldPublicAddr);
	if(!server) return 0;

	const Service* services[128];
	int n = server->getServices("gateway", services, 128);
	if(n < 1) return 0;

	for(int i=0; i<n && i<count; i++)
	{
		const Service* service = services[i];
		KGatewayNode& gwnode = gwNodes[i];
		memset(&gwnode, 0, sizeof(gwnode));
		gwnode.id = 0;
		gwnode.publicIp = service->m_publicAddr.m_ip;
		gwnode.publicPort = service->m_publicAddr.m_port;
		gwnode.privateIp = service->m_privateAddr.m_ip;
		gwnode.privatePort = service->m_privateAddr.m_port;
		//gwnode.count = (WORD)service->m_instanceCount;
	}

	return n;
}

size_t KServerAreaSettings::serialize(KMemoryStream* so)
{
	size_t pos = so->size();
	int n = m_areaMap.size();
	if(!so->WriteShort((short)n)) return -1;
	for(int i=0; i<n; i++)
	{
		Area& area = m_areaMap.at(i)->second;
		if(!area.serialize(so)) return -1;
	}
	return so->size() - pos;
}

BOOL KServerAreaSettings::deserialize(KMemoryStream* si)
{
	short n;
	Area area;

	this->clear();
	if(!si->ReadShort(n)) return FALSE;

	for(int i=0; i<(int)n; i++)
	{
		if(!area.deserialize(si)) return FALSE;
		m_areaMap[area.m_areaID] = area;
	}

	return TRUE;
}