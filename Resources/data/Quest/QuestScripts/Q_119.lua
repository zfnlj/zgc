﻿

Quest_119 = 
{
Initialize = function(quest)
	quest:SetAttrs("prevQuest=0;abandon=0;acceptLevel=1;level=1;exp=96;money=48;");
	quest:SetAttrs("nameId=111;talk=0;desc=111;");
	quest:SetAttrs("type=3;reuse=0;hardDegree=19;achieveId=0;");
	quest:SetAttrs("nextQuest=0;achievement=0;timeperiod=0;");
	quest:SetAttrs("battleField=119;");
	
	-- Group 0
	local group0 = quest:CreateTargetGroup("and");
        group0:CreateTarget("KillMonster", "npc=119;count=1;");
  -- end groups
	return true;
end,

UseGift = function(quest,player)
	if quest:RemainDailyQuestNum()>0 then
			return "5119"; 
	else
			return "";
	end
end, 

QName = function (quest , player)
	return "daily mission19";
end,  


GiftDesc = function (quest , player)
  local exp
	local money
	local itemlist
		exp = 180			-- 经验奖励
		money = 80	  	-- 金钱奖励
	return GetQuestGiftStr(itemlist,exp,money)
end, 
  
}