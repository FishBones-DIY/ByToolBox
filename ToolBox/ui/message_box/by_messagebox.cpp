#include "by_messagebox.h"

#include <QApplication>
#include <QDesktopWidget>

ByMessageBox::ByMessageBox(QWidget* parent) : BaseWindow<QDialog>(parent)
{
	addQSSStyleSheet(":/MainWindow/res/qss/main_window.qss");

	main_widget_ = BaseWindow<QWidget>::getPopWindowShell(this, "by_message_box"
		, true, QPoint(0, 0), 10, QColor(200, 200, 200), Qt::Dialog | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
	this->setAttribute(Qt::WA_ShowModal, true);
	this->setWindowModality(Qt::ApplicationModal);

	QVBoxLayout* main_layout = new QVBoxLayout();
	main_layout->setContentsMargins(0, 0, 0, 0);
	main_layout->setSpacing(0);

	QHBoxLayout* bar_layout = new QHBoxLayout();
	bar_layout->setContentsMargins(0, 22, 0, 0);
	bar_layout->setSpacing(0);

	bar_image_ = new QLabel();
	bar_title_ = new QLabel();

	QPixmap image_saved(":/public/message_box_warning");
	bar_image_->setPixmap(image_saved);

	bar_title_->setWhatsThis("by_message_box_title_label");

	bar_layout->addSpacing(20);
	bar_layout->addWidget(bar_image_);
	bar_layout->addSpacing(8);
	bar_layout->addWidget(bar_title_);
	bar_layout->addStretch();

	content_ = new QLabel();
	content_->setTextInteractionFlags(Qt::TextSelectableByMouse);
	content_->setWhatsThis("by_message_box_content_label");
	content_->setWordWrap(true);                     // true：自动换行
	content_->setAlignment(Qt::AlignVCenter);        // 对齐方式
	QHBoxLayout* button_layout = new QHBoxLayout();
	button_layout->setContentsMargins(0, 0, 0, 20);
	button_layout->setSpacing(0);

	cancel_button_ = new QPushButton();
	ok_button_ = new QPushButton();

	cancel_button_->setWhatsThis("by_message_box_cancel_button");
	ok_button_->setWhatsThis("by_message_box_ok_button");

	connect(cancel_button_, &QPushButton::pressed, this, &ByMessageBox::onCancelButtonClicked);
	connect(ok_button_, &QPushButton::pressed, this, &ByMessageBox::onOkButtonClicked);

	button_layout->addStretch();
	button_layout->addWidget(cancel_button_);
	button_layout->addSpacing(10);
	button_layout->addWidget(ok_button_);
	button_layout->addSpacing(20);

	main_layout->addLayout(bar_layout);
	main_layout->addWidget(content_);
	main_layout->addLayout(button_layout);
	main_layout->addStretch();

	main_widget_->setLayout(main_layout);
}

int ByMessageBox::popup(const QString& title, const QString& content)
{
	return popup(title, content, "ok", "cancel");
}

int ByMessageBox::popup(const QString& title, const QString& content, const QString& ok_text, const QString& cancel_text)
{
	bar_title_->setText(title);
	content_->setText(content);

	if (cancel_text.isEmpty())
		cancel_button_->hide();
	else
	{
		cancel_button_->show();
		cancel_button_->setText(cancel_text);
	}

	if (ok_text.isEmpty())
		ok_button_->hide();
	else
	{
		ok_button_->show();
		ok_button_->setText(ok_text);
	}
	

	return this->exec();
}

int ByMessageBox::popup(QWidget* parent, bool darken, const QString& title, const QString& content, const QString& ok_text, const QString& cancel_text)
{
	QWidget* widget = new QWidget(parent, Qt::FramelessWindowHint);
	widget->resize(parent->size());
	widget->setWhatsThis(parent->whatsThis() + "_darken");
	widget->show();

	QPoint global_pos = parent->mapToGlobal(QPoint(0, 0));
	QRect rect = parent->geometry();

	int x = global_pos.x() + rect.width() / 2 - 200;
	int y = global_pos.y() + rect.height() / 2 - 90;
	this->move(x, y);

	int ret = popup(title, content, ok_text, cancel_text);
	widget->hide();

	return ret;
}

void ByMessageBox::onOkButtonClicked()
{
	done(1);
	this->close();
}

void ByMessageBox::onCancelButtonClicked()
{
	done(0);
	this->close();
}