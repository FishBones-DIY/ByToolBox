#pragma once
#include <QLabel>
#include <QWidget>
#include <QDialog>
#include <QTextEdit>
#include <QPushButton>

#include "common/public/nocopyable.h"
#include "base_window/include/base_window.h"

// 设在按钮个数
enum MSG_TYPE
{
	SA_OK = 0,         // 只有一个“确定”按钮，且两秒钟后自动关闭消息窗口
	SA_OKCANCEL = 1,   // 一个“是”按钮，一个“否”按钮
	SA_OKS = 2         // 只有一个“确定”按钮，不会自动关闭消息窗口
};

// 设在提示的类型
enum MSG_TIP_TYPE
{
	SA_SUCCESS = 0,    // 完成
	SA_FAILED = 1,     // 错误
	SA_WARNING = 2,    // 警告
	SA_TIPS = 3,       // 提示
	SA_QUESTION = 4    // 未知
};

class ByMessageBox : public BaseWindow<QDialog>, private TOOLBOX::NoCopyable
{
	Q_OBJECT
public:
	// 单例，唯一入口
	static ByMessageBox& get()
	{
		static ByMessageBox message_box;
		return message_box;
	}
public:
	int popup(const QString& title, const QString& content);
	int popup(const QString& title, const QString& content, const QString& ok_text, const QString& cancel_text);
	int popup(QWidget* parent,bool darken,const QString& title, const QString& content, const QString& ok_text, const QString& cancel_text);
protected:
	ByMessageBox(QWidget* parent = Q_NULLPTR);
	virtual ~ByMessageBox() = default;


signals:

public slots:
	void onOkButtonClicked();
	void onCancelButtonClicked();

private:
	QLabel* content_{ nullptr };
	QLabel* bar_image_{ nullptr };
	QLabel* bar_title_{ nullptr };
	QPushButton* cancel_button_{ nullptr };
	QPushButton* ok_button_{ nullptr };
	QWidget* main_widget_{ nullptr };
};

#define MessageBox_By ByMessageBox::get().popup