#pragma once

#include <vector>

#include <QLabel>
#include <QPushButton>
#include <QButtonGroup>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedWidget>

class ByTabWidget : public QWidget
{
	Q_OBJECT
public:
	ByTabWidget(QWidget* parent = Q_NULLPTR);
public:
	int addTab(QWidget* pane_widget, const QString& bar_image_path, const QString& bar_text);
public slots:
	void onTabBarClicked();
private:
	QPushButton* createTabButton(const QString& image_path, const QString& text);
	
private:
	QVBoxLayout* tab_widget_layout_{ nullptr };
	QButtonGroup* tab_bar_group_{ nullptr };
	QHBoxLayout* bar_layout_{ nullptr };
	QHBoxLayout* bar_button_layout_{ nullptr };
	QWidget* parent_widget_{ nullptr };
	QStackedWidget* main_stack_widget_{ nullptr };
	std::vector<QPushButton*> tab_bar_list_;
};