#pragma once

#include "common/public/nocopyable.h"

#include <string>
#include <memory>

namespace TOOLBOX
{
	class ShortcutKeyItem : private NoCopyable
	{
	public:
		ShortcutKeyItem(std::wstring n) { item_name_ = std::move(n); }
		virtual ~ShortcutKeyItem() = default;
	public:
		// ��ʼ���ӿ�
		virtual bool init() { return true; }

		// ����
		virtual bool start() { return true; }

		// ֹͣ
		virtual bool stop() { return true; }

		// ����
		virtual bool call() { return true; }

	public:
		virtual std::wstring getName() { return item_name_; }

		virtual void setName(std::wstring name) { item_name_ = std::move(name); }
	protected:
		std::wstring item_name_;
	};

using	ShortcutKeyItemPtr = std::shared_ptr<ShortcutKeyItem>;
} //namespace TOOLBOX
