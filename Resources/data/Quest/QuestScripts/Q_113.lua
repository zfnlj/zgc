-- Script for Quest 113 每日任务
-- [doneTalk=0;cate=0;failTalk=0;note=0;viewTalk=0;hard2=0]
Quest_113 = 
{
Initialize = function(quest)
	quest:SetAttrs("prevQuest=0;abandon=0;acceptLevel=1;level=1;exp=120;money=60;");
	quest:SetAttrs("nameId=113;talk=0;desc=113;finishDesc=1500511;");
	quest:SetAttrs("type=3;reuse=0;hardDegree=2;achieveId=0;");
	quest:SetAttrs("nextQuest=0;achievement=0;timeperiod=0;");
	quest:SetAttrs("battleField=113;");
	
	-- Group 0
	local group0 = quest:CreateTargetGroup("and");
        group0:CreateTarget("KillMonster", "npc=113;count=1;");
  -- end groups
	return true;
end,

QName = function (quest , player)
	return "daily mission2_3";
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