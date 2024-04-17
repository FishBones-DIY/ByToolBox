#pragma once

#include <QMenu>
#include <QWidget>
#include <QPushButton>
#include <QMouseEvent>

class NoteTitleRightPressMenu : public QWidget
{
	Q_OBJECT
public:
	NoteTitleRightPressMenu(QWidget* parent = Q_NULLPTR);

public:
	void updateMenu(bool move_up = true, bool move_down = true , bool move_delete = true);
	void refreshMenu();
signals:
	void moveUpPressed();
	void moveDownPressed();
	void moveDeletePressed();

public slots:
	void onMoveUpPressed();
	void onMoveDownPressed();
	void onMoveDeletePressed();
private:
	QPushButton* move_up_button_{ nullptr };
	QPushButton* move_down_button_{ nullptr };
	QPushButton* move_delete_button_{ nullptr };
};