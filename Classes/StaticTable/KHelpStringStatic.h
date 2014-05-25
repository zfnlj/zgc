#ifndef _KHELPSTRINGSTATIC_H
#define _KHELPSTRINGSTATIC_H

#include <System/Collections/DynArray.h>
#include "System/Collections/KMapByVector.h"
#include <list>
#include "../common/KCommonObj.h"
#include <System/File/KTabfileLoader.h>
#include <string>
class KHelpStringStatic
{
public:
	virtual bool init();
	static KHelpStringStatic* create();
	void Init(System::File::KTabFile2* fileReader);
	const char* GetString(){ return m_des.c_str();}
private:
	int m_Id;
	std::string m_des;
	std::string m_author;
	friend class KGameStaticMgr;
};
typedef System::Collections::KMapByVector<int,KHelpStringStatic*> KHelpStringMap;

#endif // 
