#include "by_line_edit.h"

void ByLineEdit::focusInEvent(QFocusEvent* e) {
	emit focusIn();
	QLineEdit::focusInEvent(e);
}

void ByLineEdit::focusOutEvent(QFocusEvent* e) {
	emit focusOut();
	QLineEdit::focusOutEvent(e);
}