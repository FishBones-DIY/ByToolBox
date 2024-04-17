#pragma once
#include <QLabel>
#include <QWidget>
#include <QDialog>
#include <QTextEdit>
#include <QPushButton>

#include "common/public/nocopyable.h"
#include "base_window/include/base_window.h"

// ���ڰ�ť����
enum MSG_TYPE
{
	SA_OK = 0,         // ֻ��һ����ȷ������ť���������Ӻ��Զ��ر���Ϣ����
	SA_OKCANCEL = 1,   // һ�����ǡ���ť��һ�����񡱰�ť
	SA_OKS = 2         // ֻ��һ����ȷ������ť�������Զ��ر���Ϣ����
};

// ������ʾ������
enum MSG_TIP_TYPE
{
	SA_SUCCESS = 0,    // ���
	SA_FAILED = 1,     // ����
	SA_WARNING = 2,    // ����
	SA_TIPS = 3,       // ��ʾ
	SA_QUESTION = 4    // δ֪
};

class ByMessageBox : public BaseWindow<QDialog>, private TOOLBOX::NoCopyable
{
	Q_OBJECT
public:
	// ������Ψһ���
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