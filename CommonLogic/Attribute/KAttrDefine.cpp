#include "KAttrDefine.h"

namespace KAttributeAbout
{
	int CompactIndex(int idx, int param)
	{
		return -(idx * 10 + param);
	}
	void DecompackIndex(int compackedIdx, int& idx, int& param)
	{
		idx = -compackedIdx / 10;
		param = -compackedIdx % 10;
	}

	bool IsCompositeAttr( int idx )
	{
		switch(idx)
		{
		case ca_Strength:
		case ca_Stamina:
		case ca_Agility:
		case ca_Will:
// 		case ca_Skill:
// 		case ca_BpAttack:
// 		case ca_Defense:
			return true;
		}
		return false;
	}
}