
AIUtil ={
			ai_33009 = function(self,ctrl,card)
					local des = CPP.AIAssist:AbilityMostValuableTarget(ctrl,card,330090,99,2)
					if (des==nil) then
						return ""
					else
						local val = CPP.AIAssist:CalcAbilityDoVal(ctrl,330090,card,des) + 1.5
						local str = des:GetRealId() .. '*' .. val
						return str
					end
			end,
}
