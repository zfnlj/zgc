

Quest_110 = 
{
Initialize = function(quest)
	quest:SetAttrs("prevQuest=0;abandon=0;acceptLevel=1;level=1;exp=180;money=20;");
	quest:SetAttrs("nameId=110;talk=0;desc=110;");
	quest:SetAttrs("type=3;reuse=0;hardDegree=10;achieveId=0;");
	quest:SetAttrs("nextQuest=0;achievement=0;timeperiod=0;");
	quest:SetAttrs("battleField=131;");
	
	-- Group 0
	local group0 = quest:CreateTargetGroup("and");
        group0:CreateTarget("KillMonster", "npc=110;count=1;");
  -- end groups
	return true;
end,

QName = function (quest , player)
	return "daily mission10";
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