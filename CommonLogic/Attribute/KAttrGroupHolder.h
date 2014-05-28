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
	 * �������д��ڵ����ԵĽӿ�
	 */
	class IAllExistAttrVisitor
	{
	public:
		virtual void Visit(int idx) = 0;
		~IAllExistAttrVisitor(){};
	};
	/**
	 * ���Է��ʽӿ�
	 */
	class IAttrGroupHolder
	{
	public:
		virtual ~IAttrGroupHolder(){ }

	public:
		virtual void Reset() = 0;
		// �������ֵ
		virtual int GetAttrValue(int idx) const = 0;
		// �趨����ֵ
		virtual void SetAttrValue(int idx, int val, int param) = 0;
		// ��������ֵ
		virtual void IncreaseAttrValue(int idx, int val, int param) = 0;
		// �������д��ڵ�����
		virtual void VisitAllExistAttr(IAllExistAttrVisitor& visitor) = 0;
	};

	/**
	 * ��ɫ������
	 */
	class KCharAttrGroupHolder
		: public IAttrGroupHolder
		, public System::Memory::KPortableMemoryPool<KCharAttrGroupHolder, 1024>
	{
	public:
		//��������������������2��
		const static int MAX_ATTR_GROUP = 2;

	public:
		KCharAttrGroupHolder();

	public:
		virtual void Reset();

		// �õ�ָ��������ֵ
		virtual int GetAttrValue(int idx) const;

		// �趨����ֵ
		virtual void SetAttrValue(int idx, int val, int param);
		// ��������ֵ
		virtual void IncreaseAttrValue(int idx, int val, int param);
		// �������д��ڵ�����
		virtual void VisitAllExistAttr(IAllExistAttrVisitor& visitor);

	protected:
		// ��¼���е�������
		KAttributeAbout::IAttrHolder* m_attrHolderList[MAX_ATTR_GROUP];
		// ��¼��1������(�޹�ʽ)
		KAttributeAbout::KCommonAttrHolder<KAttributeAbout::part0_ca1_end - KAttributeAbout::part0_ca1_start, KAttributeAbout::part0_ca1_start> m_commonAttrHolder;
		// ��¼��2������(2�����Ĺ�ʽ)
		KAttributeAbout::K2ParamAttrHolder<KAttributeAbout::part0_ca2_end - KAttributeAbout::part0_ca2_start, KAttributeAbout::part0_ca2_start> m_2ParamAttrHolder;
	};

	/**
	 * ͨ�����Դ洢
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
		// �õ�ָ��������ֵ
		virtual int GetAttrValue(int idx) const;
		// �趨����ֵ
		virtual void SetAttrValue(int idx, int val, int param);
		// ��������ֵ
		virtual void IncreaseAttrValue(int idx, int val, int param);
		// �������д��ڵ�����
		virtual void VisitAllExistAttr(IAllExistAttrVisitor& visitor);
	protected:
		typedef System::Collections::KMapByVector<int, int> HOLDER_TYPE;
		HOLDER_TYPE m_attrHolder;
	};

	/**
	 * ���������
	 */
	class KPlayerAttrGroupHolder
		: public IAttrGroupHolder
		, public System::Memory::KPortableMemoryPool<KPlayerAttrGroupHolder, 1024>
	{
	public:
		//��������������������2��
		const static int MAX_ATTR_GROUP = 3;

	public:
		KPlayerAttrGroupHolder();
		virtual ~KPlayerAttrGroupHolder();

	public:
		virtual void Reset();

		// �õ�ָ��������ֵ
		virtual int GetAttrValue(int idx) const;

		// �趨����ֵ
		virtual void SetAttrValue(int idx, int val, int param);
		// ��������ֵ
		virtual void IncreaseAttrValue(int idx, int val, int param);
		// �������д��ڵ�����
		virtual void VisitAllExistAttr(IAllExistAttrVisitor& visitor);

	protected:
		KCharAttrGroupHolder * m_pCharAttrGroupHolder;

		// ��¼���е�������
		KAttributeAbout::IAttrHolder* m_attrHolderList[MAX_ATTR_GROUP];
		// ��¼��1������(�޹�ʽ)
		KAttributeAbout::KCommonAttrHolder<KAttributeAbout::part1_ca1_end - KAttributeAbout::part1_ca1_start, KAttributeAbout::part1_ca1_start> m_PlayerAttrHolder;
		// ��¼��2������(2�����Ĺ�ʽ)
		KAttributeAbout::K2ParamAttrHolder<KAttributeAbout::part1_ca2_end - KAttributeAbout::part1_ca2_start, KAttributeAbout::part1_ca2_start> m_PlayerParamAttrHolder;
		// ��¼��3������(�޹�ʽ���߻��Զ���ǿ����)
		KAttributeAbout::KCommonAttrHolder<KAttributeAbout::part1_ca3_end - KAttributeAbout::part1_ca3_start, KAttributeAbout::part1_ca3_start> m_PlayerCustomAttrHolder;
	};

	/**
	 * WorldPlayer������
	 */
	class KWorldPlayerAttrHolder
		: public IAttrGroupHolder
		, public System::Memory::KPortableMemoryPool<KWorldPlayerAttrHolder, 1024>
	{
	public:
		//��������������������2��
		const static int MAX_ATTR_GROUP = 1;

	public:
		KWorldPlayerAttrHolder();

	public:
		virtual void Reset();

		// �õ�ָ��������ֵ
		virtual int GetAttrValue(int idx) const;

		// �趨����ֵ
		virtual void SetAttrValue(int idx, int val, int param);
		// ��������ֵ
		virtual void IncreaseAttrValue(int idx, int val, int param);
		// �������д��ڵ�����
		virtual void VisitAllExistAttr(IAllExistAttrVisitor& visitor);

	protected:
		// ��¼���е�������
		KAttributeAbout::IAttrHolder* m_attrHolderList[MAX_ATTR_GROUP];
		// ��¼��1������(�޹�ʽ)
		KAttributeAbout::KCommonAttrHolder<KAttributeAbout::eWPA_Count - KAttributeAbout::eWPA_Undefine, KAttributeAbout::eWPA_Undefine> m_WorldPlayerAttrHolder;
	};

	/**
	 * NPC������
	 */
	class KNpcAttrGroupHolder
		: public IAttrGroupHolder
		, public System::Memory::KPortableMemoryPool<KNpcAttrGroupHolder, 1024>
	{
	public:
		//��������������������2��
		const static int MAX_ATTR_GROUP = 2;

	public:
		KNpcAttrGroupHolder();
		~KNpcAttrGroupHolder();

	public:
		virtual void Reset();

		// �õ�ָ��������ֵ
		virtual int GetAttrValue(int idx) const;

		// �趨����ֵ
		virtual void SetAttrValue(int idx, int val, int param);
		// ��������ֵ
		virtual void IncreaseAttrValue(int idx, int val, int param);
		// �������д��ڵ�����
		virtual void VisitAllExistAttr(IAllExistAttrVisitor& visitor);

	protected:
		KCharAttrGroupHolder * m_pCharAttrGroupHolder;

		// ��¼���е�������
		KAttributeAbout::IAttrHolder* m_attrHolderList[MAX_ATTR_GROUP];
		// ��¼��1������(�޹�ʽ)
		KAttributeAbout::KCommonAttrHolder<KAttributeAbout::part2_ca1_end - KAttributeAbout::part2_ca1_start, KAttributeAbout::part2_ca1_start> m_NpcAttrHolder;
		// ��¼��2������(2�����Ĺ�ʽ)
		KAttributeAbout::K2ParamAttrHolder<KAttributeAbout::part2_ca2_end - KAttributeAbout::part2_ca2_start, KAttributeAbout::part2_ca2_start> m_NpcParamAttrHolder;
	};

	/**
	 * BattlePet������
	 */
	class KBattlePetAttrGroupHolder
		: public IAttrGroupHolder
		, public System::Memory::KPortableMemoryPool<KBattlePetAttrGroupHolder, 1024>
	{
	public:
		//��������������������2��
		const static int MAX_ATTR_GROUP = 2;

	public:
		KBattlePetAttrGroupHolder();
		virtual ~KBattlePetAttrGroupHolder();

	public:
		virtual void Reset();

		// �õ�ָ��������ֵ
		virtual int GetAttrValue(int idx) const;

		// �趨����ֵ
		virtual void SetAttrValue(int idx, int val, int param);
		// ��������ֵ
		virtual void IncreaseAttrValue(int idx, int val, int param);
		// �������д��ڵ�����
		virtual void VisitAllExistAttr(IAllExistAttrVisitor& visitor);

	protected:
		KCharAttrGroupHolder * m_pCharAttrGroupHolder;

		// ��¼���е�������
		KAttributeAbout::IAttrHolder* m_attrHolderList[MAX_ATTR_GROUP];
		// ��¼��1������(�޹�ʽ)
		KAttributeAbout::KCommonAttrHolder<KAttributeAbout::part3_ca1_end - KAttributeAbout::part3_ca1_start, KAttributeAbout::part3_ca1_start> m_BettlePetAttrHolder;
		// ��¼��2������(2�����Ĺ�ʽ)
		KAttributeAbout::K2ParamAttrHolder<KAttributeAbout::part3_ca2_end - KAttributeAbout::part3_ca2_start, KAttributeAbout::part3_ca2_start> m_BettlePetParamAttrHolder;
	};

	/**
	 * Building������
	 */
	class KBuildingAttrGroupHolder
		: public IAttrGroupHolder
		, public System::Memory::KPortableMemoryPool<KBuildingAttrGroupHolder, 1024>
	{
	public:
		//��������������������2��
		const static int MAX_ATTR_GROUP = 2;

	public:
		KBuildingAttrGroupHolder();

	public:
		virtual void Reset();

		// �õ�ָ��������ֵ
		virtual int GetAttrValue(int idx) const;

		// �趨����ֵ
		virtual void SetAttrValue(int idx, int val, int param);
		// ��������ֵ
		virtual void IncreaseAttrValue(int idx, int val, int param);
		// �������д��ڵ�����
		virtual void VisitAllExistAttr(IAllExistAttrVisitor& visitor);

	protected:
		// ��¼���е�������
		KAttributeAbout::IAttrHolder* m_attrHolderList[MAX_ATTR_GROUP];
		// ��¼��1������(�޹�ʽ)
		KAttributeAbout::KCommonAttrHolder<KAttributeAbout::part4_ca1_end - KAttributeAbout::part4_ca1_start, KAttributeAbout::part4_ca1_start> m_commonAttrHolder;
		// ��¼��2������(2�����Ĺ�ʽ)
		KAttributeAbout::K2ParamAttrHolder<KAttributeAbout::part4_ca2_end - KAttributeAbout::part4_ca2_start, KAttributeAbout::part4_ca2_start> m_2ParamAttrHolder;
		// ��¼�ȼ�����
		int m_level;
		// ��¼size
		int m_size;
	};


	/**
	 * ��ͨ����������
	 */
	class KTransportAttrGroupHolder
		: public IAttrGroupHolder
		, public System::Memory::KPortableMemoryPool<KTransportAttrGroupHolder, 1024>
	{
	public:
		//��������������������2��
		const static int MAX_ATTR_GROUP = 2;

	public:
		KTransportAttrGroupHolder();
		virtual ~KTransportAttrGroupHolder();

	public:
		virtual void Reset();

		// �õ�ָ��������ֵ
		virtual int GetAttrValue(int idx) const;

		// �趨����ֵ
		virtual void SetAttrValue(int idx, int val, int param);
		// ��������ֵ
		virtual void IncreaseAttrValue(int idx, int val, int param);
		// �������д��ڵ�����
		virtual void VisitAllExistAttr(IAllExistAttrVisitor& visitor);

	protected:
		KCharAttrGroupHolder * m_pCharAttrGroupHolder;

		// ��¼���е�������
		KAttributeAbout::IAttrHolder* m_attrHolderList[MAX_ATTR_GROUP];
		// ��¼��1������(�޹�ʽ)
		KAttributeAbout::KCommonAttrHolder<KAttributeAbout::part5_ca1_end - KAttributeAbout::part5_ca1_start, KAttributeAbout::part5_ca1_start> m_commonAttrHolder;
		// ��¼��2������(2�����Ĺ�ʽ)
		KAttributeAbout::K2ParamAttrHolder<KAttributeAbout::part5_ca2_end - KAttributeAbout::part5_ca2_start, KAttributeAbout::part5_ca2_start> m_2ParamAttrHolder;
	};

	/**
	 * �ӵ�������
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

		// �õ�ָ��������ֵ
		virtual int GetAttrValue(int idx) const							{ return 0; }

		// �趨����ֵ
		virtual void SetAttrValue(int idx, int val, int param)			{}
		// ��������ֵ
		virtual void IncreaseAttrValue(int idx, int val, int param)		{}
		// �������д��ڵ�����
		virtual void VisitAllExistAttr(IAllExistAttrVisitor& visitor)	{}
	};
	
	/**
	 * ����ս��������
	 */
	class KTribeBattlePetAttrGroupHolder
		: public IAttrGroupHolder
		, public System::Memory::KPortableMemoryPool<KTribeBattlePetAttrGroupHolder, 1024>
	{
	public:
		//��������������������2��
		const static int MAX_ATTR_GROUP = 2;

	public:
		KTribeBattlePetAttrGroupHolder();
		virtual ~KTribeBattlePetAttrGroupHolder();

	public:
		virtual void Reset();

		// �õ�ָ��������ֵ
		virtual int GetAttrValue(int idx) const;

		// �趨����ֵ
		virtual void SetAttrValue(int idx, int val, int param);
		// ��������ֵ
		virtual void IncreaseAttrValue(int idx, int val, int param);
		// �������д��ڵ�����
		virtual void VisitAllExistAttr(IAllExistAttrVisitor& visitor);

	protected:
		KCharAttrGroupHolder * m_pCharAttrGroupHolder;

		// ��¼���е�������
		KAttributeAbout::IAttrHolder* m_attrHolderList[MAX_ATTR_GROUP];
		// ��¼��1������(�޹�ʽ)
		KAttributeAbout::KCommonAttrHolder<KAttributeAbout::part6_ca1_end - KAttributeAbout::part6_ca1_start, KAttributeAbout::part6_ca1_start> m_TribeBettlePetAttrHolder;
		// ��¼��2������(2�����Ĺ�ʽ)
		KAttributeAbout::K2ParamAttrHolder<KAttributeAbout::part6_ca2_end - KAttributeAbout::part6_ca2_start, KAttributeAbout::part6_ca2_start> m_TribeBettlePetParamAttrHolder;
	};

}