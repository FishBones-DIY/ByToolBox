#include "by_plain_text_edit.h"

#include <QTextBrowser>

void ByTextEdit::wheelEvent(QWheelEvent* e) {
	if (e->modifiers() == Qt::ControlModifier)//ctrl键的判断
	{
		if (e->delta() > 0)
		{
			zoomIn();//放大
			return;
		}
		else
		{
			zoomOut();//缩小
			return;
		}
	}
	QTextEdit::wheelEvent(e);

}