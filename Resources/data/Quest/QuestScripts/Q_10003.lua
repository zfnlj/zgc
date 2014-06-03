-- Script for Quest 10001 摸底考试
-- [doneTalk=0;cate=0;failTalk=0;note=0;viewTalk=0;hard2=0]
Quest_10003 = 
{
Initialize = function(quest)
	quest:SetAttrs("prevQuest=10002;abandon=0;acceptLevel=1;level=1;exp=10;money=5;");
	quest:SetAttrs("nameId=100010;talk=0;desc=100011;finishDesc=1500511;");
	quest:SetAttrs("type=1;sheetType=1;reuse=0;hardDegree=1;achieveId=0;");
	quest:SetAttrs("nextQuest=10004;achievement=0;timeperiod=0;");
	quest:SetAttrs("battleField=3;");
	
	-- Group 0
	local group0 = quest:CreateTargetGroup("and");
        group0:CreateTarget("KillMonster", "npc=10003;count=1;");
  -- end groups
	return true;
end,

QName = function (quest , player)
	return "stage 1-3";
end,   


UseGift = function(quest,player)
	local item
	if player:GetAttrValue(88)==0 then
		item = "5001" -- 任务奖励物品
	elseif player:GetAttrValue(88)==1 then
	  item = "5002" -- 任务奖励物品
	else -- 默认奖励信息为0，一般不要改
	  item = "5003" -- 任务奖励物品
	end 
	return item; 
end,  

SelectGiftList = function(quest)
	return "5001;5002;5003";
end,  


GiftDesc = function (quest , player)
  local exp
	local money
	local itemlist
		exp = 10			-- 经验奖励
		money = 5	  	-- 金钱奖励
	return GetQuestGiftStr(itemlist,exp,money)
end, 
  
}