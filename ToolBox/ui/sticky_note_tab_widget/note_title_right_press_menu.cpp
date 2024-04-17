#include "note_title_right_press_menu.h"

#include <QVBoxLayout>
#include <QHBoxLayout>

#include "base_window/include/base_window.h"

NoteTitleRightPressMenu::NoteTitleRightPressMenu(QWidget* parent)
	:QWidget(parent)
{
	QWidget* menu_widget_shell = BaseWindow<QWidget>::getPopWindowShell(this, "note_move_button_background_widget"
		,true, QPoint(0,0), 10, QColor(247, 157, 44));
	
	QVBoxLayout* menu_layout = new QVBoxLayout();
	menu_layout->setContentsMargins(0, 0, 0, 0);
	menu_layout->setSpacing(0);

	move_up_button_ = new QPushButton(u8"ÉÏÒÆ");
	move_up_button_->setWhatsThis("note_move_button");
	move_down_button_ = new QPushButton(u8"ÏÂÒÆ");
	move_down_button_->setWhatsThis("note_move_button");
	move_delete_button_ = new QPushButton(u8"É¾³ý");
	move_delete_button_->setWhatsThis("note_move_button");

	menu_layout->addWidget(move_up_button_);
	menu_layout->addWidget(move_down_button_);
	menu_layout->addWidget(move_delete_button_);

	menu_widget_shell->setLayout(menu_layout);

	int button_width = 120;
	int button_height = 40;

	move_up_button_->setFixedSize({ button_width ,button_height });
	move_down_button_->setFixedSize({ button_width ,button_height });
	move_delete_button_->setFixedSize({ button_width ,button_height });

	connect(move_up_button_, &QPushButton::pressed, this, &NoteTitleRightPressMenu::onMoveUpPressed);
	connect(move_down_button_, &QPushButton::pressed, this, &NoteTitleRightPressMenu::onMoveDownPressed);
	connect(move_delete_button_, &QPushButton::pressed, this, &NoteTitleRightPressMenu::onMoveDeletePressed);

	refreshMenu();
}

void NoteTitleRightPressMenu::refreshMenu()
{
	move_up_button_->setVisible(true);
	move_down_button_->setVisible(true);
	move_delete_button_->setVisible(true);

	updateMenu(true, true, true);
}

void NoteTitleRightPressMenu::updateMenu(bool move_up, bool move_down, bool move_delete)
{
	int button_num = 0;

	if (move_up)
		++button_num;
	else
		move_up_button_->setVisible(false);

	if (move_down)
		++button_num;
	else
		move_down_button_->setVisible(false);

	if (move_delete)
		++button_num;
	else
		move_delete_button_->setVisible(false);
}

void NoteTitleRightPressMenu::onMoveUpPressed()
{
	emit moveUpPressed();
}

void NoteTitleRightPressMenu::onMoveDownPressed()
{
	emit moveDownPressed();
}

void NoteTitleRightPressMenu::onMoveDeletePressed()
{
	emit moveDeletePressed();
}
