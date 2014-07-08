
AIUtil ={
			ai_33009 = function(self,ctrl,card)
					local des = CPP.AIAssist:AbilityMostValuableTarget(ctrl,card,330090,99,2)
					if (des==nil) then
						return ""
					else
						local val = 1.5 - CPP.AIAssist:CalcAbilityDoVal(ctrl,330090,card,des)
						local str = des:GetRealId() .. '*' .. val
						return str
					end
			end,
}
