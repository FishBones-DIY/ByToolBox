#include "by_plain_text_edit.h"

#include <QTextBrowser>

void ByTextEdit::wheelEvent(QWheelEvent* e) {
	if (e->modifiers() == Qt::ControlModifier)//ctrl�����ж�
	{
		if (e->delta() > 0)
		{
			zoomIn();//�Ŵ�
			return;
		}
		else
		{
			zoomOut();//��С
			return;
		}
	}
	QTextEdit::wheelEvent(e);

}