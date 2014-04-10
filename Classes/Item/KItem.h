/********************************************************************
created:	2012/02/21
manager:	
copyright:	Aurora Interactive, 2011

*********************************************************************/


#pragma once

#include "System/Collections/KVector.h"
#include "System/Collections/KBitset.h"
#include "System/Collections/KString.h"
#include "System/Misc/KStream.h"
#include "System/Memory/KStepObjectPool.h"
#include "KItemCreate.h"
//#include "luaJIT/KLuaWrap.h"

#define ITEM_CUSTOM_NODE_MAXLEN		40		// �Զ�������buffer��󳤶�
#define MAIL_HEAD_LEN			40
#define MAIL_BODY_LEN			160
namespace KItemAbout
{
	struct KCreateInfo_ItemBase;

	// ��Ʒ���л����
	enum enumItemDbStruct
	{
		enum_attributeList=0,	// ���Զ�
		enum_durable,			// ��ǰ�;�ֵ
		enum_expirationTime,	// ����ʱ��
		enum_frequency,			// ʹ��ʣ�����
		enum_stackNumber,		// ��ǰ���Ӳ���
		enum_author,			// ����
		enum_customAttribute,	// �Զ�������
		enum_binding,			// ����
		enum_mail,				// �ʼ�ת��Ʒ
		enum_customData,		// �Զ�������
		enumIDS_Count
	};
	
	/**
	** ��Ʒ�Զ�����������
	**/
	enum enumItemCustomData
	{
		enum_ICD_None=0,						// ��
		enum_ICD_BattlePet_StrengthLvl,			// ����ǿ�ȵȼ�
		enum_ICD_BattlePet_Quality,				// ����Ʒ��
		
		enum_ICD_Food_MeatID,					// �������ʳ��ID
		enum_ICD_Food_IngredientID1,			// �����������ID1
		enum_ICD_Food_IngredientID2,			// �����������ID2

		enum_ICD_Count
	};

	const int ItemAuthNameLen = 50;		// ��Ʒ������Ϣ�ĳ���
	typedef ::System::Collections::KString<ItemAuthNameLen> ItemAuth;	// ����

	// ��Ʒ���Զ�
	class KItemAttrNode
	{
	public:
		KItemAttrNode();
		virtual ~KItemAttrNode();
		bool operator==(const KItemAttrNode& ItemAttrNode) const;
		bool operator!=(const KItemAttrNode& ItemAttrNode) const;

		bool Serilize(KDataOutputStream& os) const;
		bool Unserilize(KMsgInputStream& si);
		void Clear();

	public:
		int m_nAttrID;
		int m_nAttrValue;
		int m_nParam;
	};

	// ��Ʒ���Զ��б�
	class KItemAttrList
	{
	public:
		KItemAttrList();
		virtual ~KItemAttrList();
		KItemAttrList(const KItemAttrList& ItemAttrList);
		KItemAttrList& operator=(const KItemAttrList& ItemAttrList);
		bool operator==(const KItemAttrList& ItemAttrList) const;
		bool operator!=(const KItemAttrList& ItemAttrList) const;

		int Size() const;
		bool Serilize(KDataOutputStream& os) const;
		bool Unserilize(KMsgInputStream& si);
		bool AddAttr(int nAttrID, int nAttrValue, int nParam);
		void Clear();

		int GetAttrCount() const;
		bool GetAttrInfo(int nIdx, int& nAttrID, int& nAttrValue, int& nParam) const;

	public:
		::System::Collections::KVector<KItemAttrNode> m_vecItemAttrList;	// ���Զ� id, value
	};

	class KItem;

	// �ʼ����
	class KItemMail : public System::Memory::KPortableMemoryPool<KItemMail,1024>
	{
		friend class KItem;
	public:
		KItemMail();
		virtual ~KItemMail();
		KItemMail(const KItemMail& ItemMail);
		KItemMail& operator=(const KItemMail& ItemMail);
		bool operator==(const KItemMail& ItemMail) const;
		bool operator!=(const KItemMail& ItemMail) const;

		bool Serilize(KDataOutputStream& os) const;
		bool Unserilize(KMsgInputStream& si);
		void Clear();

	private:
		::System::Collections::KString<MAIL_HEAD_LEN> m_szTitle;
		::System::Collections::KString<30> m_szTime;
		::System::Collections::KString<MAIL_BODY_LEN> m_szContent;
		::System::Collections::KString<MAX_NAME_DEF> m_szSenderName;
	};

	// �Զ������ݽڵ�
	class KItemCustomDataNode
	{
	public:
		KItemCustomDataNode();
		virtual ~KItemCustomDataNode();
		KItemCustomDataNode(const KItemCustomDataNode& ItemCustomNodeData);
		KItemCustomDataNode& operator=(const KItemCustomDataNode& ItemCustomNodeData);
		bool operator==(const KItemCustomDataNode& ItemCustomNodeData) const;
		bool operator!=(const KItemCustomDataNode& ItemCustomNodeData) const;

		bool Serilize(KDataOutputStream& os) const;
		bool Unserilize(KMsgInputStream& si);
		void Clear();

	public:
		unsigned short m_nFlag;		// ���
		BYTE m_nBuffLen;			// buffer����
		BYTE m_szBuffer[ITEM_CUSTOM_NODE_MAXLEN];		// buffer
	};

	// �Զ�������
	class KItemCustomData : public System::Memory::KPortableMemoryPool<KItemCustomData,1024>
	{
		friend class KItem;
	public:
		KItemCustomData();
		virtual ~KItemCustomData();
		KItemCustomData(const KItemCustomData& ItemCustomData);
		KItemCustomData& operator=(const KItemCustomData& ItemCustomData);
		bool operator==(const KItemCustomData& ItemCustomData) const;
		bool operator!=(const KItemCustomData& ItemCustomData) const;

		bool Serilize(KDataOutputStream& os) const;
		bool Unserilize(KMsgInputStream& si);
		void Clear();

		/**
		** ����Զ�������
		**/
		bool AddCustomData(unsigned short nFlag, const void* pBuffer, int nBuffLen);
		/**
		** �õ���ӵ��Զ�����������
		**/
		int GetSize() const;
		/**
		** �õ�ָ��index���Զ�����������
		**/
		int GetFlag(int nIdx) const;
		/**
		** �õ�ָ��index���Զ�����������
		**/
		const BYTE* GetBuffer(int nIdx, int& nBufferLen) const;
		/**
		** �õ�ָ�����͵��Զ�����������
		**/
		const BYTE* GetBufferByFlag(int nFlag, int& nBufferLen) const;

	private:
		::System::Collections::KVector<KItemCustomDataNode> m_listNode;
	};

	// ��Ʒ��
	class KItem : public System::Memory::KPortableMemoryPool<KItem,1024>
	{
	public:
		KItem();
		virtual ~KItem();
		KItem(const KItem& Item);
		KItem(DWORD id,int stackNum);
		KItem& operator=(const KItem& Item);

		bool operator==(const KItem& Item) const;
		bool operator!=(const KItem& Item) const;

		// ���л�
		bool Serilize(KDataOutputStream& os) const;
		// �����л�
		bool Unserilize(KMsgInputStream& si);
		// Chat���л�
		bool SerilizeChat(char* pBuff, int nBuffLen) const;
		// Chat�����л�
		bool UnserilizeChat(const char* pBuff);
		void Clear();
		bool CheckOneFlag(enumItemDbStruct nFlag) const;
		/**
		** �Ƿ��ܵ���(����������ܵ���)
		** 1���д�����ʱ��Ĳ���
		** 2��װ��һ������
		** 3�����Զ������Բ���
		** 4���ʼ���Ʒ����
		**/
		bool IsStack() const;

		inline DWORD GetID() const
		{
			return m_dwID;
		}

		/**
		** ����һ������
		**/
		bool AddAttr(int nAttrID, int nAttrValue, int nParam);
		bool AddAttrNode(const KItemAttrNode& attrNode);
		/**
		** ɾ����������
		**/
		void ClearAllAttr();
		/**
		** �õ���ǰ���õ���������
		**/
		int GetAttrSize() const;
		/**
		** �õ���ǰ���õ�����ID������ֵ
		**/
		bool GetAttrInfo(int nIdx, int& nAttrID, int& nAttrValue, int& nParam) const;
		/**
		** �õ���ǰ���õ�����ID
		**/
		int GetAttrID(int nIdx) const;
		/**
		** �õ���ǰ���õ�����ֵ
		**/
		int GetAttrValue(int nIdx) const;

		bool SetDurable(USHORT sDurable);
		int GetDurable() const;
		bool SetExpirationTime(UINT nExpirationTime);
		UINT GetExpirationTime() const;
		bool IsFrequency() const;
		bool SetFrequency(USHORT sFrequency);
		USHORT GetFrequency() const;
		bool SetStackNumber(USHORT sStackNumber);
		int GetStackNumber() const;
		bool SetAuthor(const char* pAuthor);
		void ClearAuthor();

		/**
		** ����һ���Զ�������
		**/
		bool AddCustomAttr(int nAttrID, int nAttrValue, int nParam);
		/**
		** ɾ�������Զ�������
		**/
		void DelAllCustomAttr();
		/**
		** �õ���ǰ���õ��Զ�����������
		**/
		int GetCustomAttrSize() const;
		/**
		** �õ���ǰ���õ��Զ�������ID
		**/
		int GetCustomAttrID(int nIdx) const;
		/**
		** �õ���ǰ���õ��Զ�������ֵ
		**/
		int GetCustomAttrValue(int nIdx) const;

		bool SetBinding(bool bBinding);
		bool IsBinding() const;
		/**
		** ��Ʒ����ʼ���Ϣ
		**/
		bool AddMailItem(const char *pName, const char *pTitle, const char *pTime, const char *pContent);
		/**
		** �Ƿ���һ���ʼ���Ʒ
		**/
		bool IsMailItem() const;
		/**
		** �Ƿ���һ��װ��
		**/
		bool IsDeviceItem() const;
		/**
		** �õ��ʼ�������
		**/
		const char* GetMailSenderName() const;
		/**
		** �õ��ʼ�Title
		**/
		const char* GetMailTitle() const;
		/**
		** �õ��ʼ�����ʱ��
		**/
		const char* GetMailTime() const;
		/**
		** �õ��ʼ�����
		**/
		const char* GetMailContent() const;

		/**
		** ���һ���Զ�������
		**/
		bool AddCustomData(unsigned short nFlag, const void* pBuffer, int nBuffLen);
		/**
		** �Ƿ����Զ�������
		**/
		bool IsCustomData() const;
		/**
		** �õ��Զ������ݸ���
		**/
		int GetCustomDataSize() const;
		/**
		** �õ���Ӧ�Զ�����������
		**/
		int GetCustomDataFlag(int nIdx) const;
		/**
		** �õ���Ӧ�Զ�����������
		**/
		const BYTE* GetCustomDataBuffer(int nIdx, int& nBufferLen);
		
		/**
		** ��ȡ��ƷƷ��
		**/
		int GetQuality() const;

		void AddStackNum(int val);
	private:
		void SetOneCheckFlag(enumItemDbStruct nFlag);
		void ClearOneCheckFlag(enumItemDbStruct nFlag);

	public:
		DWORD m_dwID;					// ��ƷID

	private:
		// ��ڴ洢�ṹ
		DWORD m_dwFlag;					// ���Ա��������Щ���ֱ��洢��
		KItemAttrList m_AttrList;		// ���Զ� id, value
		int m_nDurable;					// ��ǰ�;�ֵ(USHORT)
		int m_nExpirationTime;			// ����ʱ��(UINT)
		int m_nFrequency;				// ʹ��ʣ�����(USHORT)
		int m_nStackNumber;				// ��ǰ���Ӳ���(USHORT)
		ItemAuth m_strAuthor;			// ����
		KItemAttrList m_CustomAttr;		// �Զ�������
		bool m_bBinding;				// ����(BYTE)
		KItemMail m_Mail;				// �ʼ�ת��Ʒ
		KItemCustomData m_ItemCustom;		// �Զ�������

	public:
		BeginDefLuaClass(KItem)
			DefMemberVar(m_dwID);
		EndDef
	};
}

