/********************************************************************
created:	2011/3/10
manager:	Allen Wang
copyright:	Aurora Interactive, 2011
*********************************************************************/
#pragma once
#include "System/KType.h"
#include "System/Patterns/KSingleton.h"
#include "System/Collections/KMapByVector.h"
#include "System/Misc/KStream.h"

namespace KAttributeAbout
{
	// AttrHolder�ӿ�
	class IAttrHolder
	{
	public:
		virtual ~IAttrHolder(){}

	public:
		virtual void Reset() = 0;
		// �������ֵ
		virtual int GetAttrValue(int idx) const = 0;
		// �趨����ֵ
		virtual void SetAttrValue(int idx, int val, int param) = 0;
		// ��������ֵ
		virtual void IncreaseAttrValue(int idx, int val, int param) = 0;
	};
	/**
	 * 1�������Ļ����ӿ�
	 */
	template<DWORD ATTR_SIZE, int BASE_ATTR>
	class KCommonAttrHolder : public IAttrHolder
	{
	public:
		KCommonAttrHolder()
		{
			memset(m_data, 0, sizeof(m_data));
		}

	public:
		virtual void Reset()
		{
			memset(m_data, 0, sizeof(m_data));
		}

		virtual int GetAttrValue(int idx) const
		{
			idx -= BASE_ATTR;
			ASSERT_RETURN(idx >= 0 && idx < ATTR_SIZE, 0);
			return m_data[idx];
		}
		virtual void SetAttrValue(int idx, int val, int param)
		{
			idx -= BASE_ATTR;
			ASSERT_RETURN_VOID(idx >= 0 && idx < ATTR_SIZE);
			if (param == 0)
			{
				m_data[idx] = val;
			}
			else
			{
				ASSERT_I(false);
			}
		}
		// ���������еĹ�ʽ����
		virtual void IncreaseAttrValue(int idx, int val, int param)
		{
			idx -= BASE_ATTR;
			ASSERT_RETURN_VOID(idx >= 0 && idx < ATTR_SIZE);
			if (param == 0)
			{
				m_data[idx] += val;
			}
			else
			{
				ASSERT_I(false);
			}
		}

		template<typename T>
		void VisitAll(T& v)
		{
			for (int i = 0; i < ATTR_SIZE; i++)
			{
				v(i + BASE_ATTR);
			}
		}

	private:
		int m_data[ATTR_SIZE];
	};

	/**
	 * 2�������Ļ�����ʽ�ӿ�	Result = (Base + P1) * (1 + P2)
	 */
	template<DWORD ATTR_SIZE, int BASE_ATTR>
	class K2ParamAttrHolder : public IAttrHolder
	{
	public:
		K2ParamAttrHolder()
		{
			memset(m_data, 0, sizeof(m_data));
		}

	public:
		virtual void Reset()
		{
			memset(m_data, 0, sizeof(m_data));
		}

		virtual int GetAttrValue(int idx) const
		{
			idx -= BASE_ATTR;
			ASSERT_RETURN(idx >= 0 && idx < ATTR_SIZE, 0);
			return m_data[idx].currentVal;
		}
		virtual void SetAttrValue(int idx, int val, int param)
		{
			idx -= BASE_ATTR;
			ASSERT_RETURN_VOID(idx >= 0 && idx < ATTR_SIZE);
			switch(param)
			{
			case 0:	//����ֵ
				m_data[idx].baseVal = val;
				break;
			case 1:
				m_data[idx].p1 = val;
				break;
			case 2:
				m_data[idx].p2 = val;
				break;
			default:
				ASSERT_I(false);
			}
			m_data[idx].RecomputeAttr();
		}
		virtual void IncreaseAttrValue(int idx, int val, int param)
		{
			idx -= BASE_ATTR;
			ASSERT_RETURN_VOID(idx >= 0 && idx < ATTR_SIZE);
			switch(param)
			{
			case 0:	//����ֵ
				m_data[idx].baseVal += val;
				break;
			case 1:
				m_data[idx].p1 += val;
				break;
			case 2:
				m_data[idx].p2 += val;
				break;
			default:
				ASSERT_I(false);
			}
			m_data[idx].RecomputeAttr();
		}

		template<typename T>
		void VisitAll(T& v)
		{
			for (int i = 0; i < ATTR_SIZE; i++)
			{
				v(i + BASE_ATTR);
			}
		}

	private:
		struct
		{
			int baseVal;
			int p1;
			int p2;
			int currentVal;
			inline void RecomputeAttr()
			{
				// Result = (Base + P1) * (1 + P2)
				// ʹ��int64��ֹ���
				currentVal = (int)(((INT64)baseVal + (INT64)p1) * (1000 + (INT64)p2) / 1000);
			}
		} m_data[ATTR_SIZE];
	};
}
