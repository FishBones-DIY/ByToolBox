#pragma once

#include <QLineEdit>
#include <QMouseEvent>

class ByLineEdit : public QLineEdit
{
	Q_OBJECT
public:
	ByLineEdit(QWidget* parent = Q_NULLPTR): QLineEdit(parent){};
protected:
	virtual void focusInEvent(QFocusEvent* e)override;
	virtual void focusOutEvent(QFocusEvent * e)override;
signals:
	void focusIn();
	void focusOut();

public slots:
};