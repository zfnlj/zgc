﻿-- Script for Quest 10001 摸底考试
-- [doneTalk=0;cate=0;failTalk=0;note=0;viewTalk=0;hard2=0]
Quest_10002 = 
{
Initialize = function(quest)
	quest:SetAttrs("prevQuest=10001;abandon=0;acceptLevel=1;level=1;exp=12;money=6;");
	quest:SetAttrs("nameId=100010;talk=0;desc=100011;");
	quest:SetAttrs("type=1;reuse=0;hardDegree=1;achieveId=0;");
	quest:SetAttrs("nextQuest=10003;achievement=0;timeperiod=0;");
	quest:SetAttrs("battleField=2;");
	
	-- Group 0
	local group0 = quest:CreateTargetGroup("and");
        group0:CreateTarget("KillMonster", "npc=10002;count=1;");
  -- end groups
	return true;
end,

QName = function (quest , player)
	return "stage 1-2";
end,   

GiftDesc = function (quest , player)
  local exp
	local money
	local itemlist
		exp = 12			-- 经验奖励
		money = 6	  	-- 金钱奖励
	return GetQuestGiftStr(itemlist,exp,money)
end, 
  
}