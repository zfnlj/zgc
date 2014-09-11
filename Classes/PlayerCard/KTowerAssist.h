#ifndef _KTOWERASSIST_H
#define _KTOWERASSIST_H

struct tb_player_record;
namespace KTowerAssist
{
	void _reset(tb_player_record* record);
	void _stepOn(tb_player_record* record);
	bool _isShow(tb_player_record* record);
	int _getLayer(tb_player_record* record);
	bool _active(tb_player_record* record,int curLayer);
	void _create(tb_player_record* record);
}

#endif // 
