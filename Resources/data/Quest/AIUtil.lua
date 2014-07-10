
AIUtil ={
			UseSkill_33009 = function(self,ctrl,card)
					local des = CPP.AIAssist:AbilityMostValuableTarget(ctrl,card,330090,99,2)
					if (des==nil) then
						return ""
					else
						local val = 1.5 - CPP.AIAssist:CalcAbilityDoVal(ctrl,330090,card,des)
						local str = des:GetRealId() .. '*' .. val
						return str
					end
			end,
			
			SoldierAtk_20007 = function(self,ctrl,card,lst)
					local des
					if ( card:GetAtk() < 5) then
					  des = CPP.AIAssist:BestAttackTarget(ctrl,card,lst,2,1)
					  if (des==nil) then
							des = CPP.AIAssist:BestAttackTarget(ctrl,card,lst,card:GetHp()-1,0)
						end
					else
						des = CPP.AIAssist:BestAttackTarget(ctrl,card,lst,99,0)
					end
					
					if (des==nil) then
						return 0
					else
						return des:GetRealId()
					end
			end,
}
