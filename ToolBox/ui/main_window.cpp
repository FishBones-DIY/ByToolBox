#include "main_window.h"

#include <vector>
#include <memory>

#include <QFile>
#include <QTabBar>
#include <QWidget>
#include <QPushButton>
#include <QGridLayout>

#include "ui/shortcut_key_widget.h"
#include "ui/by_tab_widget/by_tab_widget.h"
#include "sticky_note_tab_widget/sticky_note_tab_widget.h"
#include "shortcut_keys/item_center/shortcut_key_manager.h"

namespace TOOLBOX
{

MainWindow::MainWindow(QWidget* parent)
    : BaseWindow<QWidget>(parent)
{
    addQSSStyleSheet(":/MainWindow/res/qss/main_window.qss");
 
    const int width = 1200;
    const int height = 800;

    this->setMinimumSize(width, height);
    this->setMaximumSize(width, height);


    QWidget* central_widget = BaseWindow<QWidget>::getPopWindowShell(this, "central_widget"
        , true, QPoint(0, 0), 10, QColor(200, 200, 200), Qt::Dialog | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);

    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(Qt::FramelessWindowHint);

    this->setObjectName("main_window");

    QVBoxLayout* central_layout = new QVBoxLayout(central_widget);
    central_layout->setContentsMargins(0, 0, 0, 0);

    central_layout->addWidget(createTitleBarWidget(central_widget));
    central_layout->addWidget(createMainTabWidget(central_widget));
    //central_layout->addSpacing(10);
    //central_layout->addWidget(createBottomDetailsWidget(central_widget));
    
    central_widget->setLayout(central_layout);

}

QWidget* MainWindow::createTitleBarWidget(QWidget* parent)
{
    QWidget* title_bar_widget = new QWidget(parent);
    title_bar_widget->setWhatsThis("main_title_bar");
    QHBoxLayout* captionLayout = baseCreateCaptionLayout("", 
        QString::fromWCharArray(kClientUIProductName), ByPublicUI::defaultFlags,
        ByPublicUI::ShowIcon | ByPublicUI::DefaultTitleIcon | ByPublicUI::SetDefaultWindowIcon);
    title_bar_widget->setLayout(captionLayout);
    return title_bar_widget;
}

QWidget* MainWindow::createBottomDetailsWidget(QWidget* parent)
{
    QWidget* title_bar_widget = new QWidget(parent);
    title_bar_widget->setFixedHeight(23);

    QHBoxLayout* details_layout = new QHBoxLayout();
    details_layout->setContentsMargins(0, 0, 0, 5);
    details_layout->setSpacing(0);

    QLabel* version_label = new QLabel();
    version_label->setText(u8"°æ±¾: 0.0.0.1");
    version_label->setWhatsThis("version_label");

    details_layout->addSpacing(30);
    details_layout->addWidget(version_label);
    details_layout->addStretch();

    title_bar_widget->setLayout(details_layout);


    return title_bar_widget;
}

QWidget* MainWindow::createMainTabWidget(QWidget* parent)
{
    ByTabWidget* main_tab_widget = new ByTabWidget(parent);
    main_tab_widget->setObjectName("main_tab_widget");
    main_tab_widget->addTab(createShortcutKeyWidget(parent), ":/public/default_logo",u8"¿ì½Ý¼ü");
    main_tab_widget->addTab(createNoteWidget(parent), ":/public/default_logo", u8"±ãÇ©");
    auto empty3 = new QWidget();
    //main_tab_widget->addTab(empty3, "", "333");

    return main_tab_widget;
}

QWidget* MainWindow::createShortcutKeyWidget(QWidget* parent)
{
    ShortcutKeyWidget* shortcut_key_widget = new ShortcutKeyWidget(parent, ShortcutKeyMgr.getItemList());
    shortcut_key_widget->setWhatsThis("shortcut_key_widget");
    return shortcut_key_widget;
}

QWidget* MainWindow::createNoteWidget(QWidget* parent)
{
    StickyNoteTabWidget* sticky_note_widget = new StickyNoteTabWidget(parent);
    sticky_note_widget->setWhatsThis("sticky_note_widget");
    return sticky_note_widget;
}

} // namespace TOOLBOX