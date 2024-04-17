#pragma once

#include "../item_center/shortcut_key_item.h"

namespace TOOLBOX
{
	class CloseSP : public ShortcutKeyItem
	{
	public:
		CloseSP() :CloseSP(	L"close_sp") {}
		CloseSP(const std::wstring& str) :ShortcutKeyItem(str) {};

	public:
		virtual bool call() override;
	};

} //namespace TOOLBOX