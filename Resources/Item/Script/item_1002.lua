item_1002 = 
{
Initialize = function(KUseItem)
	return true;
end,
run = function(KUseItem,playerId )
	KUseItem:GeneratNormalCard(playerId,3,2);
end,
}