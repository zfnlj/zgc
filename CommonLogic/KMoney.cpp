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
		// 金的处理
		if (gold != 0)
		{
			std::string str = gold.toString();
			if (str.size() <= 3)	// 不需要加逗号
			{
				for (size_t i = 0; i < str.size(); ++i)
				{
					ASSERT_RETURN_VOID(os.WriteByte(str[i]));
				}
			}
			else	// 需要加逗号
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
		// 贝的处理
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
		// 金子文字 1400095
		// 贝壳文字 1400096
		_GoldShellToString(money, buffer, len, KStringManager::Instance()->GetStringByID(1400095), KStringManager::Instance()->GetStringByID(1400096), false);
	}

	void GoldShellToRichString( KInteger money, char* buffer, size_t len )
	{
		// 金子图标
		// 贝壳图标
		_GoldShellToString(money, buffer, len, KStringManager::Instance()->GetStringByID(1400097), KStringManager::Instance()->GetStringByID(1400098), true);
	}
}