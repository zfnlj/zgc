
Quest_123 = 
{
Initialize = function(quest)
	quest:SetAttrs("prevQuest=0;abandon=0;acceptLevel=1;level=1;exp=150;money=70;");
	quest:SetAttrs("nameId=123;talk=0;desc=123;finishDesc=1500511;");
	quest:SetAttrs("type=3;reuse=0;hardDegree=3;achieveId=0;");
	quest:SetAttrs("nextQuest=0;achievement=0;timeperiod=0;");
	quest:SetAttrs("battleField=123;");
	
	-- Group 0
	local group0 = quest:CreateTargetGroup("and");
        group0:CreateTarget("KillMonster", "npc=123;count=1;");
  -- end groups
	return true;
end,

QName = function (quest , player)
	return "daily mission3_3";
end,  


GiftDesc = function (quest , player)
  local exp
	local money
	local itemlist
		exp = 150			-- 经验奖励
		money = 70	  	-- 金钱奖励
	return GetQuestGiftStr(itemlist,exp,money)
end, 
  
}