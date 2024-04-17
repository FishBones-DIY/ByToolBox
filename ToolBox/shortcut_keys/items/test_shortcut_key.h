#pragma once

#include "../item_center/shortcut_key_item.h"

namespace TOOLBOX
{
	class ShortcutKeyTest : public ShortcutKeyItem
	{
	public:
		ShortcutKeyTest() :ShortcutKeyItem(	L"test") {}
		ShortcutKeyTest(const std::wstring& str) :ShortcutKeyItem(str) {};

	public:
		virtual bool call() override;
	};

} //namespace TOOLBOX