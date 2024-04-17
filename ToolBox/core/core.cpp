#include "common/public/load_lib.h"
#include "shortcut_keys/items/tq_funs.h"
#include "shortcut_keys/item_center/shortcut_key_manager.h"

#include "core.h"

namespace TOOLBOX
{
	bool Core::init()
	{
		loadShortcutKeyItems();

		return true;
	}

	void Core::loadShortcutKeyItems()
	{
		ShortcutKeyMgr.add(std::make_shared<RunTQ>(L"运行天擎"));
		ShortcutKeyMgr.add(std::make_shared<CloseSP>(L"关闭自保"));
		ShortcutKeyMgr.add(std::make_shared<CloseTQ>(L"关闭天擎"));
		ShortcutKeyMgr.add(std::make_shared<CloseSPAndReplaceSafeBase>(L"替换SafeBase.dll"));
	}

} //namespace TOOLBOX