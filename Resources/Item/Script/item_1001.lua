﻿-- Script for Quest 10001 摸底考试
-- [doneTalk=0;cate=0;failTalk=0;note=0;viewTalk=0;hard2=0]
item_1001 = 
{
Initialize = function(KUseItem)
	--KUseItem:SetAttrs("total=3;rank=1;soldier=1;skill=1;secret=1;");
	return true;
end,
run = function(KUseItem,playerId )
	--(UINT64 playerId,int count,int rank1Rate,int rank2Rate,int rank3Rate,heroRate)
	KUseItem:GeneratNormalCard(playerId,3,95,0,0,5); 
end,
}