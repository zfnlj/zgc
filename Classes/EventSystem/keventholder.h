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
	 * 事件处理对象的id分配器
	 * 考虑到一个DWORD足够大，就不回收了，直接累加
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

	// 事件栈的最大递归层数
	const static int MAX_EVENT_STACKS = 10;

	/**
	 * 事件管理
	 */
	template<DWORD EVENT_SIZE, int EVENT_BASE_ID>
	class KEventHolder
	{
	private:
		/**
		 * 事件触发器的实例结构
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
		 * 注册一个新事件响应器
		 * param:
		 *		eventID:		事件id
		 *		pEI:			响应对象
		 *		key:			事件响应器的用户定义Key
		 * return:
		 *		如果成功，返回事件响应器的唯一标识id，否则返回0
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
		 * 响应事件
		 * param:
		 *		eventID:		事件id
		 *		pEventData:		事件的关联数据
		 */
		void OnEvent(const KEventData* pEventData)
		{
			if (m_handlingEventStack >= MAX_EVENT_STACKS)
			{
				//DUMP_CORE_ONLY_ONCE_BEGIN;
				Log(LOG_ERROR, "KEventHolder::OnEvent 递归超出上限层次！");
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
				if (m_handlingEventStack == 1)	//最外层，可以对事件进行真正的删除处理
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
		 * 根据EventID和Key移除所有的响应EventID的与key关联的事件响应器
		 * param:
		 *		eventID:	事件id
		 *		key:		key值(为0认为移除全部)
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
		 * 根据Key移除所有的与其关联的事件响应器
		 * param:
		 *		key:		key值
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
		 * 根据id删除唯一一个Trigger
		 * param:
		 *		id:			id值
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
		 * 根据事件处理对象的指针移除所有跟key相关的响应器，如果key填0则移除所有
		 * param:
		 *		pEI:		事件响应器
		 *		key:		key值
		 *		eventID:	事件id
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

		// 呼吸
		void Breathe()
		{
			//if (NOW > m_lastBreatheTime)
			{
				CheckAndRemoveOnce();
				// 5-10分钟检查一次
				m_lastBreatheTime += 5000;//1000 * 60 * 5 + rand() % 300;
			}
		}

	private:
		// 记录所有事件的链表
		KEventHandler* m_Events[EVENT_SIZE];
		// 记录处理事件层数，如果超过一定层数则拒绝处理，出LOG。
		int m_handlingEventStack;
		// 上次呼吸时间
		DWORD m_lastBreatheTime;

		class KEventHandlerAllocator : public System::Memory::KPortableMemoryPool<KEventHandler, 8192>{};

		/**
		 * 清除事件列表
		 */
		void Clear()
		{
			if (m_handlingEventStack == 0)	//非事件处理中，直接清除全部
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
				//不应该在这种状态调用
				//DUMP_CORE_ONLY_ONCE;
				//全部标记删除
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
		
		// 将事件处理器插入到事件列表
		void InsertEventList(int eventPos, KEventHandler* pOneHolder)
		{
			pOneHolder->m_pNext = m_Events[eventPos];
			m_Events[eventPos] = pOneHolder;

		}

		// 根据事件id映射到位置
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

		// 定期检查、删除无效的事件注册
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
