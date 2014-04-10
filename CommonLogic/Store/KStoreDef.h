/********************************************************************
created:	2012/9/19
manager:	Zhang Liuxi
decr:		NPC商店的一些定义
copyright:	Aurora Interactive, 2012
*********************************************************************/

#pragma once


#define PRICE_TYPE_MAX				(2)			// 买商品时需要的货币类型
#define INVALID_STORE_ID			(-1)		// 无效商店ID
#define STORE_STOREPOS_MAX			(100)		// 商店品的最大数
#define STORE_BUYBACKPOS_MAX		(5)			// 回购物品的最大数
#define REPAIR_ALL_FLAG				(-1)		// 修理全部装备
#define STORE_TITLE_MAXLEN			(20)		// 商店标题最大长度
#define STORE_LABEL_MAXLEN			(40)		// 商店label最大长度
#define STORE_LABEL_MAXNUM			(10)		// 一个商店的最多label数
#define STORE_ONCE_MAXNUM			(999)		// 一次购买的最多物品个数

namespace KStoreAbout
{

/**
** 购买物品结果
**/
enum STOREBUY_RETSULT
{
	enumBR_suc,					// 成功
	enumBR_failed,				// 通用失败
	enumBR_moneyFailed,			// 钱不够
	enumBR_bagNotEnough,		// 背包位置不够
	enumBR_costItemFailed,		// 不能卖

	enumBR_count
};









};

