﻿-- Script for Quest 911 每日任务
-- [doneTalk=0;cate=0;failTalk=0;note=0;viewTalk=0;hard2=0]
Quest_911 = 
{
Initialize = function(quest)
	quest:SetAttrs("prevQuest=0;abandon=0;acceptLevel=1;level=1;exp=100;money=50;");
	quest:SetAttrs("nameId=101;talk=0;desc=101;finishDesc=1500511;");
	quest:SetAttrs("type=4;reuse=0;hardDegree=1;achieveId=0;");
	quest:SetAttrs("nextQuest=0;achievement=0;timeperiod=0;");
	quest:SetAttrs("battleField=101;");
	
	-- Group 0
	local group0 = quest:CreateTargetGroup("and");
        group0:CreateTarget("KillMonster", "npc=911;count=1;");
  -- end groups
	return true;
end,

QName = function (quest , player)
	return "tower mission";
end,  


GiftDesc = function (quest , player)
  local exp
	local money
	local itemlist
		exp = 60			-- 经验奖励
		money = 30	  	-- 金钱奖励
	return GetQuestGiftStr(itemlist,exp,money)
end, 
  
}