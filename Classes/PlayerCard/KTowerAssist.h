#ifndef _KTOWERASSIST_H
#define _KTOWERASSIST_H

class KDeckDefStatic;
struct tb_player_record;
namespace KTowerAssist
{
	void _reset(tb_player_record* record);
	void _stepOn(tb_player_record* record);
	bool _isShow(tb_player_record* record);
	int _getLayer();
	bool _active(tb_player_record* record,int curLayer);
	void _create(tb_player_record* record);
	void _deactive(tb_player_record* record);
	int _calcMoney(tb_player_record* record);
	int _calcExp(tb_player_record* record);
	int _calcFateStone(tb_player_record* record);
	int _getPos(tb_player_record* record);
	KDeckDefStatic* _createDeckDef(KDeckDefStatic* pDeck);
	void _win(int questId);
}

#endif // 
