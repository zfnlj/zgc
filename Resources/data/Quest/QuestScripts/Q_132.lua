﻿

Quest_132 = 
{
Initialize = function(quest)
	quest:SetAttrs("prevQuest=0;abandon=0;acceptLevel=1;level=1;exp=180;money=80;");
	quest:SetAttrs("nameId=132;talk=0;desc=132;finishDesc=1500511;");
	quest:SetAttrs("type=3;reuse=0;hardDegree=1;achieveId=0;");
	quest:SetAttrs("nextQuest=0;achievement=0;timeperiod=0;");
	quest:SetAttrs("battleField=132;");
	
	-- Group 0
	local group0 = quest:CreateTargetGroup("and");
        group0:CreateTarget("KillMonster", "npc=132;count=1;");
  -- end groups
	return true;
end,

QName = function (quest , player)
	return "daily mission4_1";
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