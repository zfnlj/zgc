﻿-- 
-- [doneTalk=0;cate=0;failTalk=0;note=0;viewTalk=0;hard2=0]
item_5102 = 
{
Initialize = function(KUseItem)
	return true;
end,
run = function(KUseItem,playerId )
  local rndVal = KUseItem:RndVal();
  if rndVal <= 2 then	
  	KUseItem:GenerateHero(playerId,10001);
  elseif rndVal <= 30 then 
   	KUseItem:GenerateItem(playerId,100010,1);
  end
end,
}