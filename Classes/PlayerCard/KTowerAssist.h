#ifndef _KTOWERASSIST_H
#define _KTOWERASSIST_H

struct tb_player_record;
namespace KTowerAssist
{
	void _reset(tb_player_record* record);
	void _stepOn(tb_player_record* record);
	bool _isShowTower(tb_player_record* record);
	int _getTowerLayer(tb_player_record* record);
}

#endif // 
