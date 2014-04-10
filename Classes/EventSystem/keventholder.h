/********************************************************************
created:	2011/3/10
manager:	Allen Wang
copyright:	Aurora Interactive, 2011
*********************************************************************/

#pragma once

#include "System/KType.h"
#include "System/Collections/KMapByVector.h"
#include "System/Memory/KStepObjectPool.h"
#include "KEventInterface.h"
//#include "Clock/IWorldClock.h"
#include "Inc/KCommonDefine.h"

#define NOW				(nowTicks)
#define SECOND_NOW		(nowTime)

namespace KEventAbout
{
	/**
	 * �¼���������id������
	 * ���ǵ�һ��DWORD�㹻�󣬾Ͳ������ˣ�ֱ���ۼ�
	 */
	class KEventHandlerIDAllocator
	{
	public:
		static DWORD Alloc()
		{
			static DWORD currID = 0;
			return ++currID;
		}
	};

	// �¼�ջ�����ݹ����
	const static int MAX_EVENT_STACKS = 10;

	/**
	 * �¼�����
	 */
	template<DWORD EVENT_SIZE, int EVENT_BASE_ID>
	class KEventHolder
	{
	private:
		/**
		 * �¼���������ʵ���ṹ
		 */
		struct KEventHandler
		{
			DWORD m_dwID;
			uint_r m_dwKey;
			uint_r m_dwCustomData;
			KEventHandler* m_pNext;
			bool m_bDeleteFlag;
			IEventInterface* m_pEI;

			KEventHandler()
				: m_dwID(0)
				, m_dwKey(0)
				, m_dwCustomData(0)
				, m_pNext(NULL)
				, m_bDeleteFlag(false)
				, m_pEI(NULL)
			{
			
			}
		};

	public:
		KEventHolder()
		{
			memset(m_Events, 0, sizeof(KEventHandler*) * EVENT_SIZE);
			m_handlingEventStack = 0;
			m_lastBreatheTime = 0;
		}

		~KEventHolder()
		{
			Clear();
		}

	public:
		void Reset()
		{
			this->Clear();
		}

	public:
		/**
		 * ע��һ�����¼���Ӧ��
		 * param:
		 *		eventID:		�¼�id
		 *		pEI:			��Ӧ����
		 *		key:			�¼���Ӧ�����û�����Key
		 * return:
		 *		����ɹ��������¼���Ӧ����Ψһ��ʶid�����򷵻�0
		 */
		DWORD RegEventHandler(KEventID eventID, IEventInterface* pEI, uint_r key = 0, uint_r customData = 0)
		{
			ASSERT_RETURN(pEI, 0);
			
			int pos = 0;
			ASSERT_RETURN(ToPos(eventID, pos), 0);
			
			DWORD id = KEventHandlerIDAllocator::Alloc();
			ASSERT_RETURN(id, 0);

			KEventHandler* pEH = KEventHandlerAllocator::Alloc();
			ASSERT_RETURN(pEH, 0);
			pEH->m_bDeleteFlag = false;
			pEH->m_dwCustomData = customData;
			pEH->m_dwKey = key;
			pEH->m_dwID = id;
			pEH->m_pEI = pEI;
			
			this->InsertEventList(pos, pEH);
			return id;
		}

		/**
		 * ��Ӧ�¼�
		 * param:
		 *		eventID:		�¼�id
		 *		pEventData:		�¼��Ĺ�������
		 */
		void OnEvent(const KEventData* pEventData)
		{
			if (m_handlingEventStack >= MAX_EVENT_STACKS)
			{
				//DUMP_CORE_ONLY_ONCE_BEGIN;
				Log(LOG_ERROR, "KEventHolder::OnEvent �ݹ鳬�����޲�Σ�");
				//DUMP_CORE_ONLY_ONCE_END;
				return;
			}

			ASSERT_RETURN_VOID(pEventData);
			KEventID eventID = pEventData->GetEventID();

			int pos = 0;
			
			ASSERT_RETURN_VOID(ToPos(eventID, pos));

			m_handlingEventStack++;

			KEventHandler* pFront = NULL;
			KEventHandler* pT = m_Events[pos];

			while(pT)
			{
				if (m_handlingEventStack == 1)	//����㣬���Զ��¼�����������ɾ������
				{
					while (pT && pT->m_bDeleteFlag)
					{
						if (pFront == NULL)
						{
							m_Events[pos] = pT->m_pNext;
							KEventHandlerAllocator::Free(pT);
							pT = m_Events[pos];
						}
						else
						{
							pFront->m_pNext = pT->m_pNext;
							KEventHandlerAllocator::Free(pT);
							pT = pFront->m_pNext;
						}
					}
					if (pT == NULL)
					{
						break;
					}
				}
				if (!pT->m_bDeleteFlag)
				{
					bool bRet = pT->m_pEI->OnEvent(eventID, pEventData, pT->m_dwCustomData);
					if (bRet)
					{
						pT->m_bDeleteFlag = true;
					}
				}

				pFront = pT;
				pT = pT->m_pNext;
			}
			m_handlingEventStack--;
		}

		/**
		 * ����EventID��Key�Ƴ����е���ӦEventID����key�������¼���Ӧ��
		 * param:
		 *		eventID:	�¼�id
		 *		key:		keyֵ(Ϊ0��Ϊ�Ƴ�ȫ��)
		 */
		void RemoveEventHandler(KEventID eventID, uint_r key)
		{
			int pos = 0;
			ASSERT_RETURN_VOID(ToPos(eventID, pos));
			KEventHandler* pEH = m_Events[pos];
			while (pEH)
			{
				if (pEH->m_dwKey == key || key == 0)
				{
					pEH->m_bDeleteFlag = true;
				}
				pEH = pEH->m_pNext;
			}
		}

		/**
		 * ����Key�Ƴ����е�����������¼���Ӧ��
		 * param:
		 *		key:		keyֵ
		 */
		void RemoveEventHandlerByKey(uint_r key)
		{
			for (int i = 0; i < EVENT_SIZE; i++)
			{
				KEventHandler* pEH = m_Events[i];
				while (pEH)
				{
					if (pEH->m_dwKey == key)
					{
						pEH->m_bDeleteFlag = true;
					}
					pEH = pEH->m_pNext;
				}
			}
		}

		/**
		 * ����idɾ��Ψһһ��Trigger
		 * param:
		 *		id:			idֵ
		 */
		void RemoveEventHandlerByID(DWORD id)
		{
			for (int i = 0; i < EVENT_SIZE; i++)
			{
				KEventHandler* pEH = m_Events[i];
				while (pEH)
				{
					if (pEH->m_dwID == id)
					{
						pEH->m_bDeleteFlag = true;
					}
					pEH = pEH->m_pNext;
				}
			}
		}

		/**
		 * �����¼���������ָ���Ƴ����и�key��ص���Ӧ�������key��0���Ƴ�����
		 * param:
		 *		pEI:		�¼���Ӧ��
		 *		key:		keyֵ
		 *		eventID:	�¼�id
		 */
		void RemoveEventHandler(IEventInterface* pEI, uint_r key = 0, KEventID eventID = enumInvalidEvent)
		{
			int pos = 0;
			if (eventID != enumInvalidEvent)
			{
				ASSERT_RETURN_VOID(ToPos(eventID, pos));
				KEventHandler* pEH = m_Events[pos];
				while (pEH)
				{
					if (pEH->m_pEI == pEI && (key == 0 || pEH->m_dwKey == key))
					{
						pEH->m_bDeleteFlag = true;
					}
					pEH = pEH->m_pNext;
				}
			}
			else
			{
				for (int i = 0; i < EVENT_SIZE; i++)
				{
					KEventHandler* pEH = m_Events[i];
					while (pEH)
					{
						if (pEH->m_pEI == pEI && (key == 0 || pEH->m_dwKey == key))
						{
							pEH->m_bDeleteFlag = true;
						}
						pEH = pEH->m_pNext;
					}
				}
			}
		}

		// ����
		void Breathe()
		{
			//if (NOW > m_lastBreatheTime)
			{
				CheckAndRemoveOnce();
				// 5-10���Ӽ��һ��
				m_lastBreatheTime += 5000;//1000 * 60 * 5 + rand() % 300;
			}
		}

	private:
		// ��¼�����¼�������
		KEventHandler* m_Events[EVENT_SIZE];
		// ��¼�����¼��������������һ��������ܾ�������LOG��
		int m_handlingEventStack;
		// �ϴκ���ʱ��
		DWORD m_lastBreatheTime;

		class KEventHandlerAllocator : public System::Memory::KPortableMemoryPool<KEventHandler, 8192>{};

		/**
		 * ����¼��б�
		 */
		void Clear()
		{
			if (m_handlingEventStack == 0)	//���¼������У�ֱ�����ȫ��
			{
				for (int i = 0; i < EVENT_SIZE; i++)
				{
					while (m_Events[i] != NULL)
					{
						KEventHandler* pEH = m_Events[i];
						m_Events[i] = pEH->m_pNext;
						KEventHandlerAllocator::Free(pEH);
					}
					m_Events[i] = NULL;
				}
			}
			else
			{
				//��Ӧ��������״̬����
				//DUMP_CORE_ONLY_ONCE;
				//ȫ�����ɾ��
				for (int i = 0; i < EVENT_SIZE; i++)
				{
					KEventHandler* pEH = m_Events[i];
					while (pEH)
					{
						pEH->m_bDeleteFlag = true;
						pEH = pEH->m_pNext;
					}
				}
			}
		}
		
		// ���¼����������뵽�¼��б�
		void InsertEventList(int eventPos, KEventHandler* pOneHolder)
		{
			pOneHolder->m_pNext = m_Events[eventPos];
			m_Events[eventPos] = pOneHolder;

		}

		// �����¼�idӳ�䵽λ��
		bool ToPos(KEventID eventID, int& pos)
		{
			int npos = (int)eventID - EVENT_BASE_ID;
			if (npos >= 0 && npos < EVENT_SIZE)
			{
				pos = npos;
				return true;
			}
			return false;
		}

		// ���ڼ�顢ɾ����Ч���¼�ע��
		void CheckAndRemoveOnce()
		{
			for (int pos = 0; pos < EVENT_SIZE; pos++)
			{
				KEventHandler* pFront = NULL;
				KEventHandler* pT = m_Events[pos];

				while(pT)
				{
					if (pT->m_bDeleteFlag)
					{
						if (pFront == NULL)
						{
							m_Events[pos] = pT->m_pNext;
							KEventHandlerAllocator::Free(pT);
							pT = m_Events[pos];
						}
						else
						{
							pFront->m_pNext = pT->m_pNext;
							KEventHandlerAllocator::Free(pT);
							pT = pFront->m_pNext;
						}
					}
					else
					{
						pFront = pT;
						pT = pT->m_pNext;
					}
				}
			}
		}
	};
}
