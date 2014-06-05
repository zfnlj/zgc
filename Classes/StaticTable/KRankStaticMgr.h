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
	int m_level;		// ��Ӧ�ȼ�
	int m_exp;			// �ȼ���Ӧ��exp����

};


class KRankStaticDataManager
{
	public:
		// ���ָ��id��ȫ�ֲ���
		const KRankStaticData* operator[](int idx) const;
		// ���ָ��id��ȫ�ֲ���
		const KRankStaticData* Get(int idx) const;

		// ��ʼ��
		bool Init(System::File::KTabFile2* fileReader);
		// ���ļ�����ȫ�ֲ���
		virtual bool Reload(System::File::KTabFile2* fileReader);
		// ���ݵ�ǰ��exp����ȼ�
		int ExpToLevel(int exp);
		// �õ��ȼ���Ӧ���ַ�������
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
			 * ����Key����Ԫ�ء�
			 * param:
			 *		key:	����
			 *		bFound:	�Ƿ��ҵ�
			 * return:
			 *		����Ҫ�����λ�á�
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
