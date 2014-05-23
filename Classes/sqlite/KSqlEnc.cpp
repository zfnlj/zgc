#include "KSqlEnc.h"
#include "System/Cipher/tea.h"
#include "../Inc/KTypeDef.h"

static BYTE key[16]="zhufanan77";
bool KSqlEnc::Enc(unsigned char* buf,unsigned char* out,int len)
{
	//const int SIZE_IN = 8, SIZE_OUT = 8, SIZE_KEY = 16;

	if( (len%8) !=0 ) {
		CCAssert(false , "Error Enc size!");
		return false;
	}
	TEA tea(key, 16);
	int n = len/8;
	unsigned char* pIn = buf;
	unsigned char* pOut = out;
	for(int i=0;i<n;i++){
		tea.encrypt(pIn, pOut);
		pIn += 8;
		pOut+= 8;
	}
	return true;
}

bool KSqlEnc::Dec(unsigned char* in,unsigned char* out,int len)
{
	if( (len%8) !=0 ){
		CCAssert(false , "Error Dec size!");
		return false;
	}
	TEA tea(key, 16);
	int n = len/8;
	unsigned char* pIn = in;
	unsigned char* pOut = out;
	for(int i=0;i<n;i++){
		tea.decrypt(pIn,pOut);
		pIn += 8;
		pOut+= 8;
	}
	return true;
}