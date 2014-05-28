/********************************************************************
created:	2011/3/10
manager:	Allen Wang
copyright:	Aurora Interactive, 2011
*********************************************************************/

#pragma once
#include "KAttrDefine.h"
#include "KAttrHolder.h"
#include "System/Collections/DynArray.h"
#include "System/Memory/KStepObjectPool.h"
#include "System/Collections/KMapByVector.h"
#include "System/Patterns/KSingleton.h"

namespace KAttributeAbout
{
	/**
	 * 遍历所有存在的属性的接口
	 */
	class IAllExistAttrVisitor
	{
	public:
		virtual void Visit(int idx) = 0;
		~IAllExistAttrVisitor(){};
	};
	/**
	 * 属性访问接口
	 */
	class IAttrGroupHolder
	{
	public:
		virtual ~IAttrGroupHolder(){ }

	public:
		virtual void Reset() = 0;
		// 获得属性值
		virtual int GetAttrValue(int idx) const = 0;
		// 设定属性值
		virtual void SetAttrValue(int idx, int val, int param) = 0;
		// 增加属性值
		virtual void IncreaseAttrValue(int idx, int val, int param) = 0;
		// 访问所有存在的属性
		virtual void VisitAllExistAttr(IAllExistAttrVisitor& visitor) = 0;
	};

	/**
	 * 角色属性组
	 */
	class KCharAttrGroupHolder
		: public IAttrGroupHolder
		, public System::Memory::KPortableMemoryPool<KCharAttrGroupHolder, 1024>
	{
	public:
		//最大属性组个数（不超过2）
		const static int MAX_ATTR_GROUP = 2;

	public:
		KCharAttrGroupHolder();

	public:
		virtual void Reset();

		// 得到指定的属性值
		virtual int GetAttrValue(int idx) const;

		// 设定属性值
		virtual void SetAttrValue(int idx, int val, int param);
		// 增加属性值
		virtual void IncreaseAttrValue(int idx, int val, int param);
		// 访问所有存在的属性
		virtual void VisitAllExistAttr(IAllExistAttrVisitor& visitor);

	protected:
		// 记录所有的属性组
		KAttributeAbout::IAttrHolder* m_attrHolderList[MAX_ATTR_GROUP];
		// 记录第1组属性(无公式)
		KAttributeAbout::KCommonAttrHolder<KAttributeAbout::part0_ca1_end - KAttributeAbout::part0_ca1_start, KAttributeAbout::part0_ca1_start> m_commonAttrHolder;
		// 记录第2组属性(2参数的公式)
		KAttributeAbout::K2ParamAttrHolder<KAttributeAbout::part0_ca2_end - KAttributeAbout::part0_ca2_start, KAttributeAbout::part0_ca2_start> m_2ParamAttrHolder;
	};

	/**
	 * 通用属性存储
	 */
	class KCommonAttrGroupHolder
		: public IAttrGroupHolder
		, public System::Memory::KPortableMemoryPool<KCommonAttrGroupHolder, 256>
	{
	public:
		KCommonAttrGroupHolder();
		~KCommonAttrGroupHolder();
	public:
		virtual void Reset();
		// 得到指定的属性值
		virtual int GetAttrValue(int idx) const;
		// 设定属性值
		virtual void SetAttrValue(int idx, int val, int param);
		// 增加属性值
		virtual void IncreaseAttrValue(int idx, int val, int param);
		// 访问所有存在的属性
		virtual void VisitAllExistAttr(IAllExistAttrVisitor& visitor);
	protected:
		typedef System::Collections::KMapByVector<int, int> HOLDER_TYPE;
		HOLDER_TYPE m_attrHolder;
	};

	/**
	 * 玩家属性组
	 */
	class KPlayerAttrGroupHolder
		: public IAttrGroupHolder
		, public System::Memory::KPortableMemoryPool<KPlayerAttrGroupHolder, 1024>
	{
	public:
		//最大属性组个数（不超过2）
		const static int MAX_ATTR_GROUP = 3;

	public:
		KPlayerAttrGroupHolder();
		virtual ~KPlayerAttrGroupHolder();

	public:
		virtual void Reset();

		// 得到指定的属性值
		virtual int GetAttrValue(int idx) const;

		// 设定属性值
		virtual void SetAttrValue(int idx, int val, int param);
		// 增加属性值
		virtual void IncreaseAttrValue(int idx, int val, int param);
		// 访问所有存在的属性
		virtual void VisitAllExistAttr(IAllExistAttrVisitor& visitor);

	protected:
		KCharAttrGroupHolder * m_pCharAttrGroupHolder;

		// 记录所有的属性组
		KAttributeAbout::IAttrHolder* m_attrHolderList[MAX_ATTR_GROUP];
		// 记录第1组属性(无公式)
		KAttributeAbout::KCommonAttrHolder<KAttributeAbout::part1_ca1_end - KAttributeAbout::part1_ca1_start, KAttributeAbout::part1_ca1_start> m_PlayerAttrHolder;
		// 记录第2组属性(2参数的公式)
		KAttributeAbout::K2ParamAttrHolder<KAttributeAbout::part1_ca2_end - KAttributeAbout::part1_ca2_start, KAttributeAbout::part1_ca2_start> m_PlayerParamAttrHolder;
		// 记录第3组属性(无公式，策划自定义强化用)
		KAttributeAbout::KCommonAttrHolder<KAttributeAbout::part1_ca3_end - KAttributeAbout::part1_ca3_start, KAttributeAbout::part1_ca3_start> m_PlayerCustomAttrHolder;
	};

	/**
	 * WorldPlayer属性组
	 */
	class KWorldPlayerAttrHolder
		: public IAttrGroupHolder
		, public System::Memory::KPortableMemoryPool<KWorldPlayerAttrHolder, 1024>
	{
	public:
		//最大属性组个数（不超过2）
		const static int MAX_ATTR_GROUP = 1;

	public:
		KWorldPlayerAttrHolder();

	public:
		virtual void Reset();

		// 得到指定的属性值
		virtual int GetAttrValue(int idx) const;

		// 设定属性值
		virtual void SetAttrValue(int idx, int val, int param);
		// 增加属性值
		virtual void IncreaseAttrValue(int idx, int val, int param);
		// 访问所有存在的属性
		virtual void VisitAllExistAttr(IAllExistAttrVisitor& visitor);

	protected:
		// 记录所有的属性组
		KAttributeAbout::IAttrHolder* m_attrHolderList[MAX_ATTR_GROUP];
		// 记录第1组属性(无公式)
		KAttributeAbout::KCommonAttrHolder<KAttributeAbout::eWPA_Count - KAttributeAbout::eWPA_Undefine, KAttributeAbout::eWPA_Undefine> m_WorldPlayerAttrHolder;
	};

	/**
	 * NPC属性组
	 */
	class KNpcAttrGroupHolder
		: public IAttrGroupHolder
		, public System::Memory::KPortableMemoryPool<KNpcAttrGroupHolder, 1024>
	{
	public:
		//最大属性组个数（不超过2）
		const static int MAX_ATTR_GROUP = 2;

	public:
		KNpcAttrGroupHolder();
		~KNpcAttrGroupHolder();

	public:
		virtual void Reset();

		// 得到指定的属性值
		virtual int GetAttrValue(int idx) const;

		// 设定属性值
		virtual void SetAttrValue(int idx, int val, int param);
		// 增加属性值
		virtual void IncreaseAttrValue(int idx, int val, int param);
		// 访问所有存在的属性
		virtual void VisitAllExistAttr(IAllExistAttrVisitor& visitor);

	protected:
		KCharAttrGroupHolder * m_pCharAttrGroupHolder;

		// 记录所有的属性组
		KAttributeAbout::IAttrHolder* m_attrHolderList[MAX_ATTR_GROUP];
		// 记录第1组属性(无公式)
		KAttributeAbout::KCommonAttrHolder<KAttributeAbout::part2_ca1_end - KAttributeAbout::part2_ca1_start, KAttributeAbout::part2_ca1_start> m_NpcAttrHolder;
		// 记录第2组属性(2参数的公式)
		KAttributeAbout::K2ParamAttrHolder<KAttributeAbout::part2_ca2_end - KAttributeAbout::part2_ca2_start, KAttributeAbout::part2_ca2_start> m_NpcParamAttrHolder;
	};

	/**
	 * BattlePet属性组
	 */
	class KBattlePetAttrGroupHolder
		: public IAttrGroupHolder
		, public System::Memory::KPortableMemoryPool<KBattlePetAttrGroupHolder, 1024>
	{
	public:
		//最大属性组个数（不超过2）
		const static int MAX_ATTR_GROUP = 2;

	public:
		KBattlePetAttrGroupHolder();
		virtual ~KBattlePetAttrGroupHolder();

	public:
		virtual void Reset();

		// 得到指定的属性值
		virtual int GetAttrValue(int idx) const;

		// 设定属性值
		virtual void SetAttrValue(int idx, int val, int param);
		// 增加属性值
		virtual void IncreaseAttrValue(int idx, int val, int param);
		// 访问所有存在的属性
		virtual void VisitAllExistAttr(IAllExistAttrVisitor& visitor);

	protected:
		KCharAttrGroupHolder * m_pCharAttrGroupHolder;

		// 记录所有的属性组
		KAttributeAbout::IAttrHolder* m_attrHolderList[MAX_ATTR_GROUP];
		// 记录第1组属性(无公式)
		KAttributeAbout::KCommonAttrHolder<KAttributeAbout::part3_ca1_end - KAttributeAbout::part3_ca1_start, KAttributeAbout::part3_ca1_start> m_BettlePetAttrHolder;
		// 记录第2组属性(2参数的公式)
		KAttributeAbout::K2ParamAttrHolder<KAttributeAbout::part3_ca2_end - KAttributeAbout::part3_ca2_start, KAttributeAbout::part3_ca2_start> m_BettlePetParamAttrHolder;
	};

	/**
	 * Building属性组
	 */
	class KBuildingAttrGroupHolder
		: public IAttrGroupHolder
		, public System::Memory::KPortableMemoryPool<KBuildingAttrGroupHolder, 1024>
	{
	public:
		//最大属性组个数（不超过2）
		const static int MAX_ATTR_GROUP = 2;

	public:
		KBuildingAttrGroupHolder();

	public:
		virtual void Reset();

		// 得到指定的属性值
		virtual int GetAttrValue(int idx) const;

		// 设定属性值
		virtual void SetAttrValue(int idx, int val, int param);
		// 增加属性值
		virtual void IncreaseAttrValue(int idx, int val, int param);
		// 访问所有存在的属性
		virtual void VisitAllExistAttr(IAllExistAttrVisitor& visitor);

	protected:
		// 记录所有的属性组
		KAttributeAbout::IAttrHolder* m_attrHolderList[MAX_ATTR_GROUP];
		// 记录第1组属性(无公式)
		KAttributeAbout::KCommonAttrHolder<KAttributeAbout::part4_ca1_end - KAttributeAbout::part4_ca1_start, KAttributeAbout::part4_ca1_start> m_commonAttrHolder;
		// 记录第2组属性(2参数的公式)
		KAttributeAbout::K2ParamAttrHolder<KAttributeAbout::part4_ca2_end - KAttributeAbout::part4_ca2_start, KAttributeAbout::part4_ca2_start> m_2ParamAttrHolder;
		// 记录等级属性
		int m_level;
		// 记录size
		int m_size;
	};


	/**
	 * 交通工具属性组
	 */
	class KTransportAttrGroupHolder
		: public IAttrGroupHolder
		, public System::Memory::KPortableMemoryPool<KTransportAttrGroupHolder, 1024>
	{
	public:
		//最大属性组个数（不超过2）
		const static int MAX_ATTR_GROUP = 2;

	public:
		KTransportAttrGroupHolder();
		virtual ~KTransportAttrGroupHolder();

	public:
		virtual void Reset();

		// 得到指定的属性值
		virtual int GetAttrValue(int idx) const;

		// 设定属性值
		virtual void SetAttrValue(int idx, int val, int param);
		// 增加属性值
		virtual void IncreaseAttrValue(int idx, int val, int param);
		// 访问所有存在的属性
		virtual void VisitAllExistAttr(IAllExistAttrVisitor& visitor);

	protected:
		KCharAttrGroupHolder * m_pCharAttrGroupHolder;

		// 记录所有的属性组
		KAttributeAbout::IAttrHolder* m_attrHolderList[MAX_ATTR_GROUP];
		// 记录第1组属性(无公式)
		KAttributeAbout::KCommonAttrHolder<KAttributeAbout::part5_ca1_end - KAttributeAbout::part5_ca1_start, KAttributeAbout::part5_ca1_start> m_commonAttrHolder;
		// 记录第2组属性(2参数的公式)
		KAttributeAbout::K2ParamAttrHolder<KAttributeAbout::part5_ca2_end - KAttributeAbout::part5_ca2_start, KAttributeAbout::part5_ca2_start> m_2ParamAttrHolder;
	};

	/**
	 * 子弹属性组
	 */
	class KDummyAttrGroupHolder
		: public IAttrGroupHolder
		, public System::Patterns::KSingleton<KDummyAttrGroupHolder>
	{
	public:
		KDummyAttrGroupHolder()				{}
		virtual ~KDummyAttrGroupHolder()	{}

	public:
		virtual void Reset()				{}

		// 得到指定的属性值
		virtual int GetAttrValue(int idx) const							{ return 0; }

		// 设定属性值
		virtual void SetAttrValue(int idx, int val, int param)			{}
		// 增加属性值
		virtual void IncreaseAttrValue(int idx, int val, int param)		{}
		// 访问所有存在的属性
		virtual void VisitAllExistAttr(IAllExistAttrVisitor& visitor)	{}
	};
	
	/**
	 * 部落战兽属性组
	 */
	class KTribeBattlePetAttrGroupHolder
		: public IAttrGroupHolder
		, public System::Memory::KPortableMemoryPool<KTribeBattlePetAttrGroupHolder, 1024>
	{
	public:
		//最大属性组个数（不超过2）
		const static int MAX_ATTR_GROUP = 2;

	public:
		KTribeBattlePetAttrGroupHolder();
		virtual ~KTribeBattlePetAttrGroupHolder();

	public:
		virtual void Reset();

		// 得到指定的属性值
		virtual int GetAttrValue(int idx) const;

		// 设定属性值
		virtual void SetAttrValue(int idx, int val, int param);
		// 增加属性值
		virtual void IncreaseAttrValue(int idx, int val, int param);
		// 访问所有存在的属性
		virtual void VisitAllExistAttr(IAllExistAttrVisitor& visitor);

	protected:
		KCharAttrGroupHolder * m_pCharAttrGroupHolder;

		// 记录所有的属性组
		KAttributeAbout::IAttrHolder* m_attrHolderList[MAX_ATTR_GROUP];
		// 记录第1组属性(无公式)
		KAttributeAbout::KCommonAttrHolder<KAttributeAbout::part6_ca1_end - KAttributeAbout::part6_ca1_start, KAttributeAbout::part6_ca1_start> m_TribeBettlePetAttrHolder;
		// 记录第2组属性(2参数的公式)
		KAttributeAbout::K2ParamAttrHolder<KAttributeAbout::part6_ca2_end - KAttributeAbout::part6_ca2_start, KAttributeAbout::part6_ca2_start> m_TribeBettlePetParamAttrHolder;
	};

}