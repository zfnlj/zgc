-- Script for Quest 10001 摸底考试
-- [doneTalk=0;cate=0;failTalk=0;note=0;viewTalk=0;hard2=0]
Quest_10010 = 
{
Initialize = function(quest)
	quest:SetAttrs("prevQuest=10009;abandon=0;acceptLevel=1;level=1;exp=40;money=20;");
	quest:SetAttrs("nameId=100010;talk=0;desc=100011;finishDesc=1500511;noFinishDesc=1500512;");
	quest:SetAttrs("type=1;sheetType=1;reuse=0;hardDegree=1;achieveId=0;share=0;");
	quest:SetAttrs("nextQuest=10011;achievement=0;timeperiod=0;");
	quest:SetAttrs("battleField=10;");
	
	-- Group 0
	local group0 = quest:CreateTargetGroup("and");
        group0:CreateTarget("KillMonster", "npc=10011;count=1;");
  -- end groups
	return true;
end,

QName = function (quest , player)
	return "stage 1-10";
end,   

GiftDesc = function (quest , player)
  local exp
	local money
	local itemlist
		exp = 40			-- 经验奖励
		money = 20	  	-- 金钱奖励
	return GetQuestGiftStr(itemlist,exp,money)
end, 
  
}