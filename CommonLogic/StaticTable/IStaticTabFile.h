/********************************************************************
created:	2011/2/28
filename: 	CommonLogic\StaticTable\IStaticTabFile.h
file path:	CommonLogic\StaticTable
file base:	IStaticTabFile
file ext:	h
author:		zaki
copyright:	AuroraGame

purpose:	
*********************************************************************/
#pragma once

#include "System/KType.h"

namespace StaticTabFileAbout
{
	class IStaticTabFile
	{
	public:
		IStaticTabFile();
		virtual ~IStaticTabFile();
		virtual bool Reload(const char * szFilePath) = 0;
	};
};