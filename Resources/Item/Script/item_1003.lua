item_1003 = 
{
Initialize = function(KUseItem)
	return true;
end,
run = function(KUseItem,playerId )
		--(UINT64 playerId,int count,int rank1Rate,int rank2Rate,int rank3Rate,heroRate)
	KUseItem:GeneratNormalCard(playerId,3,30,40,20,10); 
end,
}