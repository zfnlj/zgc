item_2002 = 
{
Initialize = function(KUseItem)
	return true;
end,
run = function(KUseItem,playerId )
	KUseItem:BuyGold(playerId,"gold_02"); 
	return true;
end,
}