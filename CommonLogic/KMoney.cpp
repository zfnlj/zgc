#include "KMoney.h"
#include "System/Misc/KStream.h"
#include "StringManager/KStringManager.h"

namespace KMoneyAbout
{
	static void _GoldShellToString(KInteger money, char* buffer, size_t len, const char* gold_icon, const char* shell_icon, bool showZeroShell)
	{
		ASSERT_RETURN_VOID(len > 0);

		KDataOutputStream os(buffer, len);
		if (money < 0)
		{
			os.WriteByte('-');
			money = -money;
		}
		KInteger shell;
		KInteger gold;
		money.Divmod(1000, gold, shell);
		// ��Ĵ���
		if (gold != 0)
		{
			std::string str = gold.toString();
			if (str.size() <= 3)	// ����Ҫ�Ӷ���
			{
				for (size_t i = 0; i < str.size(); ++i)
				{
					ASSERT_RETURN_VOID(os.WriteByte(str[i]));
				}
			}
			else	// ��Ҫ�Ӷ���
			{
				size_t firstWriteCount = str.size() % 3;
				size_t loopCount = str.size() / 3;
				if (firstWriteCount == 0)
				{
					firstWriteCount = 3;
					--loopCount;
				}
				for (size_t i = 0; i < firstWriteCount; ++i)
				{
					ASSERT_RETURN_VOID(os.WriteByte(str[i]));
				}
				size_t readPos = firstWriteCount;
				for (size_t i = 0; i < loopCount; ++i)
				{
					ASSERT_RETURN_VOID(os.WriteByte(','));
					for (size_t j = 0; j < 3; ++j)
					{
						ASSERT_RETURN_VOID(os.WriteByte(str[readPos++]));
					}
				}
			}
			ASSERT_RETURN_VOID(os.WriteByteArray(gold_icon, strlen(gold_icon)));
		}
		// ���Ĵ���
		if (shell != 0 || showZeroShell || os.m_pos == 0)
		{
			std::string str = shell.toString();
			ASSERT_RETURN_VOID(os.WriteByteArray(str.c_str(), str.size()));
			ASSERT_RETURN_VOID(os.WriteByteArray(shell_icon, strlen(shell_icon)));
		}
		ASSERT_RETURN_VOID(os.WriteByte(0));
	}

	void GoldShellToString( KInteger money, char* buffer, size_t len )
	{
		// �������� 1400095
		// �������� 1400096
		_GoldShellToString(money, buffer, len, KStringManager::Instance()->GetStringByID(1400095), KStringManager::Instance()->GetStringByID(1400096), false);
	}

	void GoldShellToRichString( KInteger money, char* buffer, size_t len )
	{
		// ����ͼ��
		// ����ͼ��
		_GoldShellToString(money, buffer, len, KStringManager::Instance()->GetStringByID(1400097), KStringManager::Instance()->GetStringByID(1400098), true);
	}
}