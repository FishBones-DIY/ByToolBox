#include "shortcut_key_widget.h"

#include <QLabel>
#include <QVariant>
#include <QSpacerItem>
#include <QGridLayout>
#include <QPushButton>

#include "shortcut_keys/item_center/shortcut_key_manager.h"

namespace TOOLBOX
{
	ShortcutKeyWidget::ShortcutKeyWidget(QWidget* parent, const ShortcutKeyItemList& items)
		:QWidget(parent)
		, shortcut_key_item_list_(items)
	{
		setupUi();
	}

	void ShortcutKeyWidget::setupUi()
	{
		QGridLayout* grid_layout = new QGridLayout();
		grid_layout->setSpacing(24);
		grid_layout->setContentsMargins(31, 35, 31, 24);

		QVBoxLayout* v_box_layout = new QVBoxLayout();
		v_box_layout->addLayout(grid_layout);
		v_box_layout->addStretch();
		this->setLayout(v_box_layout);

		int i = 0;
		int column = 4;
		for (const auto& item : shortcut_key_item_list_)
		{
			QPushButton* iconBtn = new QPushButton();
			iconBtn->setFixedSize(162, 88);
			iconBtn->setWhatsThis("shortcut_key_button");
			QString shortcut_key_name = QString::fromStdWString(item->getName());
			iconBtn->setProperty("shortcut_key_name", shortcut_key_name);

			connect(iconBtn, &QPushButton::clicked, this, &ShortcutKeyWidget::onShortcutKeyClicked);
			QVBoxLayout* b_layout = new QVBoxLayout();
			b_layout->setContentsMargins(0, 0, 0, 0);
			b_layout->setSpacing(0);
			QLabel* shortcut_key_name_label = new QLabel();
			shortcut_key_name_label->setWhatsThis("shortcut_key_name_label");
			shortcut_key_name_label->setText(shortcut_key_name);
			shortcut_key_name_label->setWordWrap(true);                     // true：自动换行
			shortcut_key_name_label->setAlignment(Qt::AlignCenter);        // 对齐方式
			b_layout->addWidget(shortcut_key_name_label);
			iconBtn->setLayout(b_layout);



			QVBoxLayout* v_layout = new QVBoxLayout();
			v_layout->addWidget(iconBtn, 0, Qt::AlignHCenter);
			QLabel* button_lable = new QLabel();
			button_lable->setText(shortcut_key_name);
			v_layout->addWidget(button_lable, 0, Qt::AlignHCenter | Qt::AlignTop);
			QWidget* button_widget = new QWidget();
			button_widget->setLayout(v_layout);

			grid_layout->addWidget(iconBtn, i / column, i % column);
			++i;
		}

		//QWidget* button_widget = new QWidget();
		//gridLayout->addWidget(button_widget, 10,10);

	}

	void ShortcutKeyWidget::onShortcutKeyClicked()
	{
		QPushButton* iconBtn = qobject_cast<QPushButton*>(sender());
		Q_ASSERT(iconBtn);
		if (!iconBtn) return;

		std::wstring shortcut_key_name = iconBtn->property("shortcut_key_name").toString().toStdWString();
		std::thread(
			[=]() {
				ShortcutKeyMgr.call(shortcut_key_name);
			}).detach();
	}

} // namespace TOOLBOX