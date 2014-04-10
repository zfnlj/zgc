/********************************************************************
created:	2011/3/10
manager:	Allen Wang
copyright:	Aurora Interactive, 2011
*********************************************************************/

#pragma once
#include "System/KType.h"
#include "KEventDefine.h"

namespace KEventAbout
{
	// �¼�����ӿ�
	class IEventInterface
	{
	public:
		virtual ~IEventInterface() {};
	public:
		/**
		 * �¼���Ӧ
		 * param:
		 *		id:			�¼�id
		 *		pData:		�¼�����ʱ�Ļ���
		 *		customData:	�û��Զ�������
		 * return:
		 *		���¼��Ƿ������(�Ժ󻹻᲻�ᴦ����¼�)��������(�Ժ󲻴�����)��Ҫ����true������false
		 */
		virtual bool OnEvent(KEventID id, const KEventData* pData, uint_r customData)
		{
			if (Filter(id, pData, customData))
			{
				return OnEventImp(id, pData, customData);
			}
			return false;
		}

	protected:
		/**
		 * �¼���Ӧ��ʵ�ֲ���
		 * return:
		 *		���¼��Ƿ������(�Ժ󻹻᲻�ᴦ����¼�)��������(�Ժ󲻴�����)��Ҫ����true������false
		 */
		virtual bool OnEventImp(KEventID id, const KEventData* pData, uint_r customData) = 0;
		
		/**
		 * �¼�������
		 * �����¼����������Ƿ�Ҫ������¼�
		 * �����Ҫ�����򷵻�true������false
		 */
		virtual bool Filter(KEventID id, const KEventData* pData, uint_r customData) { return true; }
	};
};

