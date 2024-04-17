#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include <functional>

#include <Public/Helper/qaxpublic.h>
#include <QAXBase/Interface/IUIThread.h>
#include <QAXBase/Helper/TaskHelper.h>
#include <Skylar/helper/customizehelper.h>
#include <Skylar/SkylarID.h>

#include <QObject>
#include <QPoint>
#include <QLabel>
#include <QHBoxLayout>
#include <QDebug>
#include <QFontMetrics>
#include <QPointer>

#include "tbutton4.h"

#ifdef Q_OS_WIN
#include <windowsx.h>
#include <QAXBase.win32/Helper/Win32APIHelper.h>
#include <dwmapi.h>
#pragma comment (lib, "User32.lib")
#endif

class QWidget;
class QMouseEvent;

namespace SafeUI
{
	static const Qt::WindowFlags defaultFlags = Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint | Qt::WindowTitleHint;

	enum CustomizeWindowFlag
	{
		None = 0x00000000,
		ShowIcon = 0x00000001,
		DefaultTitleIcon = 0x00000002,						// 使用默认图标，会随定制化改变
		SetDefaultWindowIcon = 0x00000004,					// 设置默认窗口图标（显示在任务栏等）
		NoSkinChangeTitle = 0x00000008						// 标题栏颜色不随定制变化
	};

	enum ClientUIEventFlag
	{
		kUnkown = 0x00000000,
		kTitlebarLogo = 0x00000001,
		kProductName = 0x00000002,
	};

	const wchar_t* const kClientUITitlebarLogo = L"client.ui.titlebar_logo";
	const wchar_t* const kClientUIProductName = L"client.ui.product_name";
}

template<class T>
class BaseWindow : public T
{
public:
	BaseWindow(QWidget* parent = nullptr, Qt::WindowFlags flags = 0)
		: T(parent)
		, m_bResizeble(false)
		, m_nCaptionHeight(0)
		, m_bMousePressed(false)
		, m_nLeftInsertIndex(0)
		, m_titleLabel(nullptr)
		, m_logoLabel(nullptr)
		, m_bDrawShadow(true)
		, m_bFisrtDrawShadow(true)
		, event_flag_(SafeUI::kUnkown)
		, customize_type_(SafeUI::None)
	{
		setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint | flags);
		setContextMenuPolicy(Qt::NoContextMenu);
		GetWindowsVersion();
	}

	virtual ~BaseWindow()
	{
		if (notifier_)
		{
			if (event_flag_ & SafeUI::kTitlebarLogo)
				notifier_->UnReginsterNotify(SafeUI::kClientUITitlebarLogo, client_ui_observer_);

			if (event_flag_ & SafeUI::kProductName)
				notifier_->UnReginsterNotify(SafeUI::kClientUIProductName, client_ui_observer_);
		}
	}

	QWidget* baseCreateCaption(const QString& logo = "", const QString& title = "", Qt::WindowFlags type = (Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint | Qt::WindowTitleHint), int customizeType = SafeUI::ShowIcon);

	QHBoxLayout* baseCreateCaptionLayout(const QString& logo = "", const QString& title = "", Qt::WindowFlags type = (Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint | Qt::WindowTitleHint), int customizeType = SafeUI::ShowIcon);

	void baseSetResizeble(bool bResizeble);

	void baseSetCaptionHeight(int nCaptionHeight);

	int baseGetLeftInsertIndex();

	bool baseSetCaptionTitle(const QString& title);

	bool baseSetIcon(const QString& icon);

	bool baseConcernClientUIEvent(const int& flag);

public:
	virtual void OnClientUITitlbarLogoChanged();

	virtual void OnClientUIProductNameChanged();

protected:
	virtual bool nativeEvent(const QByteArray& eventType, void* message, long* result);

	virtual void mousePressEvent(QMouseEvent* e);

	virtual void mouseMoveEvent(QMouseEvent* e);

	virtual void mouseReleaseEvent(QMouseEvent* e);

	virtual void showEvent(QShowEvent* e);

private:
	void GetWindowsVersion();

	bool IsWindowsVistaLater();

	void DrawFramelessWindowShadow();

	const std::wstring& GetDefaultLogo();

private:
	bool				m_bResizeble;
	int					m_nCaptionHeight;
	bool				m_bMousePressed;
	QPoint				m_clickPos;
	int					m_nLeftInsertIndex;
	QLabel*				m_titleLabel;
	QLabel*				m_logoLabel;

	bool				m_bFisrtDrawShadow;
	bool				m_bDrawShadow;
	DWORD				m_dwMajorVersion;

	IEventObserverPtr	client_ui_observer_;
	IEventNotifierPtr	notifier_;
	int					event_flag_;
	int					customize_type_;

	std::wstring		default_logo_;
};

template<class T>
class ClientUIObserver : public NsCommonConfig::DefaultEventObserverImpl<ClientUIObserver<T>>
{
public:
	ClientUIObserver(QPointer<BaseWindow<T>> base_window) : base_window_(base_window)
	{
		Base::CObjectLoader pluginLoader(MODULE_QAX_BASE);
		ui_thread_ = pluginLoader.CreateObject<decltype(ui_thread_)>();
	};

	virtual qax_result STDCALLTYPE OnKeyChanged(const wchar_t* lpKey, void* pParam = nullptr);

private:
	QPointer<BaseWindow<T>> base_window_;
	IUIThreadPtr ui_thread_;
};

#include "./impl/basewindow.inl"

#endif // BASEWINDOW_H
