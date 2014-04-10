#include "KItem.h"
#include "CommonLogic/Item/KItemDef.h"
#include "CommonLogic/Item/KStruct_Item.h"
#include <string>

#define CUSTOMEATTR_MAX_SIZE	(5)		// 自定义属性最大个数

namespace KItemAbout
{
	//////////////////////////////////////////////////////////////////////////
	// 物品属性对
	KItemAttrNode::KItemAttrNode()
	{
		Clear();
	}
	
	KItemAttrNode::~KItemAttrNode()
	{
	
	}

	void KItemAttrNode::Clear()
	{
		m_nAttrID = 0;
		m_nAttrValue = 0;
		m_nParam = 0;
	}

	bool KItemAttrNode::operator==( const KItemAttrNode& ItemAttrNode ) const
	{
		if(m_nAttrID != ItemAttrNode.m_nAttrID) return false;
		if(m_nAttrValue != ItemAttrNode.m_nAttrValue) return false;
		if(m_nParam != ItemAttrNode.m_nParam) return false;

		return true;
	}

	bool KItemAttrNode::operator!=( const KItemAttrNode& ItemAttrNode ) const
	{
		return !(*this == ItemAttrNode);
	}

	bool KItemAttrNode::Serilize(KDataOutputStream& os) const
	{
		BOOL bRet = os.WriteInt(m_nAttrID);
		ASSERT_RETURN(bRet, false);
		bRet = os.WriteInt(m_nAttrValue);
		ASSERT_RETURN(bRet, false);
		bRet = os.WriteInt(m_nParam);
		ASSERT_RETURN(bRet, false);

		return true;
	}

	bool KItemAttrNode::Unserilize(KMsgInputStream& si)
	{
		BOOL bRet = si.ReadInt(m_nAttrID);
		ASSERT_RETURN(bRet, false);
		bRet = si.ReadInt(m_nAttrValue);
		ASSERT_RETURN(bRet, false);
		bRet = si.ReadInt(m_nParam);
		ASSERT_RETURN(bRet, false);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	// 物品属性对列表
	KItemAttrList::KItemAttrList()
	{
		Clear();
	}

	KItemAttrList::~KItemAttrList()
	{
	
	}
	
	KItemAttrList::KItemAttrList(const KItemAttrList& ItemAttrList)
	{
		m_vecItemAttrList = ItemAttrList.m_vecItemAttrList;
	}
	
	KItemAttrList& KItemAttrList::operator=(const KItemAttrList& ItemAttrList)
	{
		if(this == &ItemAttrList)
		{
			return *this;
		}

		m_vecItemAttrList = ItemAttrList.m_vecItemAttrList;
		return *this;
	}

	bool KItemAttrList::operator==( const KItemAttrList& ItemAttrList ) const
	{
		int nThisSize = m_vecItemAttrList.size();
		int nOtherSize = ItemAttrList.m_vecItemAttrList.size();
		if(nThisSize != nOtherSize) return false;
		
		for(int nLoop=0;nLoop<nThisSize;nLoop++)
		{
			if(m_vecItemAttrList[nLoop] != ItemAttrList.m_vecItemAttrList[nLoop])
			{
				return false;
			}
		}

		return true;
	}

	bool KItemAttrList::operator!=( const KItemAttrList& ItemAttrList ) const
	{
		return !(*this == ItemAttrList);
	}

	void KItemAttrList::Clear()
	{
		m_vecItemAttrList.clear();
	}

	int KItemAttrList::GetAttrCount() const
	{
		return m_vecItemAttrList.size();
	}

	bool KItemAttrList::GetAttrInfo(int nIdx, int& nAttrID, int& nAttrValue, int& nParam) const
	{
		int nAttrSize = m_vecItemAttrList.size();
		ASSERT_RETURN(nIdx >= 0 && nIdx < nAttrSize, false);

		nAttrID = m_vecItemAttrList[nIdx].m_nAttrID;
		nAttrValue = m_vecItemAttrList[nIdx].m_nAttrValue;
		nParam = m_vecItemAttrList[nIdx].m_nParam;
		return true;
	}

	int KItemAttrList::Size() const
	{
		return m_vecItemAttrList.size();
	}

	bool KItemAttrList::Serilize(KDataOutputStream& os) const
	{
		int nSize = Size();

		BOOL bRet = os.WriteByte((BYTE)nSize);
		ASSERT_RETURN(bRet, false);

		for(int nLoop=0;nLoop<nSize;nLoop++)
		{
			bool bSerRet = m_vecItemAttrList[nLoop].Serilize(os);
			ASSERT_RETURN(bSerRet, false);
		}

		return true;
	}

	bool KItemAttrList::Unserilize(KMsgInputStream& si)
	{
		Clear();

		BYTE nSize = 0;
		BOOL bRet = si.ReadByte(nSize);
		ASSERT_RETURN(bRet, false);

		for(int nLoop=0;nLoop<nSize;nLoop++)
		{
			KItemAttrNode ItemAttrNode;
			bool bSerRet = ItemAttrNode.Unserilize(si);
			ASSERT_RETURN(bSerRet, false);

			m_vecItemAttrList.push_back(ItemAttrNode);
		}

		return true;
	}
	
	bool KItemAttrList::AddAttr(int nAttrID, int nAttrValue, int nParam)
	{
		ASSERT_RETURN(nAttrID > 0, false);

		int nSize = Size();
		for(int nLoop=0;nLoop<nSize;nLoop++)
		{
			KItemAttrNode& ItemAttrNode = m_vecItemAttrList[nLoop];
			if(ItemAttrNode.m_nAttrID == nAttrID)
			{
				ItemAttrNode.m_nAttrValue = nAttrValue;
				ItemAttrNode.m_nParam = nParam;
				return true;
			}
		}

		KItemAttrNode ItemAttrNodeNew;
		ItemAttrNodeNew.m_nAttrID = nAttrID;
		ItemAttrNodeNew.m_nAttrValue = nAttrValue;
		ItemAttrNodeNew.m_nParam = nParam;
		m_vecItemAttrList.push_back(ItemAttrNodeNew);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	// 邮件
	KItemMail::KItemMail()
	{
		Clear();
	}

	KItemMail::~KItemMail()
	{
		Clear();
	}

	KItemMail::KItemMail(const KItemMail& ItemMail)
	{
		m_szTitle = ItemMail.m_szTitle;
		m_szTime = ItemMail.m_szTime;
		m_szContent = ItemMail.m_szContent;
		m_szSenderName = ItemMail.m_szSenderName;
	}

	KItemMail& KItemMail::operator=(const KItemMail& ItemMail)
	{
		if(this == &ItemMail)
		{
			return *this;
		}

		m_szTitle = ItemMail.m_szTitle;
		m_szTime = ItemMail.m_szTime;
		m_szContent = ItemMail.m_szContent;
		m_szSenderName = ItemMail.m_szSenderName;

		return *this;
	}

	bool KItemMail::operator==( const KItemMail& ItemMail ) const
	{
		if (m_szTitle != ItemMail.m_szTitle || m_szTime != ItemMail.m_szTime || m_szContent != ItemMail.m_szContent || m_szSenderName != ItemMail.m_szSenderName)
		{
			return false;
		}

		return true;
	}

	bool KItemMail::operator!=( const KItemMail& ItemMail ) const
	{
		return !(*this == ItemMail);
	}

	// 镶嵌的石头信息列表
	void KItemMail::Clear()
	{
		m_szTitle.clear();
		m_szTime.clear();
		m_szContent.clear();
		m_szSenderName.clear();
	}

	bool KItemMail::Serilize(KDataOutputStream& os) const
	{
		BOOL bRet = os.WriteString(m_szSenderName.c_str());
		ASSERT_RETURN(bRet, false);
		bRet = os.WriteString(m_szTitle.c_str());
		ASSERT_RETURN(bRet, false);
		bRet = os.WriteString(m_szTime.c_str());
		ASSERT_RETURN(bRet, false);
		bRet = os.WriteString(m_szContent.c_str());
		ASSERT_RETURN(bRet, false);

		return true;
	}

	bool KItemMail::Unserilize(KMsgInputStream& si)
	{
		char szSender[MAX_NAME_DEF] = {0};
		char szTitle[MAIL_HEAD_LEN] = {0};
		char szTime[30] = {0};
		char szContent[MAIL_BODY_LEN] = {0};

		BOOL bRet = si.ReadString(szSender, sizeof(szSender));
		ASSERT_RETURN(bRet, false);
		bRet = si.ReadString(szTitle, sizeof(szTitle));
		ASSERT_RETURN(bRet, false);
		bRet = si.ReadString(szTime, sizeof(szTime));
		ASSERT_RETURN(bRet, false);
		bRet = si.ReadString(szContent, sizeof(szContent));
		ASSERT_RETURN(bRet, false);

		m_szSenderName = szSender;
		m_szTitle = szTitle;
		m_szTime = szTime;
		m_szContent = szContent;

		return true;
	}
	
	//////////////////////////////////////////////////////////////////////////
	KItemCustomDataNode::KItemCustomDataNode()
	{
		Clear();
	}

	KItemCustomDataNode::~KItemCustomDataNode()
	{

	}

	KItemCustomDataNode::KItemCustomDataNode( const KItemCustomDataNode& ItemCustomNodeData )
	{
		m_nFlag = ItemCustomNodeData.m_nFlag;
		m_nBuffLen = ItemCustomNodeData.m_nBuffLen;
		memcpy(m_szBuffer, ItemCustomNodeData.m_szBuffer, m_nBuffLen);
	}

	KItemCustomDataNode& KItemCustomDataNode::operator=( const KItemCustomDataNode& ItemCustomNodeData )
	{
		if(this == &ItemCustomNodeData)
		{
			return *this;
		}
		
		m_nFlag = ItemCustomNodeData.m_nFlag;
		m_nBuffLen = ItemCustomNodeData.m_nBuffLen;
		memcpy(m_szBuffer, ItemCustomNodeData.m_szBuffer, m_nBuffLen);
		return *this;
	}

	bool KItemCustomDataNode::operator==( const KItemCustomDataNode& ItemCustomNodeData ) const
	{
		if(m_nFlag != ItemCustomNodeData.m_nFlag) return false;
		if(m_nBuffLen != ItemCustomNodeData.m_nBuffLen) return false;
		if(memcmp(m_szBuffer, ItemCustomNodeData.m_szBuffer, m_nBuffLen)) return false;

		return true;
	}

	bool KItemCustomDataNode::operator!=( const KItemCustomDataNode& ItemCustomNodeData ) const
	{
		return !(*this == ItemCustomNodeData);
	}

	bool KItemCustomDataNode::Serilize( KDataOutputStream& os ) const
	{
		BOOL bRet = os.WriteShort(m_nFlag);
		ASSERT_RETURN(bRet, false);
		bRet = os.WriteByte(m_nBuffLen);
		ASSERT_RETURN(bRet, false);
		bRet = os.WriteByteArray(m_szBuffer, m_nBuffLen);
		ASSERT_RETURN(bRet, false);
		
		return true;
	}

	bool KItemCustomDataNode::Unserilize( KMsgInputStream& si )
	{
		short nFlag = 0;
		BOOL bRet = si.ReadShort(nFlag);
		m_nFlag = nFlag;
		ASSERT_RETURN(bRet, false);
		bRet = si.ReadByte(m_nBuffLen);
		ASSERT_RETURN(bRet, false);
		bRet = si.ReadByteArray(m_szBuffer, m_nBuffLen);
		ASSERT_RETURN(bRet, false);
		
		return true;
	}

	void KItemCustomDataNode::Clear()
	{
		m_nFlag = 0;
		m_nBuffLen = 0;
		memset(m_szBuffer, 0, sizeof(m_szBuffer));
	}

	//////////////////////////////////////////////////////////////////////////
	// 自定义数据
	KItemCustomData::KItemCustomData()
	{
		Clear();
	}

	KItemCustomData::~KItemCustomData()
	{

	}

	KItemCustomData::KItemCustomData( const KItemCustomData& ItemCustomData )
	{
		int nSize = ItemCustomData.m_listNode.size();
		for(int i=0;i<nSize;i++)
		{
			m_listNode.push_back(ItemCustomData.m_listNode[i]);
		}
	}

	KItemCustomData& KItemCustomData::operator=( const KItemCustomData& ItemCustomData )
	{
		if(this == &ItemCustomData)
		{
			return *this;
		}
		
		m_listNode = ItemCustomData.m_listNode;

		return *this;
	}

	bool KItemCustomData::operator==( const KItemCustomData& ItemCustomData ) const
	{
		int nThisSize = m_listNode.size();
		int nOtherSize = ItemCustomData.m_listNode.size();
		if(nThisSize != nOtherSize) return false;
		
		for(int i=0;i<nThisSize;i++)
		{
			if(m_listNode[i] != ItemCustomData.m_listNode[i])
			{
				return false;
			}
		}
		
		return true;
	}

	bool KItemCustomData::operator!=( const KItemCustomData& ItemCustomData ) const
	{
		return !(*this == ItemCustomData);
	}

	bool KItemCustomData::Serilize( KDataOutputStream& os ) const
	{
		int nSize = m_listNode.size();
		
		BOOL bRet = os.WriteByte((BYTE)nSize);
		ASSERT_RETURN(bRet, false);

		for(int i=0;i<nSize;i++)
		{
			bool bSerRet = m_listNode[i].Serilize(os);
			ASSERT_RETURN(bSerRet, false);
		}
		
		return true;
	}

	bool KItemCustomData::Unserilize( KMsgInputStream& si )
	{
		Clear();
		
		BYTE nSize = 0;
		BOOL bRet = si.ReadByte(nSize);
		ASSERT_RETURN(bRet, false);

		for(int nLoop=0;nLoop<nSize;nLoop++)
		{
			KItemCustomDataNode ItemCustomNodeData;
			bool bSerRet = ItemCustomNodeData.Unserilize(si);
			ASSERT_RETURN(bSerRet, false);

			m_listNode.push_back(ItemCustomNodeData);
		}

		return true;
	}

	void KItemCustomData::Clear()
	{
		m_listNode.clear();
	}

	bool KItemCustomData::AddCustomData( unsigned short nFlag, const void* pBuffer, int nBuffLen )
	{
		ASSERT_RETURN(nFlag > enum_ICD_None && nFlag < enum_ICD_Count, false);
		ASSERT_RETURN(pBuffer && nBuffLen < ITEM_CUSTOM_NODE_MAXLEN, false);
		
		KItemCustomDataNode ItemCustomNode;
		ItemCustomNode.m_nFlag = nFlag;
		ItemCustomNode.m_nBuffLen = nBuffLen;
		memcpy(ItemCustomNode.m_szBuffer, pBuffer, nBuffLen);
		
		m_listNode.push_back(ItemCustomNode);
		return true;
	}

	int KItemCustomData::GetSize() const
	{
		return m_listNode.size();
	}

	int KItemCustomData::GetFlag( int nIdx ) const
	{
		int nSize = m_listNode.size();
		if(nIdx > 0 && nIdx < nSize)
		{
			return m_listNode[nIdx].m_nFlag;
		}
		
		return enum_ICD_None;
	}

	const BYTE* KItemCustomData::GetBuffer( int nIdx, int& nBufferLen ) const
	{
		int nSize = m_listNode.size();
		if(nIdx > 0 && nIdx < nSize)
		{
			nBufferLen = m_listNode[nIdx].m_nBuffLen;
			return m_listNode[nIdx].m_szBuffer;
		}

		return NULL;
	}

	const BYTE* KItemCustomData::GetBufferByFlag(int nFlag, int& nBufferLen ) const
	{
		if(nFlag <= enum_ICD_None || nFlag >= enum_ICD_Count)
		{
			nBufferLen = 0;
			return NULL;
		}
		int nSize = m_listNode.size();
		if(nSize <= 0)
		{
			nBufferLen = 0;
			return NULL;
		}
		
		for(int i=0;i<nSize;i++)
		{
			if(m_listNode[i].m_nFlag == nFlag)
			{
				nBufferLen = m_listNode[i].m_nBuffLen;
				return m_listNode[i].m_szBuffer;
			}
		}
		
		nBufferLen = 0;
		return NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	// 物品类
	KItem::KItem()
	{
		Clear();
	}

	KItem::~KItem()
	{

	}

	KItem::KItem(const KItem& Item)
	{
		m_dwID = Item.m_dwID;							// 物品ID
		m_dwFlag = Item.m_dwFlag;						// 用以标记下面哪些部分被存储了
		m_AttrList = Item.m_AttrList;					// 属性对 id, value
		m_nDurable = Item.m_nDurable;					// 当前耐久值
		m_nExpirationTime = Item.m_nExpirationTime;		// 到期时间
		m_nFrequency = Item.m_nFrequency;				// 使用剩余次数
		m_nStackNumber = Item.m_nStackNumber;			// 当前叠加层数
		m_strAuthor = Item.m_strAuthor;					// 作者
		m_CustomAttr = Item.m_CustomAttr;				// 自定义属性
		m_bBinding = Item.m_bBinding;					// 绑定
		m_Mail = Item.m_Mail;							// 邮件
		m_ItemCustom = Item.m_ItemCustom;				// 自定义数据
	}
	KItem::KItem(DWORD id,int stackNum)
	{
		Clear();
		m_dwID = id;
		SetStackNumber(stackNum);
	}
	
	KItem& KItem::operator=(const KItem& Item)
	{
		if(this == &Item)
		{
			return *this;
		}

		m_dwID = Item.m_dwID;							// 物品ID
		m_dwFlag = Item.m_dwFlag;						// 用以标记下面哪些部分被存储了
		m_AttrList = Item.m_AttrList;					// 属性对 id, value
		m_nDurable = Item.m_nDurable;					// 当前耐久值
		m_nExpirationTime = Item.m_nExpirationTime;		// 到期时间
		m_nFrequency = Item.m_nFrequency;				// 使用剩余次数
		m_nStackNumber = Item.m_nStackNumber;			// 当前叠加层数
		m_strAuthor = Item.m_strAuthor;					// 作者
		m_CustomAttr = Item.m_CustomAttr;				// 自定义属性
		m_bBinding = Item.m_bBinding;					// 绑定
		m_Mail = Item.m_Mail;							// 邮件
		m_ItemCustom = Item.m_ItemCustom;				// 自定义数据

		return *this;
	}

	bool KItem::operator==( const KItem& Item ) const
	{
		if(m_dwID != Item.m_dwID) return false;
		if(m_dwFlag != Item.m_dwFlag) return false;
		if(m_AttrList != Item.m_AttrList) return false;
		if(m_nDurable != Item.m_nDurable) return false;
		if(m_nExpirationTime != Item.m_nExpirationTime) return false;
		if(m_nFrequency != Item.m_nFrequency) return false;
		if(m_nStackNumber != Item.m_nStackNumber) return false;
		if(m_strAuthor != Item.m_strAuthor) return false;
		if(m_CustomAttr != Item.m_CustomAttr) return false;
		if(m_bBinding != Item.m_bBinding) return false;
		if(m_Mail != Item.m_Mail) return false;
		if(m_ItemCustom != Item.m_ItemCustom) return false;

		return true;
	}

	bool KItem::operator!=( const KItem& Item ) const
	{
		return !(*this == Item);
	}

	void KItem::Clear()
	{
		m_dwID = ITEM_INVALID_ID;	// 物品ID
		m_dwFlag = 0;				// 用以标记下面哪些部分被存储了
		m_AttrList.Clear();			// 属性对 id, value
		m_nDurable = 0;				// 当前耐久值
		m_nExpirationTime = 0;		// 到期时间
		m_nFrequency = 0;			// 使用剩余次数
		m_nStackNumber = 0;			// 当前叠加层数
		m_strAuthor.clear();		// 作者
		m_CustomAttr.Clear();		// 自定义属性
		m_bBinding = false;			// 绑定
		m_Mail.Clear();				// 邮件
		m_ItemCustom.Clear();		// 自定义数据
	}

	bool KItem::CheckOneFlag( enumItemDbStruct nFlag ) const
	{
		ASSERT_I(nFlag >= 0 && nFlag < enumIDS_Count);
		return ((m_dwFlag & (1 << nFlag)) ? true : false);
	}

	void KItem::SetOneCheckFlag( enumItemDbStruct nFlag )
	{
		ASSERT_I(nFlag >= 0 && nFlag < enumIDS_Count);
		m_dwFlag |= (1 << nFlag);
	}

	void KItem::ClearOneCheckFlag(enumItemDbStruct nFlag)
	{
		ASSERT_I(nFlag >= 0 && nFlag < enumIDS_Count);
		m_dwFlag &= ~(1 << nFlag);
	}

	bool KItem::Serilize(KDataOutputStream& os) const
	{
		// 物品ID
		BOOL bRet = os.WriteDword(m_dwID);
		ASSERT_RETURN(bRet, false);

		// 标记位
		bRet = os.WriteDword(m_dwFlag);
		ASSERT_RETURN(bRet, false);

		// 属性对 id, value
		if(CheckOneFlag(enum_attributeList))
		{
			bool bSerRet = m_AttrList.Serilize(os);
			ASSERT_RETURN(bSerRet, false);
		}

		// 当前耐久值
		if(CheckOneFlag(enum_durable))
		{
			bRet = os.WriteShort((short)m_nDurable);
			ASSERT_RETURN(bRet, false);
		}
		
		// 到期时间
		if(CheckOneFlag(enum_expirationTime))
		{
			bRet = os.WriteInt(m_nExpirationTime);
			ASSERT_RETURN(bRet, false);
		}

		// 使用剩余次数
		if(CheckOneFlag(enum_frequency))
		{
			bRet = os.WriteShort(m_nFrequency);
			ASSERT_RETURN(bRet, false);
		}

		// 当前叠加层数
		if(CheckOneFlag(enum_stackNumber))
		{
			bRet = os.WriteShort(m_nStackNumber);
			ASSERT_RETURN(bRet, false);
		}

		// 作者
		if(CheckOneFlag(enum_author))
		{
			bRet = os.WriteString(m_strAuthor.c_str(), m_strAuthor.GetCount());
			ASSERT_RETURN(bRet, false);
		}

		// 自定义属性
		if(CheckOneFlag(enum_customAttribute))
		{
			bool bSerRet = m_CustomAttr.Serilize(os);
			ASSERT_RETURN(bSerRet, false);
		}

		// 绑定
		if(CheckOneFlag(enum_binding))
		{
			BYTE nTmp = m_bBinding ? 1 : 0;
			bRet = os.WriteByte(nTmp);
			ASSERT_RETURN(bRet, false);
		}

		// 邮件信息
		if(CheckOneFlag(enum_mail))
		{
			bool bSerRet = m_Mail.Serilize(os);
			ASSERT_RETURN(bSerRet, false);
		}
		
		// 自定义数据
		if(CheckOneFlag(enum_customData))
		{
			bool bSerRet = m_ItemCustom.Serilize(os);
			ASSERT_RETURN(bSerRet, false);
		}

		return true;
	}

	bool KItem::Unserilize(KMsgInputStream& si)
	{
		// 物品ID
		BOOL bRet = si.ReadDword(m_dwID);
		ASSERT_RETURN(bRet, false);

		// 标记位
		bRet = si.ReadDword(m_dwFlag);
		ASSERT_RETURN(bRet, false);

		// 属性对 id, value
		if(CheckOneFlag(enum_attributeList))
		{
			bool bSerRet = m_AttrList.Unserilize(si);
			ASSERT_RETURN(bSerRet, false);
		}

		// 当前耐久值
		if(CheckOneFlag(enum_durable))
		{
			short sTmp = 0;
			bRet = si.ReadShort(sTmp);
			ASSERT_RETURN(bRet, false);
			m_nDurable = sTmp;
		}

		// 到期时间
		if(CheckOneFlag(enum_expirationTime))
		{
			int nTmp = 0;
			bRet = si.ReadInt(nTmp);
			ASSERT_RETURN(bRet, false);
			m_nExpirationTime = nTmp;
		}

		// 使用剩余次数
		if(CheckOneFlag(enum_frequency))
		{
			short sTmp = 0;
			bRet = si.ReadShort(sTmp);
			ASSERT_RETURN(bRet, false);
			m_nFrequency = sTmp;
		}

		// 当前叠加层数
		if(CheckOneFlag(enum_stackNumber))
		{
			short sTmp = 0;
			bRet = si.ReadShort(sTmp);
			ASSERT_RETURN(bRet, false);
			m_nStackNumber = sTmp;
		}

		// 作者
		if(CheckOneFlag(enum_author))
		{
			// 内容
			char cTmp[1024] = "";
			bRet = si.ReadString(cTmp, sizeof(cTmp));
			ASSERT_RETURN(bRet, false);
			m_strAuthor.append(cTmp, strlen(cTmp));
		}

		// 自定义属性
		if(CheckOneFlag(enum_customAttribute))
		{
			bool bSerRet = m_CustomAttr.Unserilize(si);
			ASSERT_RETURN(bSerRet, false);
		}

		// 绑定
		if(CheckOneFlag(enum_binding))
		{
			BYTE byteTmp;
			bRet = si.ReadByte(byteTmp);
			ASSERT_RETURN(bRet, false);
			m_bBinding = (byteTmp ? true : false);
		}

		// 邮件信息
		if(CheckOneFlag(enum_mail))
		{
			bool bSerRet = m_Mail.Unserilize(si);
			ASSERT_RETURN(bSerRet, false);
		}
		
		// 自定义数据
		if(CheckOneFlag(enum_customData))
		{
			bool bSerRet = m_ItemCustom.Unserilize(si);
			ASSERT_RETURN(bSerRet, false);
		}

		return true;
	}

	bool KItem::SerilizeChat(char* pBuff, int nBuffLen) const
	{
		ASSERT_RETURN(pBuff && nBuffLen > 0, false);
		char buff[1000];
		KDataOutputStream os(buff, sizeof(buff));
		bool bRet  = Serilize(os);
		ASSERT_RETURN(bRet, false);
		ASSERT_RETURN((int)(os.m_pos) * 2 < nBuffLen, false);
		char tmp[3] = "";
		for(int i=0;i<(int)os.m_pos;i++)
		{
			sprintf_k(tmp, sizeof(tmp), "%02x", (BYTE)(os.m_pBuf[i]));
			strcat(pBuff, tmp);
		}
		return true;
	}

	bool KItem::UnserilizeChat(const char* pBuff)
	{
		ASSERT_RETURN(pBuff, false);
		int nBuffLen = strlen(pBuff);
		int n = nBuffLen % 2;
		ASSERT_RETURN(0 == n, false);
		int nSize = nBuffLen / 2;
		char buff[1000];
		ASSERT_RETURN(sizeof(buff) >= nSize, false);
		
		char szHex[3] = "";
		for(int i=0;i<nSize;i++)
		{
			szHex[0] = pBuff[2 * i];
			szHex[1] = pBuff[2 * i + 1];
			int nValue;
			sscanf(szHex, "%x", &nValue);
			BYTE byteValue = (BYTE)nValue;
			memcpy(buff + i, &byteValue, sizeof(byteValue));
		}
		KMsgInputStream si(buff, nSize);
		bool bRet = Unserilize(si);
		ASSERT_RETURN(bRet, false);
		return true;
	}

	bool KItem::IsStack() const
	{
		if( CheckOneFlag(enum_expirationTime) ||
			CheckOneFlag(enum_frequency) ||
			CheckOneFlag(enum_customAttribute) ||
			CheckOneFlag(enum_customData) ||
			CheckOneFlag(enum_mail))
		{
			return false;
		}

		return true;
	}

	bool KItem::AddAttr( int nAttrID, int nAttrValue, int nParam )
	{
		SetOneCheckFlag(enum_attributeList);
		m_AttrList.AddAttr(nAttrID, nAttrValue, nParam);
		return true;
	}

	bool KItem::AddAttrNode( const KItemAttrNode& attrNode )
	{
		SetOneCheckFlag(enum_attributeList);
		m_AttrList.AddAttr(attrNode.m_nAttrID, attrNode.m_nAttrValue, attrNode.m_nParam);
		return true;
	}

	void KItem::ClearAllAttr()
	{
		ClearOneCheckFlag(enum_attributeList);
		m_AttrList.Clear();
	}

	int KItem::GetAttrSize() const
	{
		return m_AttrList.GetAttrCount();
	}

	bool KItem::GetAttrInfo(int nIdx, int& nAttrID, int& nAttrValue, int& nParam) const
	{
		return m_AttrList.GetAttrInfo(nIdx, nAttrID, nAttrValue, nParam);
	}
	
	int KItem::GetAttrID(int nIdx) const
	{
		int nAttrID = 0;
		int nAttrValue = 0;
		int nParam = 0;

		if(!m_AttrList.GetAttrInfo(nIdx, nAttrID, nAttrValue, nParam)) return 0;
		return nAttrID;
	}

	int KItem::GetAttrValue(int nIdx) const
	{
		int nAttrID = 0;
		int nAttrValue = 0;
		int nParam = 0;

		if(!m_AttrList.GetAttrInfo(nIdx, nAttrID, nAttrValue, nParam)) return 0;
		return nAttrValue;
	}

	bool KItem::SetDurable( USHORT sDurable )
	{
		SetOneCheckFlag(enum_durable);
		m_nDurable = sDurable;
		return true;
	}
	
	int KItem::GetDurable() const
	{
		return m_nDurable;
	}

	bool KItem::SetExpirationTime( UINT nExpirationTime )
	{
		SetOneCheckFlag(enum_expirationTime);
		m_nExpirationTime = nExpirationTime;
		return true;
	}
	
	UINT KItem::GetExpirationTime() const
	{
		return m_nExpirationTime;
	}

	bool KItem::IsFrequency() const
	{
		return CheckOneFlag(enum_frequency);
	}

	bool KItem::SetFrequency( USHORT sFrequency )
	{
		SetOneCheckFlag(enum_frequency);
		m_nFrequency = sFrequency;
		return true;
	}
	
	USHORT KItem::GetFrequency() const
	{
		return m_nFrequency;
	}

	bool KItem::SetStackNumber( USHORT sStackNumber )
	{
		SetOneCheckFlag(enum_stackNumber);
		m_nStackNumber = sStackNumber;
		return true;
	}
	
	int KItem::GetStackNumber() const
	{
		return m_nStackNumber;
	}
	void KItem::AddStackNum(int val)
	{
		m_nStackNumber += val;
	}

	bool KItem::SetAuthor( const char* pAuthor )
	{
		ASSERT_RETURN(pAuthor, false);
		SetOneCheckFlag(enum_author);
		m_strAuthor = pAuthor;
		return true;
	}

	void KItem::ClearAuthor()
	{
		ClearOneCheckFlag(enum_author);
		m_strAuthor.clear();
	}

	bool KItem::AddCustomAttr( int nAttrID, int nAttrValue, int nParam )
	{
		SetOneCheckFlag(enum_customAttribute);
		m_CustomAttr.AddAttr(nAttrID, nAttrValue, nParam);
		return true;
	}

	void KItem::DelAllCustomAttr()
	{
		ClearOneCheckFlag(enum_customAttribute);
		m_CustomAttr.Clear();
	}

	int KItem::GetCustomAttrSize() const
	{
		return m_CustomAttr.Size();
	}

	int KItem::GetCustomAttrID(int nIdx) const
	{
		if(nIdx < 0 && nIdx >= m_CustomAttr.Size()) return 0;
		int nAttrID = 0;
		int nAttrValue = 0;
		int nParam = 0;
		if(!m_CustomAttr.GetAttrInfo(nIdx, nAttrID, nAttrValue, nParam)) return 0;
		return nAttrID;
	}

	int KItem::GetCustomAttrValue(int nIdx) const
	{
		if(nIdx < 0 && nIdx >= m_CustomAttr.Size()) return 0;
		int nAttrID = 0;
		int nAttrValue = 0;
		int nParam = 0;
		if(!m_CustomAttr.GetAttrInfo(nIdx, nAttrID, nAttrValue, nParam)) return 0;
		return nAttrValue;
	}

	bool KItem::SetBinding( bool bBinding )
	{
		if(bBinding)
		{
			SetOneCheckFlag(enum_binding);
			m_bBinding = true;
		}
		else
		{
			ClearOneCheckFlag(enum_binding);
			m_bBinding = false;
		}

		return true;
	}

	bool KItem::IsBinding() const
	{
		return m_bBinding;
	}

	bool KItem::AddMailItem(const char *pName, const char *pTitle, const char *pTime, const char *pContent)
	{
		if (NULL == pName || NULL == pTitle || NULL == pTime || NULL == pContent)
		{
			return false;
		}

		m_Mail.m_szSenderName = pName;
		m_Mail.m_szTitle = pTitle;
		m_Mail.m_szTime = pTime;
		m_Mail.m_szContent = pContent;

		SetOneCheckFlag(enum_mail);
		return true;
	}

	bool KItem::IsMailItem() const
	{
		return CheckOneFlag(enum_mail);
	}

	bool KItem::IsDeviceItem() const
	{
		return true;
	}

	const char* KItem::GetMailSenderName() const
	{
		if(!IsMailItem()) return NULL;
		return m_Mail.m_szSenderName.c_str();
	}

	const char* KItem::GetMailTitle() const
	{
		if(!IsMailItem()) return NULL;
		return m_Mail.m_szTitle.c_str();
	}

	const char* KItem::GetMailTime() const
	{
		if(!IsMailItem()) return NULL;
		return m_Mail.m_szTime.c_str();
	}

	const char* KItem::GetMailContent() const
	{
		if(!IsMailItem()) return NULL;
		return m_Mail.m_szContent.c_str();
	}

	bool KItem::AddCustomData( unsigned short nFlag, const void* pBuffer, int nBuffLen )
	{
		ASSERT_RETURN(nFlag > enum_ICD_None && nFlag < enum_ICD_Count, false);
		ASSERT_RETURN(pBuffer && nBuffLen > 0 && nBuffLen < ITEM_CUSTOM_NODE_MAXLEN, false);
		
		SetOneCheckFlag(enum_customData);
		return m_ItemCustom.AddCustomData(nFlag, pBuffer, nBuffLen);
	}

	bool KItem::IsCustomData() const
	{
		return m_ItemCustom.GetSize() != 0;
	}

	int KItem::GetCustomDataSize() const
	{
		return m_ItemCustom.GetSize();
	}

	int KItem::GetCustomDataFlag( int nIdx ) const
	{
		return m_ItemCustom.GetFlag(nIdx);
	}

	const BYTE* KItem::GetCustomDataBuffer( int nIdx, int& nBufferLen )
	{
		return m_ItemCustom.GetBuffer(nIdx, nBufferLen);
	}

	int KItem::GetQuality() const
	{
		KCreateInfo_ItemBase* pCIIB = KItemCreate::Instance()->GetItemCreateInfo(m_dwID);
		return pCIIB->GetQuality();
	}

}