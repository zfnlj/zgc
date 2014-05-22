#ifndef _CRCASSIST_H
#define _CRCASSIST_H


class KCrcAssist
{
public:
	KCrcAssist(){}
	~KCrcAssist(){}
	void init(){
		MakeTable((unsigned short)0x11021);
	}
	unsigned short GetCrc_16(unsigned char * pData, int nLength);
	void MakeTable(unsigned short gEnpoly);
private:
	unsigned short m_cRctable_16[256];
};

#endif // __HELLOWORLD_SCENE_H__
