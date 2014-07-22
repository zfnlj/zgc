
Quest_133 = 
{
Initialize = function(quest)
	quest:SetAttrs("prevQuest=0;abandon=0;acceptLevel=1;level=1;exp=200;money=100;");
	quest:SetAttrs("nameId=133;talk=0;desc=133;finishDesc=1500511;");
	quest:SetAttrs("type=3;reuse=0;hardDegree=1;achieveId=0;");
	quest:SetAttrs("nextQuest=0;achievement=0;timeperiod=0;");
	quest:SetAttrs("battleField=133;");
	
	-- Group 0
	local group0 = quest:CreateTargetGroup("and");
        group0:CreateTarget("KillMonster", "npc=133;count=1;");
  -- end groups
	return true;
end,

QName = function (quest , player)
	return "daily mission4_3";
end,  


GiftDesc = function (quest , player)
  local exp
	local money
	local itemlist
		exp = 200			-- 经验奖励
		money = 100	  	-- 金钱奖励
	return GetQuestGiftStr(itemlist,exp,money)
end, 
  
}