#include "EDRArmyCustomUI.h"

#include <QPainter>

EDRArmyCustomUI::EDRArmyCustomUI(QWidget* parent):QWidget(parent)
{
    //setBackgroungImage();
}

void EDRArmyCustomUI::setBackgroungImage()
{
    this->setAutoFillBackground(true); // 这句要加上, 否则可能显示不出背景图.
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window,
        QBrush(QPixmap(":/public/default_logo").scaled(// 缩放背景图.
            this->size(),
            Qt::IgnoreAspectRatio,
            Qt::SmoothTransformation)));             // 使用平滑的缩放方式
    this->setPalette(palette);                           // 给widget加上背景图
}

void EDRArmyCustomUI::paintEvent(QPaintEvent* e)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/public/default_logo"), QRect());

}