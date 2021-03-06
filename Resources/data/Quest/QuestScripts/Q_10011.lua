﻿-- Script for Quest 10001 摸底考试
-- [doneTalk=0;cate=0;failTalk=0;note=0;viewTalk=0;hard2=0]
Quest_10011 = 
{
Initialize = function(quest)
	quest:SetAttrs("prevQuest=10010;abandon=0;acceptLevel=1;level=1;exp=60;money=30;");
	quest:SetAttrs("nameId=100011;talk=0;desc=100011;finishDesc=1500511;");
	quest:SetAttrs("type=1;reuse=0;hardDegree=1;achieveId=0;share=0;");
	quest:SetAttrs("nextQuest=0;achievement=0;timeperiod=0;");
	quest:SetAttrs("battleField=11;");
	
	-- Group 0
	local group0 = quest:CreateTargetGroup("and");
        group0:CreateTarget("KillMonster", "npc=10012;count=1;");
  -- end groups
	return true;
end,


UseGift = function(quest,player)
	return 2003;  --英雄碎片
end,


QName = function (quest , player)
	return "stage 1-11";
end,  


GiftDesc = function (quest , player)
  local exp
	local money
	local itemlist
		exp = 40			-- 经验奖励
		money = 20	  	-- 金钱奖励
	if player:GetAttrValue(88)==0 then
		itemlist = "5001:1" -- 任务奖励物品
	elseif player:GetAttrValue(88)==1 then
	  itemlist = "5002:1" -- 任务奖励物品
	else -- 默认奖励信息为0，一般不要改
	  itemlist = "5003:1" -- 任务奖励物品
	end 
		
	return GetQuestGiftStr(itemlist,exp,money)
end, 
  
}