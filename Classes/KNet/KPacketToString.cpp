#include "KPacketToString.h"
#include <sstream>
#include <iomanip>
#include "System/Misc/KStream.h"

namespace KPacketAbout
{
	std::string ToString(int cmd, const void* data, int len)
	{
		std::stringstream ss;

		KMsgInputStream is;
		is.Attach((void*)data, len);
		switch(cmd)
		{
		default:
			{
				ss << "Head code: " << cmd;
				const unsigned char* ca = (const unsigned char*)data;
				for (int i = 0; i < len; i++)
				{
					if ((i & 15) == 0)
					{
						ss << std::endl;
					}
					else if ((i & 7) == 0)
					{
						ss << "| ";
					}
					ss << std::setw(2) << std::setfill('0') << std::hex << (int)ca[i] << " ";
				}
				ss << std::endl;
			}
		}
		return ss.str();
	}
}