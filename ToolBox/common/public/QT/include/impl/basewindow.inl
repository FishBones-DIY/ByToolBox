#include <QT/controls/basewindow.h>

template<class T>
inline QWidget* BaseWindow<T>::baseCreateCaption(const QString& logo/* = ""*/, const QString& title/* = ""*/, Qt::WindowFlags type/* = (Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint | Qt::WindowTitleHint)*/, int customizeType/* = SafeUI::ShowIcon*/)
{
	QWidget* captionWidget = new QWidget();
	QHBoxLayout* captionLayout = baseCreateCaptionLayout(logo, title, type, customizeType);
	captionWidget->setLayout(captionLayout);
	return captionWidget;
}

template<class T>
QHBoxLayout* BaseWindow<T>::baseCreateCaptionLayout(const QString& logo/* = ""*/, const QString& title/* = ""*/, Qt::WindowFlags type/* = (Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint | Qt::WindowTitleHint)*/, int customizeType/* = SafeUI::ShowIcon*/)
{
	QHBoxLayout* captionLayout = new QHBoxLayout();
	captionLayout->setContentsMargins(10, 0, 0, 0);
	captionLayout->setSpacing(0);

	QWidget* logoTitleWidget = new QWidget();
	logoTitleWidget->setFixedHeight(37);
	QHBoxLayout* logoTitleLayout = new QHBoxLayout();
	logoTitleLayout->setContentsMargins(0, 0, 0, 0);
	logoTitleLayout->setSpacing(0);
	logoTitleWidget->setLayout(logoTitleLayout);

	using namespace NsCustomize;
	customize_type_ = customizeType;
	if (customizeType & SafeUI::ShowIcon)
	{
		m_logoLabel = new QLabel();
		m_logoLabel->setFixedSize(18, 18);

		QString titlebarLogo = logo;
		if (customizeType & SafeUI::DefaultTitleIcon)
			titlebarLogo = QString::fromStdWString(CustomizeHelper::GetCustomizeTitlebarLogo(GetDefaultLogo().c_str()));

		baseSetIcon(titlebarLogo);
		logoTitleLayout->addWidget(m_logoLabel);
		logoTitleLayout->addSpacing(5);
	}

	if (customizeType & SafeUI::SetDefaultWindowIcon)
	{
		QString windowIcon = QString::fromStdWString(CustomizeHelper::GetCustomizeTitlebarLogo(CustomizeHelper::GetDefaultWindowIcon().c_str()));
		setWindowIcon(QIcon(windowIcon));
	}

	if (type & Qt::WindowTitleHint)
	{
		m_titleLabel = new QLabel();
		if (customizeType & SafeUI::NoSkinChangeTitle) m_titleLabel->setWhatsThis("default_title");
		else m_titleLabel->setWhatsThis("skin_change_bold_12");
		
		baseSetCaptionTitle(title);
		logoTitleLayout->addWidget(m_titleLabel);
	}

	if (logoTitleLayout->count() != 0)
	{
		captionLayout->addWidget(logoTitleWidget);
		m_nLeftInsertIndex++;
	}

	captionLayout->addStretch(0);

	if (type & Qt::WindowMinimizeButtonHint) {
		TButton4* minBtn = new TButton4(":/public/min");
		minBtn->setFocusPolicy(Qt::NoFocus);
		connect(minBtn, SIGNAL(clicked()), this, SLOT(showMinimized()));
		captionLayout->addWidget(minBtn);
	}

	if (type & Qt::WindowMaximizeButtonHint) {
		TButton4* maxBtn = new TButton4(":/public/max");
		maxBtn->setFocusPolicy(Qt::NoFocus);
		connect(maxBtn, SIGNAL(clicked()), this, SLOT(showMaximized()));
		captionLayout->addWidget(maxBtn);
	}

	if (type & Qt::WindowCloseButtonHint) {
		TButton4* closeBtn = new TButton4(":/public/close");
		closeBtn->setFocusPolicy(Qt::NoFocus);
		connect(closeBtn, SIGNAL(clicked()), this, SLOT(close()));
		captionLayout->addWidget(closeBtn);
	}
	return captionLayout;
}

template<class T>
void BaseWindow<T>::baseSetResizeble(bool bResizeble)
{
	m_bResizeble = bResizeble;
}

template<class T>
void BaseWindow<T>::baseSetCaptionHeight(int nCaptionHeight)
{
	m_nCaptionHeight = nCaptionHeight;;
}

template<class T>
int BaseWindow<T>::baseGetLeftInsertIndex()
{
	return m_nLeftInsertIndex;
}

template<class T>
bool BaseWindow<T>::baseSetCaptionTitle(const QString& title)
{
	if (m_titleLabel == nullptr)
		return false;

	m_titleLabel->setText(title);
	m_titleLabel->setMinimumWidth(QFontMetrics(m_titleLabel->font()).width(title)+2);

	return true;
}

template<class T>
bool BaseWindow<T>::baseSetIcon(const QString& icon)
{
	if (!m_logoLabel || icon.isEmpty())
		return false;

	if (icon.endsWith(".ico", Qt::CaseInsensitive))
	{
		QIcon logoIcon(icon);
		m_logoLabel->setPixmap(logoIcon.pixmap(16, 16));
	}
	else
	{
		QIcon defaultIcon(icon);
		m_logoLabel->setPixmap(defaultIcon.pixmap(16, 16));
	}
	return true;
}

template<class T>
bool BaseWindow<T>::nativeEvent(const QByteArray& eventType, void* message, long* result)
{
	(eventType);

#ifdef Q_OS_WIN
	int boundaryWidth = 4;
	MSG* msg = reinterpret_cast<MSG*>(message);
	switch (msg->message)
	{
	case WM_NCCALCSIZE:
	{
		NCCALCSIZE_PARAMS* lpncsp = (NCCALCSIZE_PARAMS*)(msg->lParam);
		if (msg->wParam) 
		{
			lpncsp->rgrc[2] = lpncsp->rgrc[1];
			lpncsp->rgrc[1] = lpncsp->rgrc[0];
			*result = 0;
		}
		return true;
	}
	case WM_NCACTIVATE:
	{
		if (T::isMinimized() || !msg->wParam)
		{
			*result = true; // system should proceed with the default processing
		}
		return true;
	}
	case WM_NCPAINT:
	{
		if (!m_bDrawShadow)
			return true;
	}
	case WM_NCHITTEST:
	{
		if (!m_bResizeble)
			break;

		int xPos = GET_X_LPARAM(msg->lParam) - frameGeometry().x();
		int yPos = GET_Y_LPARAM(msg->lParam) - frameGeometry().y();
		if (xPos < boundaryWidth && yPos < boundaryWidth)                    //左上角
			*result = HTTOPLEFT;
		else if (xPos >= width() - boundaryWidth && yPos < boundaryWidth)          //右上角
			*result = HTTOPRIGHT;
		else if (xPos < boundaryWidth && yPos >= height() - boundaryWidth)         //左下角
			*result = HTBOTTOMLEFT;
		else if (xPos >= width() - boundaryWidth && yPos >= height() - boundaryWidth)//右下角
			*result = HTBOTTOMRIGHT;
		else if (xPos < boundaryWidth)                                     //左边
			*result = HTLEFT;
		else if (xPos >= width() - boundaryWidth)                              //右边
			*result = HTRIGHT;
		else if (yPos < boundaryWidth)                                       //上边
			*result = HTTOP;
		else if (yPos >= height() - boundaryWidth)                             //下边
			*result = HTBOTTOM;
		else
			return T::nativeEvent(eventType, message, result);
	}
	default:
		return T::nativeEvent(eventType, message, result);
	}
#endif
	return T::nativeEvent(eventType, message, result);
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
void BaseWindow<T>::showEvent(QShowEvent* event)
{
	if (m_bFisrtDrawShadow && event->type() == QShowEvent::Show){
		DrawFramelessWindowShadow();
		m_bFisrtDrawShadow = false;
	}
	return T::showEvent(event);
}

template<class T>
void BaseWindow<T>::GetWindowsVersion()
{
	Base::CObjectLoader baseLoader(MODULE_QAX_BASE);
	ISpecialValuePtr spSpecialValue = baseLoader.CreateObject<ISpecialValuePtr>();
	if (nullptr == spSpecialValue) {
		m_dwMajorVersion = 6;
		return;
	}
	m_dwMajorVersion = spSpecialValue->GetOSVersionInfo().dwMajorVersion;
}

template<class T>
bool BaseWindow<T>::IsWindowsVistaLater()
{
	return m_dwMajorVersion >= 6;
}

template<class T>
void BaseWindow<T>::DrawFramelessWindowShadow()
{
#ifdef Q_OS_WIN
	HWND hwnd = reinterpret_cast<HWND>(winId());
	LONG style = GetWindowLongPtr(hwnd, GWL_STYLE);
	// 去除无边框不支持的 Window style
	style &= ~(WS_THICKFRAME);
	if (IsWindowsVistaLater()) {
		style &= ~(WS_SYSMENU);
	}

	style |= (WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_CAPTION);
	SetWindowLongPtr(hwnd, GWL_STYLE, style);

	const MARGINS shadow = { 1, 1, 1, 1 };
	auto DwmExtendFrameIntoClientAreaApi = GetCompatibleWin32API(L"dwmapi.dll", DwmExtendFrameIntoClientArea);
	if (! DwmExtendFrameIntoClientAreaApi || DwmExtendFrameIntoClientAreaApi(hwnd, &shadow) != S_OK)
		m_bDrawShadow = false;
#endif
}
