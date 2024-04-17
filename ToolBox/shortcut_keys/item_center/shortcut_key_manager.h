#pragma once

#include <vector>
#include <mutex>

#include "common/public/log.h"
#include "common/public/nocopyable.h"
#include "shortcut_key_item.h"

namespace TOOLBOX
{
	using ShortcutKeyItemList = std::vector<ShortcutKeyItemPtr>;

	class ShortcutKeyManager : private NoCopyable
	{
	public:
		// 单例，唯一入口
		static ShortcutKeyManager& get()
		{
			static ShortcutKeyManager mgr;
			return mgr;
		}
	public:
		void add(ShortcutKeyItemPtr item)
		{
			std::lock_guard<std::mutex> lck(item_mutex_);
			items_.emplace_back(item);
		}

		bool call(std::wstring item_name)
		{
			bool res = false;
			for (const auto& item : items_)
			{
				if (item->getName() == item_name)
				{
					item->call();
					res = true;
				}
			}
			if (!res)
				LOG(ERROR) << "call shortcut key failed: cannot find " << item_name;
			return res;
		}

		const ShortcutKeyItemList& getItemList() { return items_; }

	protected:
		ShortcutKeyManager() = default;
		virtual ~ShortcutKeyManager() = default;

	private:
		ShortcutKeyItemList items_;
		std::mutex item_mutex_;

	};


} //namespace TOOLBOX

#define ShortcutKeyMgr TOOLBOX::ShortcutKeyManager::get()