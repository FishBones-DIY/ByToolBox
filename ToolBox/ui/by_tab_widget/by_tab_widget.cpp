#include "by_tab_widget.h"

#include <QVariant>

#include "base_window/include/base_window.h"


ByTabWidget::ByTabWidget(QWidget* parent)
	:QWidget(parent)
{
	tab_widget_layout_ = new QVBoxLayout();
	tab_widget_layout_->setContentsMargins(0, 0, 0, 0);
	tab_widget_layout_->setSpacing(0);
	bar_layout_ = new QHBoxLayout();
	bar_layout_->setContentsMargins(0, 0, 0, 0);
	bar_layout_->setSpacing(0);
	bar_button_layout_ = new QHBoxLayout();
	bar_button_layout_->setContentsMargins(0, 0, 0, 0);
	bar_button_layout_->setSpacing(0);
	main_stack_widget_ = new QStackedWidget();
	main_stack_widget_->setWhatsThis("by_tab_stack_widget");

	tab_bar_group_ = new QButtonGroup();

	bar_layout_->addSpacing(40);
	bar_layout_->addLayout(bar_button_layout_);
	bar_layout_->addStretch();
	tab_widget_layout_->addLayout(bar_layout_);
	tab_widget_layout_->addWidget(main_stack_widget_);
	this->setLayout(tab_widget_layout_);
};

int ByTabWidget::addTab(QWidget* pane_widget, const QString& bar_image_path, const QString& bar_text)
{
	Q_ASSERT(pane_widget);
	if (!pane_widget) return -1;
	int index = main_stack_widget_->addWidget(BaseWindow<QWidget>::addScrollArea(pane_widget, "by_tab_scroll_area"));
	QPushButton* bar_button = createTabButton(bar_image_path, bar_text);
	bar_button->setCheckable(true);
	tab_bar_list_.emplace_back(bar_button);
	tab_bar_group_->addButton(bar_button, index);
	bar_button_layout_->addWidget(bar_button);
	bar_button->setProperty("tab_index", tab_bar_list_.size() - 1);

	if (1 == index)
	{
		bar_button->setChecked(true);
		main_stack_widget_->setCurrentIndex(index);
	}

	connect(bar_button, &QPushButton::pressed, this, &ByTabWidget::onTabBarClicked);

	return index;
}

void ByTabWidget::onTabBarClicked()
{
	QWidget* bar_widget = qobject_cast<QWidget*>(sender());
	Q_ASSERT(bar_widget);
	if (!bar_widget) return;

	int index = bar_widget->property("tab_index").toInt();
	tab_bar_list_[index]->setChecked(true);
	main_stack_widget_->setCurrentIndex(index);
}

QPushButton* ByTabWidget::createTabButton(const QString& image_path, const QString& text)
{
	QPushButton* tab_button = new QPushButton();
	tab_button->setWhatsThis("by_tab_button");
	tab_button->setFixedHeight(33);
	tab_button->setFixedWidth(100);


	QLabel* button_image = new QLabel();
	QLabel* button_text = new QLabel();
	button_text->setWhatsThis("by_tab_button_text");

	QPixmap pixmap(image_path);
	QSize image_size(20, 20);
	pixmap = pixmap.scaled(image_size, Qt::KeepAspectRatio);
	button_image->setPixmap(pixmap);

	button_text->setText(text);

	QHBoxLayout* tab_button_layout = new QHBoxLayout();

	tab_button_layout->addStretch();
	tab_button_layout->addWidget(button_image);
	tab_button_layout->addSpacing(2);
	tab_button_layout->addWidget(button_text);
	tab_button_layout->addStretch();

	tab_button->setLayout(tab_button_layout);

	return tab_button;
}