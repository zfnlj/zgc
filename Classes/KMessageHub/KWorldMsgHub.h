#pragma once

class KWorldMsgHub
{
public:
	KWorldMsgHub();
	virtual ~KWorldMsgHub();

public:
	virtual void GenPlayerCard(unsigned long long Param1, unsigned long long Param2); //�ƾֿ�ʼ��Ϣ
	virtual void UseItem(unsigned long long Param1, unsigned long long Param2);
	virtual void iapCallback(unsigned long long Param1, unsigned long long Param2);
};
