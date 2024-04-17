#include "EDRArmyCustomUI.h"

#include <QPainter>

EDRArmyCustomUI::EDRArmyCustomUI(QWidget* parent):QWidget(parent)
{
    //setBackgroungImage();
}

void EDRArmyCustomUI::setBackgroungImage()
{
    this->setAutoFillBackground(true); // ���Ҫ����, ���������ʾ��������ͼ.
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window,
        QBrush(QPixmap(":/public/default_logo").scaled(// ���ű���ͼ.
            this->size(),
            Qt::IgnoreAspectRatio,
            Qt::SmoothTransformation)));             // ʹ��ƽ�������ŷ�ʽ
    this->setPalette(palette);                           // ��widget���ϱ���ͼ
}

void EDRArmyCustomUI::paintEvent(QPaintEvent* e)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/public/default_logo"), QRect());

}