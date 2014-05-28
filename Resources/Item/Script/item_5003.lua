-- 
-- [doneTalk=0;cate=0;failTalk=0;note=0;viewTalk=0;hard2=0]
item_5003 = 
{
Initialize = function(KUseItem)
	return true;
end,
run = function(KUseItem,playerId )
	KUseItem:GenerateCardDeck(playerId,5003);
end,
}