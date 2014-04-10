/********************************************************************
created:	2011/8/1
manager:	Allen Wang
copyright:	Aurora Interactive, 2011
*********************************************************************/

#pragma once

// 属性改变的事件处理接口
class IAttrChangeSensor
{
public:
	virtual ~IAttrChangeSensor(){}
public:
	virtual void OnAttrChange(int idx, int oldVal, int newVal) = 0;
};