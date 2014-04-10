#include "KMsgHub.h"

template<> KMsgHub* Singleton<KMsgHub>::mSingleton = NULL;


KMsgHub::KMsgHub()
{

}

KMsgHub::~KMsgHub()
{

}

KMsgHub& KMsgHub::getSingleton(void)
{
	if(!mSingleton){
		mSingleton = new KMsgHub;
	}
	return ( *mSingleton ); 
}