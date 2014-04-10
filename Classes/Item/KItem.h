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

#define ITEM_CUSTOM_NODE_MAXLEN		40		// 自定义数据buffer最大长度
#define MAIL_HEAD_LEN			40
#define MAIL_BODY_LEN			160
namespace KItemAbout
{
	struct KCreateInfo_ItemBase;

	// 物品序列化标记
	enum enumItemDbStruct
	{
		enum_attributeList=0,	// 属性对
		enum_durable,			// 当前耐久值
		enum_expirationTime,	// 到期时间
		enum_frequency,			// 使用剩余次数
		enum_stackNumber,		// 当前叠加层数
		enum_author,			// 作者
		enum_customAttribute,	// 自定义属性
		enum_binding,			// 归属
		enum_mail,				// 邮件转物品
		enum_customData,		// 自定义数据
		enumIDS_Count
	};
	
	/**
	** 物品自定义数据类型
	**/
	enum enumItemCustomData
	{
		enum_ICD_None=0,						// 空
		enum_ICD_BattlePet_StrengthLvl,			// 宠物强度等级
		enum_ICD_BattlePet_Quality,				// 宠物品质
		
		enum_ICD_Food_MeatID,					// 料理产物食材ID
		enum_ICD_Food_IngredientID1,			// 料理产物配料ID1
		enum_ICD_Food_IngredientID2,			// 料理产物配料ID2

		enum_ICD_Count
	};

	const int ItemAuthNameLen = 50;		// 物品作者信息的长度
	typedef ::System::Collections::KString<ItemAuthNameLen> ItemAuth;	// 作者

	// 物品属性对
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

	// 物品属性对列表
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
		::System::Collections::KVector<KItemAttrNode> m_vecItemAttrList;	// 属性对 id, value
	};

	class KItem;

	// 邮件相关
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

	// 自定义数据节点
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
		unsigned short m_nFlag;		// 标记
		BYTE m_nBuffLen;			// buffer长度
		BYTE m_szBuffer[ITEM_CUSTOM_NODE_MAXLEN];		// buffer
	};

	// 自定义数据
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
		** 添加自定义数据
		**/
		bool AddCustomData(unsigned short nFlag, const void* pBuffer, int nBuffLen);
		/**
		** 得到添加的自定义数据总数
		**/
		int GetSize() const;
		/**
		** 得到指定index的自定义数据类型
		**/
		int GetFlag(int nIdx) const;
		/**
		** 得到指定index的自定义数据类型
		**/
		const BYTE* GetBuffer(int nIdx, int& nBufferLen) const;
		/**
		** 得到指定类型的自定义数据类型
		**/
		const BYTE* GetBufferByFlag(int nFlag, int& nBufferLen) const;

	private:
		::System::Collections::KVector<KItemCustomDataNode> m_listNode;
	};

	// 物品类
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

		// 序列化
		bool Serilize(KDataOutputStream& os) const;
		// 反序列化
		bool Unserilize(KMsgInputStream& si);
		// Chat序列化
		bool SerilizeChat(char* pBuff, int nBuffLen) const;
		// Chat反序列化
		bool UnserilizeChat(const char* pBuff);
		void Clear();
		bool CheckOneFlag(enumItemDbStruct nFlag) const;
		/**
		** 是否能叠加(以下情况不能叠加)
		** 1、有次数和时间的不能
		** 2、装备一定不能
		** 3、有自定义属性不能
		** 4、邮件物品不能
		**/
		bool IsStack() const;

		inline DWORD GetID() const
		{
			return m_dwID;
		}

		/**
		** 添中一个属性
		**/
		bool AddAttr(int nAttrID, int nAttrValue, int nParam);
		bool AddAttrNode(const KItemAttrNode& attrNode);
		/**
		** 删除所有属性
		**/
		void ClearAllAttr();
		/**
		** 得到当前设置的属性总数
		**/
		int GetAttrSize() const;
		/**
		** 得到当前设置的属性ID和属性值
		**/
		bool GetAttrInfo(int nIdx, int& nAttrID, int& nAttrValue, int& nParam) const;
		/**
		** 得到当前设置的属性ID
		**/
		int GetAttrID(int nIdx) const;
		/**
		** 得到当前设置的属性值
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
		** 添中一个自定义属性
		**/
		bool AddCustomAttr(int nAttrID, int nAttrValue, int nParam);
		/**
		** 删除所有自定义属性
		**/
		void DelAllCustomAttr();
		/**
		** 得到当前设置的自定义属性总数
		**/
		int GetCustomAttrSize() const;
		/**
		** 得到当前设置的自定义属性ID
		**/
		int GetCustomAttrID(int nIdx) const;
		/**
		** 得到当前设置的自定义属性值
		**/
		int GetCustomAttrValue(int nIdx) const;

		bool SetBinding(bool bBinding);
		bool IsBinding() const;
		/**
		** 物品添加邮件信息
		**/
		bool AddMailItem(const char *pName, const char *pTitle, const char *pTime, const char *pContent);
		/**
		** 是否是一个邮件物品
		**/
		bool IsMailItem() const;
		/**
		** 是否是一个装置
		**/
		bool IsDeviceItem() const;
		/**
		** 得到邮件发送者
		**/
		const char* GetMailSenderName() const;
		/**
		** 得到邮件Title
		**/
		const char* GetMailTitle() const;
		/**
		** 得到邮件发送时间
		**/
		const char* GetMailTime() const;
		/**
		** 得到邮件内容
		**/
		const char* GetMailContent() const;

		/**
		** 添加一个自定义数据
		**/
		bool AddCustomData(unsigned short nFlag, const void* pBuffer, int nBuffLen);
		/**
		** 是否有自定义数据
		**/
		bool IsCustomData() const;
		/**
		** 得到自定义数据个数
		**/
		int GetCustomDataSize() const;
		/**
		** 得到对应自定义数据类型
		**/
		int GetCustomDataFlag(int nIdx) const;
		/**
		** 得到对应自定义数据类型
		**/
		const BYTE* GetCustomDataBuffer(int nIdx, int& nBufferLen);
		
		/**
		** 获取物品品质
		**/
		int GetQuality() const;

		void AddStackNum(int val);
	private:
		void SetOneCheckFlag(enumItemDbStruct nFlag);
		void ClearOneCheckFlag(enumItemDbStruct nFlag);

	public:
		DWORD m_dwID;					// 物品ID

	private:
		// 物口存储结构
		DWORD m_dwFlag;					// 用以标记下面哪些部分被存储了
		KItemAttrList m_AttrList;		// 属性对 id, value
		int m_nDurable;					// 当前耐久值(USHORT)
		int m_nExpirationTime;			// 到期时间(UINT)
		int m_nFrequency;				// 使用剩余次数(USHORT)
		int m_nStackNumber;				// 当前叠加层数(USHORT)
		ItemAuth m_strAuthor;			// 作者
		KItemAttrList m_CustomAttr;		// 自定义属性
		bool m_bBinding;				// 归属(BYTE)
		KItemMail m_Mail;				// 邮件转物品
		KItemCustomData m_ItemCustom;		// 自定义数据

	public:
		BeginDefLuaClass(KItem)
			DefMemberVar(m_dwID);
		EndDef
	};
}

