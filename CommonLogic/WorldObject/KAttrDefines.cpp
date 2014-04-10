#include "KAttrDefines.h"

namespace KAttributeAbout {

	int CompactIndex(int idx, int param)
	{
		return -(idx * 10 + param);
	}
	void DecompackIndex(int compackedIdx, int& idx, int& param)
	{
		idx = -compackedIdx / 10;
		param = compackedIdx % 10;
	}
}
