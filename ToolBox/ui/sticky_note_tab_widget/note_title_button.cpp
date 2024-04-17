#include "note_title_button.h"

#include <QVBoxLayout>
#include <QHBoxLayout>

void NoteTitleButton::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::RightButton)//�ж��¼�Ϊ����Ҽ�
	{
		int index = this->property("note_index").toInt();
		emit rightPressed(event->globalPos(), index);
	}
	QPushButton::mousePressEvent(event);
}