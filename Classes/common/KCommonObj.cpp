

#include "KCommonObj.h"


System::Random::KRandom g_rnd;

KMsgOutputStream g_ostream;


void _CopyIntegerList(KIntegerList& src,KIntegerList& des)
{
	for(KIntegerList::iterator it=src.begin();it!=src.end();it++){
		des.push_back(*it);
	}
}

void _RndIntegerList(KIntegerList& src,KIntegerList& des)
{
	KIntegerList tmpLst;
	_CopyIntegerList(src,tmpLst);
	while(!tmpLst.empty()){
		int nRand = g_rnd.GetRandom(0,tmpLst.size());
		int pos=0;
		for(KIntegerList::iterator it=tmpLst.begin();it!=tmpLst.end();it++,pos++){
			if(pos==nRand){
				des.push_back(*it);
				tmpLst.erase(it);
				break;
			}
		}
	}
}

void _RndPick(KIntegerList& src,KIntegerList& des,int num)
{
	if(src.size()<num) return;
	KIntegerList tmpLst;
	_RndIntegerList(src,tmpLst);
	int count=0;
	for(KIntegerList::iterator it=tmpLst.begin();it!=tmpLst.end();it++,count++)
	{
		if(count==num) break;
		des.push_back(*it);
	}
}