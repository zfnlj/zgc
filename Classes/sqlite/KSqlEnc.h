#ifndef _KSQLENC_H
#define _KSQLENC_H

class KSqlEnc
{
public:
	KSqlEnc(){}
	~KSqlEnc(){}
	static bool Enc(unsigned char* buf,unsigned char* out,int len);
	static bool Dec(unsigned char* in,unsigned char* out,int len);
};

#endif // __HELLOWORLD_SCENE_H__
