﻿-- 
-- [doneTalk=0;cate=0;failTalk=0;note=0;viewTalk=0;hard2=0]
item_5013 = 
{
Initialize = function(KUseItem)
	return true;
end,
run = function(KUseItem,playerId )
  local rndVal = KUseItem:RndVal();
  if rndVal <= 10 then	
  	KUseItem:GenerateHero(playerId,10003);
  elseif rndVal <= 60 then 
   	KUseItem:GenerateItem(playerId,100030,3);
  end
end,
}