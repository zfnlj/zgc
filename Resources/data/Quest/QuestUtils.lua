
-- �ж�һ�������ǲ�����valֵ
function find(t, val)
	local k, v
	for k, v in pairs(t) do
		if v == val then
			return true
		end
	end

	return false
end

--��������id�õ���������,��������string
function GetQuestName(qid)
	local quest = CPP.questMgr:GetQuest(qid)
	if not quest then
		return ""
	end
	local qnameid = quest:GetIntAttr("nameId", 0);
	return STRING(qnameid)
end

--�����������͵õ�������
function GetQuestTypeName(qtype)
	if qtype == 1 then
		return "����"
	elseif qtype == 2 then
		return "֧��"
	end
	return "δ֪����"
end

function table2str(t)
	local str = "{";
	for k,v in pairs(t) do
		if string.len(str) > 1 then
			str = str .. ";";
		end;

		if type(v) == "table" then
			str = str .. tostring(k) .. "=" .. table2str(v)
		else
			str = str .. tostring(k) .. "=" .. tostring(v)
		end;
	end;
	return str .. "}";
end;

function parseParamValString(s)
	local t = {};
	for k,v in string.gmatch(s, "(%d+):(%d+)") do
		t[tonumber(k)] = tonumber(v);
	end;
	return t;
end;

function parseParamString(s)
	local t = {};

	for k,v in string.gmatch(s,"(%a+)%=([a-zA-Z0-9_%:%,]*)") do
		t[k] = v;
	end;

	for k,v in pairs(t) do
		if string.find(tostring(v),",") ~= nil then
			t[k] = parseParamValString(tostring(v));
		else
			t[k] = tonumber(v);
		end;
	end;
	return t;
end;

-- set:new.imageset image:Tanweidengji
-- ��������ɫ״̬
-- set:new.imageset image:Tanweidengji_kong
-- ���ǿ���ɫ״̬
-- set:new.imageset image:Tanweidengji_ban
-- �������״̬
function GetAchieveDesc(count, total)
	local achieveDesc = GetStringByID(101);
	local finishFlag = "#[o*]";
	local noFinishFlag = "#[s*]";
	local noFinshCount = total - count;

	local i;
	for i=0, count-1 do
		achieveDesc = achieveDesc .. finishFlag;
	end;

	local i;
	for i=0, noFinshCount-1 do
		achieveDesc = achieveDesc .. noFinishFlag;
	end;

	return achieveDesc;
end;

function GetHeadLine(achieveId, num)
	local str = GetStringByID(102);
	if (num == nil) or (num == 0) then
		return string.format(str, "", GetStringByID(114));
	end;

	local flag = "<IMG=set:Addpic image:yellowstar1,16,16>";
	str = string.format(str, StringFromID(achieveId), flag)

	local i;
	for i=2, num do
		str = str .. flag;
	end;

	return str;
end;

function GetGiftTitle()
	local buf = '\r\n' .. CPP.stringMgr:GetStringByID(117) .. '\r\n'
	return buf
end

function GetBPAddTitle()
	local buf = '\r\n' .. CPP.stringMgr:GetStringByID(118) .. '\r\n'
	return buf
end

function AddMoney(n)
	local b, s, g = math.modf(n / 10000) , math.modf(((n % 10000) / 100 )), math.modf(n % 100)
	local str = CPP.stringMgr:GetStringByID(100004336)
	if b > 0 then str = str .. b .. CPP.stringMgr:GetStringByID(119) end
	if s > 0 then str = str.. s .. CPP.stringMgr:GetStringByID(120) end
	if g > 0 then str = str.. g .. CPP.stringMgr:GetStringByID(121) end
	return str
end

function AddMoneyByLevel(level, money, para)
	local _money = money + para * level
	local b, s, g = math.modf(_money / 10000) , math.modf(((_money % 10000) / 100 )), math.modf(_money % 100)
	local str = CPP.stringMgr:GetStringByID(100004336)
	if b > 0 then str = str .. b .. CPP.stringMgr:GetStringByID(119) end
	if s > 0 then str = str.. s .. CPP.stringMgr:GetStringByID(120) end
	if g > 0 then str = str.. g .. CPP.stringMgr:GetStringByID(121) end
	return str
end

function AddBPMoney(n)
	return string.format(GetStringByID(108), n)
end

function AddBPPoints(nPoints)
	return string.format(GetStringByID(105), nPoints)
end

function AddBPContribution(nContribution)
	return string.format(GetStringByID(106), nContribution)
end

function AddBPRealContribution(nContribution)
	return string.format(GetStringByID(107), nContribution)
end

function ByLevel(level, money, para)
	local n = money + para * level
	local b, s, g = math.modf(n / 10000) , math.modf(((n % 10000) / 100 )), math.modf(n % 100)
	local str = GetStringByID(100004341)
	if b > 0 then str = str .. b .. GetStringByID(119) end
	if s > 0 then str = str.. s .. GetStringByID(120) end
	if g > 0 then str = str.. g .. GetStringByID(121) end
	return str
end

function AddExp(n)
	return string.format(CPP.stringMgr:GetStringByID(109), n)
end

function AddExpByLevel(level, exp, para)
	return string.format(CPP.stringMgr:GetStringByID(109), exp + para * level)
end

--------------------------------------
-- notes:
--     m and g maybe nil
-- n : item type ID
-- m : item count
-- g : item group
--------------------------------------
function AddItem(n, m, g)
	local s;
	if (g ~= nil) then
		s = string.format("<IT=1,%d,1,%d>", n, g)
	else
		s = string.format("<IT=1,%d,1>", n)
	end

	if m ~= nil and m > 1 then
		s = s .. ' X ' .. m;
	end;

	return s;
end

function GiveItemBySex(nSex, item1, item2, count)
	if (nSex == 0) then
	 	return string.format("<IT=1,%d,1>", item1)
	elseif (nSex == 1) then
		return string.format("<IT=1,%d,1>", item2)
	end

	return ""
end

function GiveItemByClass(nClass, item1, item2, item3, item4, count)
	if nClass == 0 then
		return ""
	end

	local ItemID = 0
	if (nClass == 1) then
	 	ItemID = item1
	elseif (nClass == 2) then
		ItemID = item2
	elseif (nClass == 3) then
		ItemID = item3
	elseif (nClass == 4) then
		ItemID = item4
	else
		return ""
	end

	return string.format("<IT=1,%d,1>", ItemID)
end

function AddTitle(abyClass, abyTitleID, abyLevel)
	return string.format(GetStringByID(110), GetTitleName(abyClass, abyTitleID, abyLevel), abyLevel)
end

function AddPlayerSkill(skillID)
	return string.format(GetStringByID(111), GetSkillName(skillID))
end

function AddMBExp(n)
	return string.format(GetStringByID(112), n)
end

function AddBPFame(n)
	return string.format(GetStringByID(113), n)
end

function TimerTraceText(ms)
	t = math.floor(ms/1000);
	min = t / 60;
	s = t % 60;
	fmt = StringFromID(123);
	return string.format(fmt,min,s);
end


-- �õ�������Ϣ
-- ͼƬ�����ֱ������tooltip��������
-- ��Ʒtooltip��ʽ:<a href='event:5,Item_ID'>��ʾ����</a>
-- icon��ʽ:<img src='subimage://set:xxx image:xxx' width=xxx height=xxx vspace=xxx hspace=xxx/>
-- ����ͽ�Ǯ��tooltip��ʽ:<a href='event:7,1450301'><img xxxxx/></a>
-- 1450301	����ֵ 1450302	��Ǯ
function GetQuestGiftStr(itemList,qExp,qMoney,qSupport)
	local giftInfo = ""
	local jyIcon = "set:Renwu1 image:JingYan"
	local jqIcon = "set:Renwu1 image:JinQian"
	local defIcon = ""
	local width = 27
	local height = 27
	local vspace = 2
	local hspace = 2

	--[[ ȡ����ʾ"�㽫�õ���"
	if qExp > 0 or qMoney > 0 or (itemList ~= nil and itemList ~= "") then
		giftInfo = giftInfo .. CPP.stringMgr:GetStringByID(50003) .. "<br>"
	else
		return ""
	end
	]]
	qExp = qExp or 0
	qMoney = qMoney or 0
	qSupport = qSupport or 0
    --[[ ��ֵ����������,�˴�ֻ����Ʒ����
    if qExp > 0 then
		giftInfo = giftInfo .. string.format("<a href='event:7,1450301'><img src='subimage://%s' width='%d' height='%d'/>%d</a>			", jyIcon, width, height, qExp)
	end

	if qMoney > 0 then
		giftInfo = giftInfo .. string.format("<a href='event:7,1450302'><img src='subimage://%s' width='%d' height='%d'/>%d</a>	<br>", jqIcon, width, height, qMoney)
	elseif qExp > 0 then
		giftInfo = giftInfo .. "	<br>"
	end

	if qSupport > 0 then
		giftInfo = giftInfo .. string.format("<a href='event:7,1403274'><img src='subimage://%s' width='%d' height='%d'/>%d</a>	<br>", jqIcon, width, height, qSupport)
	end
	]]

	if itemList ~= nil and itemList ~= "" then
		local cnt = 0

		--[[ ȡ����ʾ"�㻹���õ���"
		if qExp > 0 or qMoney > 0 then
			giftInfo = giftInfo .. CPP.stringMgr:GetStringByID(50004) .. "<br>"
		end
		]]

		for u,v in string.gmatch(itemList, "(%d+):(%d+)") do

			icon = GetItemIcon(tonumber(u))
			--name = GetItemName(tonumber(u))
			if icon ~= "" then
				giftInfo = giftInfo .. string.format("<a href='event:5,%d'><img src='subimage://%s' width='%d' height='%d'/> X %d</a>", tonumber(u), icon, width, height, tonumber(v))
			else
				giftInfo = giftInfo .. string.format("<img src='subimage://%s' width='%d' height='%d'/> X %d", defIcon, width, height, tonumber(v))
			end

			cnt = cnt + 1				-- һ��������Ʒ
			if (cnt%2) == 0 then
				giftInfo = giftInfo .. "<br>"
			else
				giftInfo = giftInfo .. "			"
			end

		end
	end

	return giftInfo
end


-- �õ�������������Ϣ
-- ͼƬ�����ֱ������tooltip��������
-- ��Ʒtooltip��ʽ:<a href='event:5,Item_ID'>��ʾ����</a>
-- icon��ʽ:<img src='subimage://set:xxx image:xxx' width=xxx height=xxx vspace=xxx hspace=xxx/>
-- ����ͽ�Ǯ��tooltip��ʽ:<a href='event:7,1450301'><img xxxxx/></a>
-- 1450301	����ֵ 1450302	��Ǯ
function GetProfQuestGiftStr(itemList,qMoney,qBlue,isbuff,buffstr)
	local giftInfo = ""
	local jyIcon = "set:Renwu1 image:JingYan"
	local jqIcon = "set:Renwu1 image:JinQian"
	local defIcon = ""
	local width = 27
	local height = 27
	local vspace = 2
	local hspace = 2


	if qBlue > 0 or qMoney > 0 or (itemList ~= nil and itemList ~= "") then
		if isbuff > 1 then
			giftInfo = giftInfo .. buffstr .. "<br>"
		end
	else
		return ""
	end


  if qBlue > 0 then
		giftInfo = giftInfo .. string.format("<a href='event:7,1405139'><img src='subimage://%s' width='%d' height='%d'/>%d</a>			", jyIcon, width, height, qBlue)
	end

	if qMoney > 0 then
		giftInfo = giftInfo .. string.format("<a href='event:7,1450302'><img src='subimage://%s' width='%d' height='%d'/>%d</a>	<br>", jqIcon, width, height, qMoney)
	elseif qBlue > 0 then
		giftInfo = giftInfo .. "	<br>"
	end

	if itemList ~= nil and itemList ~= "" then
		local cnt = 0

		--[[ ȡ����ʾ"�㻹���õ���"
		if qBlue > 0 or qMoney > 0 then
			giftInfo = giftInfo .. CPP.stringMgr:GetStringByID(50004) .. "<br>"
		end
		]]

		for u,v in string.gmatch(itemList, "(%d+):(%d+)") do

			icon = GetItemIcon(tonumber(u))
			--name = GetItemName(tonumber(u))
			if icon ~= "" then
				giftInfo = giftInfo .. string.format("<a href='event:5,%d'><img src='subimage://%s' width='%d' height='%d'/> X %d</a>", tonumber(u), icon, width, height, tonumber(v))
			else
				giftInfo = giftInfo .. string.format("<img src='subimage://%s' width='%d' height='%d'/> X %d", defIcon, width, height, tonumber(v))
			end

			cnt = cnt + 1				-- һ��������Ʒ
			if (cnt%2) == 0 then
				giftInfo = giftInfo .. "<br>"
			else
				giftInfo = giftInfo .. "			"
			end

		end
	end

	return giftInfo
end


--Ӫ��������������Ϣ�ӿ�
function GetCampQuestGiftDes(quest, player)
	--[[
    local cnt = CPP.mainPlayer:GetPlayerPeriodAttrData(1, 0)        -- ����Ӫ��������ɴ���, �ͻ�����
    local clr = quest:GetIntAttr("grade", 0)    -- �������ɫ�ȼ���Ĭ��Ϊ0
    local p = 1
    if clr == 1 then
        if cnt <= 5 then
            p = 1
        else
            p = 0.7
        end
    elseif clr == 2 then
        p = 1.33
    elseif clr == 3 then
        p = 2
    elseif clr == 4 then
        p = 3
    else
        p = 0
    end
    --]]
    --local money = 100*p     -- ��Ǯ����*p
    local clrparam = {2,3,5,8,10,
    	[0] = 2,
    }
    local exp = player:GetAttrValue(41) * clrparam[quest:GetIntAttr("grade", 0)] * 4 * quest:GetIntAttr("estimateTime", 1)       -- ����
    local support = quest:GetIntAttr("campSupport", 0)
    local width = 27
    local height = 27
    local jyIcon = "set:Renwu1 image:JingYan"
    local jqIcon = "set:Renwu1 image:JinQian"
    local zcIcon = "set:Maptext image:zhichidu"	-- ��������ͼ�꣬���ý�Ǯ��
    local giftinfo = string.format("<a href='event:7,1450301'><img src='subimage://%s' width='%d' height='%d'/>%d</a>           ", jyIcon, width, height, exp)
    --giftinfo = giftinfo .. string.format("<a href='event:7,1450302'><img src='subimage://%s' width='%d' height='%d'/>%d</a> <br>", jqIcon, width, height, money)
    giftinfo = giftinfo .. string.format("<a href='event:7,1403274'><img src='subimage://%s' width='%d' height='%d'/>%d</a> <br>", zcIcon, width, height, support)
    return giftinfo
end

--������ֵ������Ϣ�ӿ�, ������ֵ��Ϣ����:��Ǯ, ����, ֧�ֶȵ�
--��ʽ9:�ַ���X��ֵ,��������,1450302Ϊ�ַ���id
--<a href='event:9,1450302,%d'><img src='subimage://%s' width='%d' height='%d'/></a>
function GetQuestFigureAwardStr(quest)
	local giftInfo = ""
	local jyIcon = "set:Renwu1 image:JingYan"
	local jqIcon = "set:Renwu1 image:JinQian"
	local sptIcon = "set:Renwu1 image:JinQian"	-- ֧�ֶ�icon��ʱ�ý�Ǯ��
	local bpIcon = "set:Renwu1 image:JinQian"	-- ����֮��icon��ʱ�ý�Ǯ��
	local defIcon = ""
	local width = 27
	local height = 27
	local vspace = 2
	local hspace = 2
	
	local qMoney = quest:GetIntAttr("money", 0)
	local qBlue = quest:GetIntAttr("bluePower", 0)
	local qExp = quest:GetIntAttr("exp", 0)
	local qSupport = quest:GetIntAttr("campSupport", 0)
	if qMoney > 0 then
		giftInfo = giftInfo .. string.format("<a href='event:9,1450302,%d'><img src='subimage://%s' width='%d' height='%d'/></a>			", qMoney, jqIcon, width, height)
	end
	if qBlue > 0 then
		giftInfo = giftInfo .. string.format("<a href='event:9,1405139,%d'><img src='subimage://%s' width='%d' height='%d'/></a>			", qBlue, bpIcon, width, height)
	end
	if qExp > 0 then
		giftInfo = giftInfo .. string.format("<a href='event:9,1450301,%d'><img src='subimage://%s' width='%d' height='%d'/></a>			", qExp, jyIcon, width, height)
	end
	if qSupport > 0 then
		giftInfo = giftInfo .. string.format("<a href='event:9,1403274,%d'><img src='subimage://%s' width='%d' height='%d'/></a>			", qSupport, sptIcon, width, height)
	end
	return giftInfo
end

--�õ��������������, ����ʱʲô��
--<a href='event:8,%s'><img src='subimage://%s' width='%d' height='%d'/></a>����event8Ϊֱ����ʾ�ַ���
GetQuestLimitCondition = function(quest, player)
	local str = ""
	local tooltipPattern = "<a href='event:8,%s'><img src='subimage://%s' width='%d' height='%d'/></a>			"
	local timeicon = "set:Renwu1 image:JingYan"	-- ��ʱ��icon
	local width = 27
	local height = 27
	
	if quest and player then
		local qtimer = quest:GetCurTimer()
		if qtimer then
			local totalTime = qtimer:GetIntAttr("total", 0)
			if totalTime > 0 then
				local minute = math.floor(totalTime/60)
				local second = math.floor(totalTime%60)
				local pattern = "����ʱ�䣺 %02d:%02d"
				local tooltipStr = string.format(pattern, minute, second)
				str = str .. string.format(tooltipPattern, tooltipStr, timeicon, width, height)
			end
		end
	end
	return str
end
