#pragma once
#include <System/tinyxml/xmldom.h>
#include <System/Collections/KVector.h>
#include <System/Collections/KMapByVector.h>
#include <SockServer/KSocketBase.h>
#include "Inc/PassportLib/GlobalDefine.h"
#include "KGatewaySetting.h"

class KServerAreaSettings
{
public:
	class Service
	{
	public:
		char m_serviceName[64];
		KSocketAddress m_publicAddr;
		KSocketAddress m_privateAddr;
		int m_instanceCount;

	public:
		Service();
		~Service();
		Service(const Service& o);

	public:
		void clear();
		BOOL setName(const char* name);
		BOOL setAddress(const char* address);

	public:
		size_t serialize(KMemoryStream* so);
		BOOL deserialize(KMemoryStream* si);
	};

public:
	class Server
	{
	public:
		typedef System::Collections::KVector<Service> ServciceArray;
		int m_id;
		char m_serverName[64];
		float m_minVer;
		float m_maxVer;
		ServciceArray m_services;
		ENUM_SERVERLIST_SHOW_STATE m_showState;
		int m_playerCount;		// 当前玩家数
		int m_maxPlayerCount;	// 最多容纳多少玩家
		BOOL m_recommented;
		class IVisitor
		{
		public:
			virtual ~IVisitor() { }
			virtual bool visit(const Service* service) = 0;
		};

	public:
		Server();
		Server(const Server& o);
		~Server();
	
	public:
		BOOL load(XMLElementNode* xml);
		void clear();

	public:
		float minVersion() const;
		float maxVersion() const;
		const char* getServername() const { return m_serverName; }

		bool foreach_service(IVisitor& visitor);

		int getServiceCount() const;
		const Service* getServiceAt(int idx) const;
		
		const Service* getService(const char* name) const;
		int getServices(const char* name, const Service** ppService, int count) const;

		size_t serialize(KMemoryStream* so);
		BOOL deserialize(KMemoryStream* si);
	public:
	/*	BeginDefLuaClass(KServerAreaSettings::Server)
			DefMemberVar(m_id)
			DefMemberFunc(getServername)
		EndDef*/
	};

public:
	class Area
	{
	public:
		typedef System::Collections::KVector<Server> ServerArray;
		int m_areaID;
		char m_areaName[64];
		ServerArray m_servers;
		class IVisitor
		{
		public:
			virtual ~IVisitor() { }
			virtual bool visit(const Server* server) = 0;
		};

	public:
		Area();
		Area(const Area& o);
		~Area();

	public:
		BOOL load(XMLElementNode* xml);
		void clear();

	public:
		bool foreach_server(IVisitor& visitor);
		int getServerCount() const;
		const Server* getServerAt(int idx) const;

		const Server* getServer(int serverId) const;
		const Server* getServer(const char* serverName) const;

		size_t serialize(KMemoryStream* so);
		BOOL deserialize(KMemoryStream* si);
	public:
		//BeginDefLuaClass(KServerAreaSettings::Area)
		//	DefMemberFunc(getServerCount)
		//	DefMemberFunc(getServerAt)
		//EndDef
	};

public:
	typedef System::Collections::KMapByVector<int,Area> AreaMap;
	AreaMap m_areaMap;
	class IVisitor
	{
	public:
		virtual ~IVisitor() { }
		virtual bool visit(const Area* area) = 0;
	};

public:
	KServerAreaSettings();
	~KServerAreaSettings();

public:
	BOOL load(StreamInterface* si);
	void clear();

public:
	bool foreach_area(IVisitor& visitor);

	int getAreaCount() const;
	const Area* getAreaAt(int idx) const;

	const Area* getArea(int areaID) const;

	const Server* findServerByWorldAddress(const KSocketAddress& publicAddr) const;
	int findGatewayServices(const KSocketAddress& worldPublicAddr, KGatewayNode* gwNodes, int count) const;

	size_t serialize(KMemoryStream* so);
	BOOL deserialize(KMemoryStream* si);
	//BeginDefLuaClass(KServerAreaSettings)
	//	DefMemberFunc(getArea)
	//EndDef
};