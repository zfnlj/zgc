

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

time_t _GetSystemTimeVal()
{
	time_t timep; 
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
    time(&timep); 
#else  
    struct cc_timeval now;  
    CCTime::gettimeofdayCocos2d(&now, NULL);  
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

void _GetTime(int level) 
{ 
	time_t timep = _GetSystemTimeVal();
    struct tm *tm; 

 
 
    tm = localtime(&timep); 
    int year = tm->tm_year + 1900; 
    int month = tm->tm_mon + 1; 
    int day = tm->tm_mday; 
    int hour=tm->tm_hour; 
    int min=tm->tm_min; 
    int second=tm->tm_sec; 
	int kk = 0;
} 

