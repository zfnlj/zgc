#pragma once
#include "System/Collections/KMapByVector.h"
#include "../StaticTable/IStaticTabFile.h"

struct KCoordinatePoint
{
	int m_Id;			// [0,65535]	点的索引	2008-10-31
	int m_Xaxis;		// [0,65535-65535]	点的X轴坐标	2008-10-31
	int m_Yaxis;		// [0,65535-65535]	点的Y轴坐标	2008-10-31
	int m_CDirection;	// [0,360，-1]	摄像机水平朝向	2008-10-31
	int	m_CAngle;		// [0,180，-1]	摄像机垂直角度	2008-10-31
	int m_CFaces;		// [0,5000，-1]	摄像机距离目标点距离	2008-10-31
	int m_RDirection;	// [0,360，-1]	角色的水平朝向	2008-10-31
	int m_MapID;		// 地图ID
	char m_desc[256];	// 点的备注信息不读取	2008-10-31

	int id() const	{ return m_Id;    }
	int x() const	{ return m_Xaxis; }
	int y() const	{ return m_Yaxis; }
	int map() const	{ return m_MapID; }

	//BeginDefLuaClassNoCon(KCoordinatePoint);
	//	DefMemberFunc(id);
	//	DefMemberFunc(x);
	//	DefMemberFunc(y);
	//	DefMemberFunc(map);
	//EndDef;
};

class KCoordinateTable
	: public StaticTabFileAbout::IStaticTabFile
{
public:
	typedef ::System::Collections::KMapByVector<int,KCoordinatePoint> Map;
	typedef Map::iterator Iterator;
	typedef Map::const_iterator ConstIterator;

public:
	KCoordinateTable();
	~KCoordinateTable();

public:
	BOOL Initialize(const char* filename);
	virtual bool Reload(const char * szFilePath);
	void Finalize();

public:
	BOOL Add(const KCoordinatePoint& point);
	BOOL Remove(int pointID);
	const KCoordinatePoint* GetPoint(int pointID) const;

public:
	Map m_map;
};
