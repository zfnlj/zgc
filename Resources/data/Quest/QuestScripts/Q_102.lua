﻿-- Script for Quest 102 每日任务
-- [doneTalk=0;cate=0;failTalk=0;note=0;viewTalk=0;hard2=0]
Quest_102 = 
{
Initialize = function(quest)
	quest:SetAttrs("prevQuest=0;abandon=0;acceptLevel=1;level=1;exp=24;money=12;");
	quest:SetAttrs("nameId=102;talk=0;desc=102;");
	quest:SetAttrs("type=3;reuse=0;hardDegree=2;achieveId=0;");
	quest:SetAttrs("nextQuest=0;achievement=0;timeperiod=0;");
	quest:SetAttrs("battleField=102;");
	
	-- Group 0
	local group0 = quest:CreateTargetGroup("and");
        group0:CreateTarget("KillMonster", "npc=102;count=1;");
  -- end groups
	return true;
end,

UseGift = function(quest,player)
	if quest:RemainDailyQuestNum()>0 then
			return "5102"; 
	else
			return "";
	end
end, 

QName = function (quest , player)
	return "daily mission2";
end,  


GiftDesc = function (quest , player)
  local exp
	local money
	local itemlist
		exp = 120			-- 经验奖励
		money = 60	  	-- 金钱奖励
	return GetQuestGiftStr(itemlist,exp,money)
end, 
  
}