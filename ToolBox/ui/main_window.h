#pragma once

#include <QMainWindow>
#include <QTabWidget>
#include <QPushButton>
#include <QtWidgets/QWidget>

#include "base_window/include/base_window.h"

namespace TOOLBOX
{
	const wchar_t* const kClientUIProductName = L"Byπ§æﬂœ‰";

	class MainWindow : public BaseWindow<QWidget>
	{
		Q_OBJECT

	public:
		MainWindow(QWidget* parent = Q_NULLPTR) ;
	protected:

	private:
		QWidget* createTitleBarWidget(QWidget* parent);
		QWidget* createMainTabWidget(QWidget* parent);
		QWidget* createShortcutKeyWidget(QWidget* parent);
		QWidget* createNoteWidget(QWidget* parent);
		QWidget* createBottomDetailsWidget(QWidget* parent);

	private:
		

	};

} // namespace TOOLBOX