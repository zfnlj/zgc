#ifndef _KRANKSTATIC_H
#define _KRANKSTATIC_H

#include "System/Patterns/KSingleton.h"
#include <System/Collections/DynArray.h>
#include "System/Collections/KMapByVector.h"
#include "System/Collections/KHashTable.h"
#include <list>
#include "../common/KCommonObj.h"
#include <System/File/KTabfileLoader.h>

class KRankStaticData
	: public ::System::Memory::KPortableMemoryPool<KRankStaticData>
{
public:
	int m_level;		// 对应等级
	int m_exp;			// 等级对应的exp上限

};


class KRankStaticDataManager
{
	public:
		// 获得指定id的全局参数
		const KRankStaticData* operator[](int idx) const;
		// 获得指定id的全局参数
		const KRankStaticData* Get(int idx) const;

		// 初始化
		bool Init(System::File::KTabFile2* fileReader);
		// 从文件加载全局参数
		virtual bool Reload(System::File::KTabFile2* fileReader);
		// 根据当前的exp计算等级
		int ExpToLevel(int exp);
		// 得到等级对应的字符串描述
		const char* GetLevelDesp(int level);
	private:
		void Insert(const KRankStaticData& data);

		typedef ::System::Collections::KHashTable<int
			, KRankStaticData*
			, KIntCompare<int>
			, KHasher<int>
			, ::System::Sync::KMTLock
			, 32
			, 32> DATA_MAP_TYPE;
		DATA_MAP_TYPE m_dataMap;

		struct ELEM_DATA
		{
			int m_level;
			int m_exp;
			bool operator <(const ELEM_DATA& o) const
			{
				return m_exp < o.m_exp;
			}
		};
		class KSortedVectorWithKey : public ::System::Collections::KSortedVector<ELEM_DATA>
		{
		public:
			/**
			 * 根据Key查找元素。
			 * param:
			 *		key:	主键
			 *		bFound:	是否找到
			 * return:
			 *		返回要插入的位置。
			 */
			int find(const int& key, BOOL& bFound) const
			{
				ELEM_DATA ed;
				ed.m_exp = key;
				return this->bsearch(ed, bFound);
			}
		};
		typedef KSortedVectorWithKey EXP_TO_LEVEL_MAP;
		EXP_TO_LEVEL_MAP m_expToLevelMap;
};


#endif // __HELLOWORLD_SCENE_H__
