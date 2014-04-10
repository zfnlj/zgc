#ifndef _KMYSTOREPANEL_H
#define _KMYSTOREPANEL_H

#include "cocos2d.h"
#include "UILayer.h"
#include "../common/KPlayerRecordAssist.h"
#include "../GameLogic/KCardInst.h"
USING_NS_CC;

#define MAX_SLOT_NUM 8

namespace KStoreAbout{
	class KStoreCreateInfo;
};

class KMyStorePanel :public CCObject
{
	enum view_filter{
		view_default,
		view_cost,
		view_race,
		view_null,
	};
public:
	KMyStorePanel();
	~KMyStorePanel();

	void init(cocos2d::extension::UILayer* layer);
	void DoClickClose(CCObject* sender);
private:
	cocos2d::extension::UILayer* m_layer;
	cocos2d::extension::UIWidget* m_Panel;

	void InitCard();
	void pagePrev();
	void pageNext();
	KPlayerRecordAssist::DBBagItemSortArray m_sortStoreCard;
	void updateCardPanel();
	void FillFilterCard(view_filter filter);
	bool IsMatchFilter(KDBBagItemUnit*,view_filter filter,int param);
	view_filter m_curFilter;
	int m_curFilterParam;
	int m_curPage;
	typedef std::vector<KDBBagItemUnit*> DBBagCardVect;
	DBBagCardVect m_filterCardVect;

	KCardInst m_ShowCard[MAX_SLOT_NUM];
};

#endif // __BattleFieldScene_SCENE_H__
