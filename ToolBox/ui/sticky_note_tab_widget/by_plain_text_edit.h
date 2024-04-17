#pragma once

#include <QTextEdit>
#include <QMouseEvent>

class ByTextEdit : public QTextEdit
{
	Q_OBJECT
public:
	ByTextEdit(QWidget* parent = Q_NULLPTR): QTextEdit(parent){};
protected:
	virtual void wheelEvent(QWheelEvent* e)override;//¹öÂÖÊÂ¼ş
};