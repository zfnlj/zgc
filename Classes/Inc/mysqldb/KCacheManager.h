#ifndef _K_CACHE_MANAGER_H_
#define _K_CACHE_MANAGER_H_

#include "KTableCache.h"
#include <System/tinyxml/xmldom.h>
#include "KTableCacheTmpl.h"

/*
<Cache>
	<Connection host="localhost" db="test" user="root", password="" />
	<Table name="tblCache">
		<Map file="tblCache.mmap" capacity="1000" />
		<Flush modify="5" delete="5" checkout="5" />
		<Columns>
			<Field name="ObjectID" type="int" />
			<Field name="FirstName" type="string" length="32" default="" />
			<Field name="LastName" type="string" length="32" default="" />
			<Field name="Sex" type="int8" default="0" />
			<Field name="LastModified", type="timestamp", default="now()" />
		</Columns>
	</Table>
</Cache>
*/

class KCacheManager
{
public:
	KCacheManager();
	~KCacheManager();

public:
	BOOL Initialize(const char* xmlCfg, KSQLConnParam& cnnParam, BOOL doFlush, IEmergencyObserver* pObserver=NULL);
	void Finalize();

public:
	const KSQLConnParam& GetConnectionParam() const;
	KTableCache* GetTableCache(const char* tableName);

private:
	BOOL LoadXml(const char* xmlCfg);
	BOOL InitCacheTable(int idx, XMLElementNode* pXml);

public:
	XMLElementNode m_xmlRoot;

private:
	int m_cacheNum;
	KTableCache** m_ppCaches;

	// ¡Ÿ ±”√¥¶
	KSQLConnParam m_cnnParam;
	KSQLConnection m_cnn;
	KSQLTableSchema m_schema;
	BOOL m_doFlush;
	IEmergencyObserver* m_pEmergencyObserver;

public:
	ITableCacheCreator* m_pCacheCreator;
	KVarBinaryMemory m_vbm;
};

#endif
