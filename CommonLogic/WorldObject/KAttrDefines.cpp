#include "KAttrDefines.h"

namespace KAttributeAbout {

	int CompactIndex2(int idx, int param)
	{
		return -(idx * 10 + param);
	}
	void DecompackIndex2(int compackedIdx, int& idx, int& param)
	{
		idx = -compackedIdx / 10;
		param = compackedIdx % 10;
	}
}
