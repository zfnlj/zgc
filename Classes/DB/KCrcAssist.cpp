#include "KCrcAssist.h"

void KCrcAssist::MakeTable(unsigned short gEnpoly)  //函数功能：生成0－255对应的16CRC校验码，其实就是计算机算法1（比特型算法）  //gEnpoly为生成多项式  //注意，低位先传送时，生成多项式应反转(低位与高位互换)。如CRC16-CCITT为0x1021，反转后为0x8408
{  
	unsigned short cRc_16=0; 
	unsigned short i,j,k; 
	for(i=0,k=0;i<256;i++,k++) {        
		cRc_16 = i<<8;       
		for(j=8;j>0;j--)       
		{  
			if(cRc_16&0x8000)                 //反转时cRc_16&0x0001
				cRc_16=(cRc_16<<=1)^gEnpoly;  // 反 转 时 cRc_16=(cRc_16>>=1)^gEnpoly          
			else 
				cRc_16<<=1;                   //反转时cRc_16>>=1         //表示的是  cRc_16 = cRc_16<<1;       
		}
		m_cRctable_16[k] = cRc_16;

	}

}

unsigned short KCrcAssist::GetCrc_16(unsigned char * pData, int nLength) //函数功能：计算数据流* pData的16位CRC校验码，数据流长度为nLength 
{      
	unsigned short cRc_16 = 0x0000;    // 初始化          
	while(nLength>0){          
		cRc_16 = (cRc_16 << 8) ^ m_cRctable_16[((cRc_16>>8) ^ *pData) & 0xff]; //cRctable_16表由函数mK_cRctable生成 
		nLength--;
		pData++; 

	}
	return cRc_16;    
}  
