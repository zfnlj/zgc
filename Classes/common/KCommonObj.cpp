

#include "KCommonObj.h"


System::Random::KRandom g_rnd;

KMsgOutputStream g_ostream;


void _CopyIntegerList(KIntegerList& src,KIntegerList& des,int rate)
{
	for(KIntegerList::iterator it=src.begin();it!=src.end();it++){
		for(int i=0;i<rate;i++){
			des.push_back(*it);
		}
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

int  _RndPick(KIntegerList& src)
{
	if(src.empty()) return 0;
	KIntegerList tmpLst;
	_RndIntegerList(src,tmpLst);
	return *tmpLst.begin();
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

time_t _GetSystemTimeVal()
{
	time_t timep; 
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
    time(&timep); 
#else  
    struct cocos2d::cc_timeval now;
    cocos2d::CCTime::gettimeofdayCocos2d(&now, NULL);
    timep = now.tv_sec; 
#endif  
	return timep;
}

void _GetTimeInterval(time_t& t1,time_t& t2,tm& interval)
{
	time_t tt = t2 -t1;
	struct tm *tm = localtime(&tt);
	memcpy(&interval,tm,sizeof(interval));
}

int _RndNormal(int low,int height)
{
	int rndVal = 0;
	int num=0;
	while(num<20){
		int val = g_rnd.GetRandom(1,11);
		if(val>=5){
			rndVal += val;
			num++;
		}
	}
	return rndVal-100;
}