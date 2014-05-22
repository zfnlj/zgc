/********************************************************************
created:	2011/5/18
manager:	Allen Wang
copyright:	Aurora Interactive, 2011
*********************************************************************/

#pragma once

namespace System
{

namespace Algorithm
{
	/**
	 * 角度计算相关
	 */

	class AngleComputer
	{
	public:
		/**
		 * 根据原点和要看到的点计算角度
		 */
		static float GetFaceAngle(float fromx, float fromy, float tox, float toy);
	};
}

}