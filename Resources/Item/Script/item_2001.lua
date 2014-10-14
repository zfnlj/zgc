item_2001 = 
{
Initialize = function(KUseItem)
	return true;
end,
run = function(KUseItem,playerId )
	KUseItem:BuyGold(playerId,"gold_01"); 
	return true;
end,
}