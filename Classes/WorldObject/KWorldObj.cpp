
#include "KWorldObj.h"

namespace KWorldObjAbout

{
	KWorldObj::KWorldObj( void ) : m_id(0)
		, m_nFlag(wokWorldObj)
		, m_fX(0.0f)
		, m_fY(0.0f)
		, m_fZ(0.0f)
		, m_bValid(true)
	{

	}

	void KWorldObj::OnEnterGame()
	{
		SetValid(true);
	}

	void KWorldObj::OnLeaveGame(bool onClearWorld /*= false*/)
	{
		SetValid(false);
	}
}
