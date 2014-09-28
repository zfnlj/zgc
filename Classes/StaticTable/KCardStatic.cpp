#include "KCardStatic.h"

KCardStatic::CardRace KCardStatic::getCardRace(const char* str)
{
	if(strcmp(str,"gold")==0){
		return KCardStatic::race_gold;
	}else if(strcmp(str,"tree")==0){
		return KCardStatic::race_tree;
	}else if(strcmp(str,"water")==0){
		return KCardStatic::race_water;
	}else if(strcmp(str,"fire")==0){
		return KCardStatic::race_fire;
	}else if(strcmp(str,"mud")==0){
		return KCardStatic::race_mud;
	}else if(strcmp(str,"summon")==0){
		return KCardStatic::race_summon;
	}else{
		return KCardStatic::race_null;
	}
}

void KCardStatic::SetID(int val)
{
	m_Id = val;
	m_type =  (CardDef)(m_Id/10000);
}

bool KCardStatic::init()
{
	memset(m_Name,0,sizeof(m_Name));
	memset(m_Desc,0,sizeof(m_Desc));
	memset(m_Detail,0,sizeof(m_Detail));
	memset(m_Show,0,sizeof(m_Show));
	return true;
}


KCardStatic* KCardStatic::create()
{
	KCardStatic* card = new KCardStatic;
	card->init();
	return card;
}

void KCardStatic::Init(System::File::KTabFile2* fileReader)
{
	char buf[64];

	int id;
	fileReader->GetInteger("ID", 0, (int*)&id);
	SetID(id);
	fileReader->GetString("Name", "", m_Name, MAX_CARD_NAME);
	fileReader->GetInteger("Atk", 0, (int*)&m_Atk);
	fileReader->GetInteger("Def", 0, (int*)&m_Def);
	fileReader->GetInteger("Cost", 0, (int*)&m_Cost);
	fileReader->GetInteger("Hp", 0, (int*)&m_Hp);
	fileReader->GetInteger("Rank", 1, (int*)&m_Rank);
	fileReader->GetString("Desc", "", m_Desc, MAX_CARD_DESC_LEN);
	fileReader->GetString("Detail", "", m_Detail, MAX_CARD_DETAIL_LEN);

	fileReader->GetString("Race", "", buf, MAX_CARD_NAME);
	m_Race = getCardRace(buf);
	fileReader->GetString("Show", "", buf, 63);
	if(strlen(buf)>2) strcpy(m_Show,buf);
}