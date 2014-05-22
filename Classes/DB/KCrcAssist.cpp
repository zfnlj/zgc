#include "KCrcAssist.h"

void KCrcAssist::MakeTable(unsigned short gEnpoly)  //�������ܣ�����0��255��Ӧ��16CRCУ���룬��ʵ���Ǽ�����㷨1���������㷨��  //gEnpolyΪ���ɶ���ʽ  //ע�⣬��λ�ȴ���ʱ�����ɶ���ʽӦ��ת(��λ���λ����)����CRC16-CCITTΪ0x1021����ת��Ϊ0x8408
{  
	unsigned short cRc_16=0; 
	unsigned short i,j,k; 
	for(i=0,k=0;i<256;i++,k++) {        
		cRc_16 = i<<8;       
		for(j=8;j>0;j--)       
		{  
			if(cRc_16&0x8000)                 //��תʱcRc_16&0x0001
				cRc_16=(cRc_16<<=1)^gEnpoly;  // �� ת ʱ cRc_16=(cRc_16>>=1)^gEnpoly          
			else 
				cRc_16<<=1;                   //��תʱcRc_16>>=1         //��ʾ����  cRc_16 = cRc_16<<1;       
		}
		m_cRctable_16[k] = cRc_16;

	}

}

unsigned short KCrcAssist::GetCrc_16(unsigned char * pData, int nLength) //�������ܣ�����������* pData��16λCRCУ���룬����������ΪnLength 
{      
	unsigned short cRc_16 = 0x0000;    // ��ʼ��          
	while(nLength>0){          
		cRc_16 = (cRc_16 << 8) ^ m_cRctable_16[((cRc_16>>8) ^ *pData) & 0xff]; //cRctable_16���ɺ���mK_cRctable���� 
		nLength--;
		pData++; 

	}
	return cRc_16;    
}  
