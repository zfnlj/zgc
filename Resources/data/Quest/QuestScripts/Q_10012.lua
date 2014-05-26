-- Script for Quest 10001 摸底考试
-- [doneTalk=0;cate=0;failTalk=0;note=0;viewTalk=0;hard2=0]
Quest_10012 = 
{
Initialize = function(quest)
	quest:SetAttrs("prevQuest=10011;abandon=0;acceptLevel=1;level=1;exp=10;money=5;");
	quest:SetAttrs("nameId=100010;talk=0;desc=100011;finishDesc=1500511;noFinishDesc=1500512;");
	quest:SetAttrs("type=1;sheetType=1;reuse=0;hardDegree=1;achieveId=0;share=0;");
	quest:SetAttrs("nextQuest=0;achievement=0;timeperiod=0;");
	quest:SetAttrs("battleField=12;");
	
	-- Group 0
	local group0 = quest:CreateTargetGroup("and");
        group0:CreateTarget("KillMonster", "npc=10012;count=1;");
  -- end groups
	return true;
end,

QName = function (quest , player)
	return "stage 1-12";
end,  

SelectGift = function(quest)
	return "5001;5002;5003";
end, 

GiftDesc = function (quest , player)
  local exp
	local money
	local itemlist
		exp = 10			-- 经验奖励
		money = 5	  	-- 金钱奖励
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