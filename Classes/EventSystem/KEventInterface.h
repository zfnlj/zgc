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
	// 事件处理接口
	class IEventInterface
	{
	public:
		virtual ~IEventInterface() {};
	public:
		/**
		 * 事件响应
		 * param:
		 *		id:			事件id
		 *		pData:		事件发生时的环境
		 *		customData:	用户自定义数据
		 * return:
		 *		该事件是否处理完成(以后还会不会处理此事件)，如果完成(以后不处理了)需要返回true，否则false
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
		 * 事件响应的实现部分
		 * return:
		 *		该事件是否处理完成(以后还会不会处理此事件)，如果完成(以后不处理了)需要返回true，否则false
		 */
		virtual bool OnEventImp(KEventID id, const KEventData* pData, uint_r customData) = 0;
		
		/**
		 * 事件过滤器
		 * 根据事件参数决定是否要处理该事件
		 * 如果需要处理则返回true，否则false
		 */
		virtual bool Filter(KEventID id, const KEventData* pData, uint_r customData) { return true; }
	};
};

