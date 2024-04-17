#pragma once
#include "common/public/log.h"
#include "common/uitils/file_utils.h"
#include "common/uitils/path_utils.h"

#include "../funcs/tq_functions.h"
#include "../item_center/shortcut_key_item.h"

namespace TOOLBOX
{
	class RunTQ : public ShortcutKeyItem
	{
	public:
		RunTQ() :RunTQ(	L"run_tq") {}
		RunTQ(const std::wstring& str) :ShortcutKeyItem(str) {};

	public:
		virtual bool call() override 
		{
			LOG(INFO) << "Start run: " << item_name_;
			TQFunctions::runTQ();
			return true;
		}
	};

	class CloseTQ : public ShortcutKeyItem
	{
	public:
		CloseTQ() :CloseTQ(L"close_tq") {}
		CloseTQ(const std::wstring& str) :ShortcutKeyItem(str) {};

	public:
		virtual bool call() override
		{
			LOG(INFO) << "Start run: " << item_name_;
			TQFunctions::closeTQ();
			return true;
		}
	};

	class CloseSP : public ShortcutKeyItem
	{
	public:
		CloseSP() :CloseSP(L"close_sp") {}
		CloseSP(const std::wstring& str) :ShortcutKeyItem(str) {};

	public:
		virtual bool call() override
		{
			LOG(INFO) << "Start run: " << item_name_;
			TQFunctions::closeSPIfOpend();
			return true;
		}
	};

	class CloseSPAndReplaceSafeBase : public ShortcutKeyItem
	{
	public:
		CloseSPAndReplaceSafeBase() :CloseSPAndReplaceSafeBase(L"close_sp_and_replace_safebase") {}
		CloseSPAndReplaceSafeBase(const std::wstring& str) :ShortcutKeyItem(str) {};

	public:
		virtual bool call() override
		{
			LOG(INFO) << "Start run: " << item_name_;
			TQFunctions::replaceSafeBase();
			return true;
		}
	};

} //namespace TOOLBOX