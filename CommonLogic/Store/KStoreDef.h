/********************************************************************
created:	2012/9/19
manager:	Zhang Liuxi
decr:		NPC�̵��һЩ����
copyright:	Aurora Interactive, 2012
*********************************************************************/

#pragma once


#define PRICE_TYPE_MAX				(2)			// ����Ʒʱ��Ҫ�Ļ�������
#define INVALID_STORE_ID			(-1)		// ��Ч�̵�ID
#define STORE_STOREPOS_MAX			(100)		// �̵�Ʒ�������
#define STORE_BUYBACKPOS_MAX		(5)			// �ع���Ʒ�������
#define REPAIR_ALL_FLAG				(-1)		// ����ȫ��װ��
#define STORE_TITLE_MAXLEN			(20)		// �̵������󳤶�
#define STORE_LABEL_MAXLEN			(40)		// �̵�label��󳤶�
#define STORE_LABEL_MAXNUM			(10)		// һ���̵�����label��
#define STORE_ONCE_MAXNUM			(999)		// һ�ι���������Ʒ����

namespace KStoreAbout
{

/**
** ������Ʒ���
**/
enum STOREBUY_RETSULT
{
	enumBR_suc,					// �ɹ�
	enumBR_failed,				// ͨ��ʧ��
	enumBR_moneyFailed,			// Ǯ����
	enumBR_bagNotEnough,		// ����λ�ò���
	enumBR_costItemFailed,		// ������

	enumBR_count
};









};

