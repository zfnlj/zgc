-- Script for Quest 501 每日任务
Quest_501 = 
{
Initialize = function(quest)
	quest:SetAttrs("prevQuest=0;abandon=0;acceptLevel=1;level=1;exp=20;money=10;");
	quest:SetAttrs("nameId=501;talk=0;desc=501;finishDesc=1500511;");
	quest:SetAttrs("type=2;sheetType=1;reuse=0;hardDegree=1;achieveId=0;");
	quest:SetAttrs("nextQuest=0;achievement=0;timeperiod=0;");
	quest:SetAttrs("battleField=501;");
	
	-- Group 0
	local group0 = quest:CreateTargetGroup("and");
        group0:CreateTarget("KillMonster", "npc=501;count=1;");
  -- end groups
	return true;
end,

QName = function (quest , player)
	return "adventure mission";
end,  


GiftDesc = function (quest , player)
  local exp
	local money
	local itemlist
		exp = 20			-- 经验奖励
		money = 10	  	-- 金钱奖励
	return GetQuestGiftStr(itemlist,exp,money)
end, 
  
}