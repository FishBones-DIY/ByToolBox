#pragma once

#include "../item_center/shortcut_key_item.h"

namespace TOOLBOX
{
	class CloseTQ : public ShortcutKeyItem
	{
	public:
		CloseTQ() :CloseTQ(	L"close_tq") {}
		CloseTQ(const std::wstring& str) :ShortcutKeyItem(str) {};

	public:
		virtual bool call() override;
	};

} //namespace TOOLBOX