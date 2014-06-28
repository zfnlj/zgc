
PlayCardUtil ={
			c_330140 = function(self, str,id)
					local hero = CPP.battleCtrl:GetMyHero(id)
					local hp = hero:GetHp()
					if hp > 12 then
						return 4
					else
						return 6
					end
			end,
}
