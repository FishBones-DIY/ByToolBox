#include "note_title_button.h"

#include <QVBoxLayout>
#include <QHBoxLayout>

void NoteTitleButton::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::RightButton)//判断事件为鼠标右键
	{
		int index = this->property("note_index").toInt();
		emit rightPressed(event->globalPos(), index);
	}
	QPushButton::mousePressEvent(event);
}