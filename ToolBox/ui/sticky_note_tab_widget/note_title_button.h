#pragma once

#include <QMenu>
#include <QPushButton>
#include <QMouseEvent>

class NoteTitleButton : public QPushButton
{
	Q_OBJECT
public:
	NoteTitleButton(QWidget* parent = Q_NULLPTR) :QPushButton(parent) {};
protected:
	virtual void mousePressEvent(QMouseEvent* e)override;

signals:
	void rightPressed(QPoint pos,int note_index);

};