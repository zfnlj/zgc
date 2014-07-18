
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
			
			UseSkill_32011 = function(self,ctrl,card)
					local str = 0 .. '*' .. CPP.AIAssist:CalcFighterNum(ctrl,0)
					return str
			end,
			
			EnterFight_20053 = function(self,ctrl,card)
					local val1 = CPP.AIAssist:CalcFighterSetVal(ctrl,0)-CPP.AIAssist:CalcFighterSetVal(ctrl,1)
					local val2 = CPP.AIAssist:CalcHandSetVal(ctrl,1) - 10;
					return (val1 - val2*0.8)> 0;
			end,
			
			EnterFight_20059 = function(self,ctrl,card)
					local val1 = CPP.AIAssist:CalcFighterNum(ctrl,0)-CPP.AIAssist:CalcFighterNum(ctrl,1)
					return val1 >= 0
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
