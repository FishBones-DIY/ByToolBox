#pragma once

#include "tbutton4.h"

#include <QLabel>
#include <QWidget>
#include <QResource>
#include <QHBoxLayout>

namespace ByPublicUI
{

	static const Qt::WindowFlags defaultFlags = Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint | Qt::WindowTitleHint;

	enum CustomizeWindowFlag
	{
		None = 0x00000000,
		ShowIcon = 0x00000001,
		DefaultTitleIcon = 0x00000002,						// 使用默认图标，会随定制化改变
		SetDefaultWindowIcon = 0x00000004,					// 设置默认窗口图标（显示在任务栏等）
	};

	enum ClientUIEventFlag
	{
		kUnkown = 0x00000000,
		kTitlebarLogo = 0x00000001,
		kProductName = 0x00000002,
	};

} // namespace ByPublicUI

template<class T>
class BaseWindow : public T
{
public:
	BaseWindow(QWidget* parent = Q_NULLPTR);
public:
	QHBoxLayout* baseCreateCaptionLayout(const QString& logo = "", const QString& title = "", Qt::WindowFlags type = (Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint | Qt::WindowTitleHint), int customizeType = SafeUI::ShowIcon);
	bool baseSetIcon(const QString& icon, const QSize& size = { 16,16 });
	bool baseSetCaptionTitle(const QString& title);
	bool LoadPublicResource();

	const void addQSSStyleSheet(const QString& qssPath);
	const void addStyleSheet(const QString& sheet);
	static const QString loadQSS(const QString& fileName);

	static QWidget* addScrollArea(QWidget* widget,const QString& whitsThis);
	static QWidget* getPopWindowShell(QWidget* widget, const QString& whitsThis, bool have_shadow = false, 
		QPoint shadow_offset = { 0,0 }, 
		int shadow_blur_radius = 0, 
		QColor shadow_color = {0,0,0}, 
		Qt::WindowFlags window_flags = Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint,
		Qt::WidgetAttribute window_arttribute = Qt::WA_TranslucentBackground);
protected:
	virtual void mousePressEvent(QMouseEvent* e) override;

	virtual void mouseMoveEvent(QMouseEvent* e)override;

	virtual void mouseReleaseEvent(QMouseEvent* e)override;

private:
	QLabel* m_logoLabel;
	QLabel* m_titleLabel;

	QPoint m_clickPos;
	int m_nCaptionHeight;
	bool m_bMousePressed;
};

#include "base_window.inl"
