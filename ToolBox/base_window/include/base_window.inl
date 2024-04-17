#include "base_window.h"

#include <QDir>
#include <QScrollArea>
#include <QGraphicsDropShadowEffect>

template<class T>
BaseWindow<T>::BaseWindow(QWidget* parent)
{
	LoadPublicResource();
}

template<class T>
QHBoxLayout* BaseWindow<T>::baseCreateCaptionLayout(const QString& logo/* = ""*/, const QString& title/* = ""*/,
	Qt::WindowFlags type/* = (Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint | Qt::WindowTitleHint)*/,
	int customizeType/* = SafeUI::ShowIcon*/)
{
	QHBoxLayout* captionLayout = new QHBoxLayout();
	captionLayout->setContentsMargins(0, 11, 0, 0);
	captionLayout->addSpacing(10);

	QWidget* logoTitleWidget = new QWidget();
	int nTitleWidgetHeight = 32;
	logoTitleWidget->setFixedHeight(nTitleWidgetHeight);
	QHBoxLayout* logoTitleLayout = new QHBoxLayout();
	logoTitleLayout->setContentsMargins(0, 0, 0, 0);
	logoTitleLayout->setSpacing(0);
	logoTitleWidget->setLayout(logoTitleLayout);

	if (customizeType & ByPublicUI::ShowIcon)
	{
		m_logoLabel = new QLabel();
		QSize size = { 30,30 };
		m_logoLabel->setFixedSize(size);

		QString titlebarLogo = logo;
		if (customizeType & ByPublicUI::DefaultTitleIcon)
			titlebarLogo = ":/public/default_logo";

		baseSetIcon(titlebarLogo, size);
		logoTitleLayout->addWidget(m_logoLabel);
		logoTitleLayout->addSpacing(5);
	}

	if (customizeType & ByPublicUI::SetDefaultWindowIcon)
	{
		QString windowIcon = ":/public/default_logo";
		setWindowIcon(QIcon(windowIcon));
	}

	if (type & Qt::WindowTitleHint)
	{
		m_titleLabel = new QLabel();
		m_titleLabel->setWhatsThis("default_title");

		baseSetCaptionTitle(title);
		logoTitleLayout->addWidget(m_titleLabel);
	}

	if (logoTitleLayout->count() != 0)
	{
		captionLayout->addWidget(logoTitleWidget);
	}

	captionLayout->addStretch(0);

	if (type & Qt::WindowMinimizeButtonHint) {
		TButton4* minBtn = new TButton4(":/public/min", 0);
		minBtn->setFocusPolicy(Qt::NoFocus);
		connect(minBtn, SIGNAL(clicked()), this, SLOT(showMinimized()));
		captionLayout->addWidget(minBtn);
	}

	if (type & Qt::WindowMaximizeButtonHint) {
		TButton4* maxBtn = new TButton4(":/public/max", 0);
		maxBtn->setFocusPolicy(Qt::NoFocus);
		connect(maxBtn, SIGNAL(clicked()), this, SLOT(showMaximized()));
		captionLayout->addWidget(maxBtn);
	}

	if (type & Qt::WindowCloseButtonHint) {
		TButton4* closeBtn = new TButton4(":/public/close", 0);
		closeBtn->setFocusPolicy(Qt::NoFocus);
		connect(closeBtn, SIGNAL(clicked()), this, SLOT(close()));
		captionLayout->addWidget(closeBtn);
	}

	captionLayout->addSpacing(20);

	return captionLayout;
}

template<class T>
bool BaseWindow<T>::baseSetIcon(const QString& icon, const QSize& size /* = { 16,16 } */)
{
	if (!m_logoLabel || icon.isEmpty())
		return false;

	if (icon.endsWith(".ico", Qt::CaseInsensitive))
	{
		QIcon logoIcon(icon);
		m_logoLabel->setPixmap(logoIcon.pixmap(size));
	}
	else
	{
		QIcon defaultIcon(icon);
		m_logoLabel->setPixmap(defaultIcon.pixmap(size));
	}
	return true;
}

template<class T>
bool BaseWindow<T>::baseSetCaptionTitle(const QString& title)
{
	if (m_titleLabel == nullptr)
		return false;

	m_titleLabel->setText(title);
	m_titleLabel->setMinimumWidth(QFontMetrics(m_titleLabel->font()).width(title) + 2);

	return true;
}

template<class T>
void BaseWindow<T>::mousePressEvent(QMouseEvent* e)
{
	if ((m_nCaptionHeight == 0 || e->y() < m_nCaptionHeight) && e->button() == Qt::LeftButton)
	{
		m_bMousePressed = true;
		m_clickPos = e->pos();
	}
	return T::mousePressEvent(e);
}

template<class T>
void BaseWindow<T>::mouseMoveEvent(QMouseEvent* e)
{
	if (m_bMousePressed && e->buttons() & Qt::LeftButton)
	{
		move(e->pos() + pos() - m_clickPos);
	}
	return T::mouseMoveEvent(e);
}

template<class T>
void BaseWindow<T>::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_bMousePressed = false;
	}
	return T::mouseMoveEvent(event);
}

template<class T>
bool BaseWindow<T>::LoadPublicResource()
{
	QString resPath = QDir::currentPath() + "\\base_window\\res\\base_window.rcc";
	bool res = QResource::registerResource(resPath);
	//setStyleSheet(loadQSS(":/public/public_style"));
	addQSSStyleSheet(":/public/public_style");
	return res;
}

template<class T>
const void BaseWindow<T>::addQSSStyleSheet(const QString& qssPath)
{
	addStyleSheet(loadQSS(qssPath));
}

template<class T>
const void BaseWindow<T>::addStyleSheet(const QString& sheet)
{
	QString appendSheet = styleSheet();
	appendSheet.append("\n" + sheet);
	// 先设为空，否则部分qss会失效
	setStyleSheet("");
	setStyleSheet(appendSheet);
}

template<class T>
const QString BaseWindow<T>::loadQSS(const QString& fileName)
{
	QString styleSheet;
	QFile qss(fileName);
	if (qss.open(QFile::ReadOnly))
	{
		styleSheet = qss.readAll();
		qss.close();
	}

	return styleSheet;
}

template<class T>
QWidget* BaseWindow<T>::addScrollArea(QWidget* widget, const QString& whitsThis)
{
	QScrollArea* sa_widget = new QScrollArea();
	sa_widget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	sa_widget->setWidget(widget); //绑定widget scrollArea成为了widget的父容器
	sa_widget->setWidgetResizable(true);//在widget要显示的内容超出范围时 能够改变widget大小 显示内容
	sa_widget->setWhatsThis(whitsThis);

	return sa_widget;
}

template<class T>
QWidget* BaseWindow<T>::getPopWindowShell(
	QWidget* widget,
	const QString& whitsThis,
	bool have_shadow /*= false*/,
	QPoint shadow_offset /*= { 0,0 }*/,
	int shadow_blur_radius /*= 0*/,
	QColor shadow_color /*= { 0,0,0 }*/,
	Qt::WindowFlags window_flags /*= Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint*/,
	Qt::WidgetAttribute window_arttribute /*= Qt::WA_TranslucentBackground*/)
{
	widget->setAttribute(window_arttribute);
	widget->setWindowFlags(window_flags);

	QWidget* widget_shell = new QWidget();
	widget_shell->setWhatsThis(whitsThis);

	QVBoxLayout* layout_shell = new QVBoxLayout();
	
	QGraphicsDropShadowEffect* shadow = nullptr;
	if (have_shadow)
	{
		shadow = new QGraphicsDropShadowEffect();
		shadow->setOffset(shadow_offset);//设置偏移
		shadow->setBlurRadius(shadow_blur_radius);//设置模糊半径
		shadow->setColor(shadow_color);//设置颜色
		layout_shell->setContentsMargins(shadow_blur_radius, shadow_blur_radius, shadow_blur_radius, shadow_blur_radius);
	}
	else
	{
		layout_shell->setContentsMargins(0, 0, 0, 0);
	}

	layout_shell->setSpacing(0);
	layout_shell->addWidget(widget_shell);

	widget_shell->setGraphicsEffect(shadow);
	widget->setLayout(layout_shell);

	return widget_shell;
}