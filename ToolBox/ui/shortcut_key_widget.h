#pragma once
#include <QWidget>
#include "shortcut_keys/item_center/shortcut_key_manager.h"
namespace TOOLBOX
{
	class ShortcutKeyWidget :public QWidget
	{
		Q_OBJECT
	public:
		ShortcutKeyWidget(QWidget* parent, const ShortcutKeyItemList& items);
	public slots:
		void onShortcutKeyClicked();
	private:
		void setupUi();
	private:
		ShortcutKeyItemList shortcut_key_item_list_;
	};
} // namespace TOOLBOX


