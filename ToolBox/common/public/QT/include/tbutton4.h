#ifndef BUTTON4_H
#define BUTTON4_H

#include <QObject>
#include <QWidget>
#include <QPaintEvent>
#include <QEvent>
#include <QPushButton>
#include <QString>
#include <QPainter>
#include <QDebug>
#include <QPainter>

class TButton4 : public QPushButton //用于四态图片
{
public:
    TButton4(QString pix_listurl, QWidget* parent = nullptr) : QPushButton(parent)
    {
        m_index = 0;
        m_enter = false;
        QPixmap pix(pix_listurl);
        m_nHeight = pix.height();
        m_nWidth = pix.width() / 4;
        for (int i = 0; i < 4; i++) {
            m_pixlist << pix.copy(i * m_nWidth, 0, m_nWidth, m_nHeight);
        }
    }

    QSize sizeHint() const
    {
        return QSize(m_nWidth, m_nHeight);
    }

protected:
    void paintEvent(QPaintEvent*) override
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);
        painter.drawPixmap((width() - m_pixlist.at(m_index).width()) / 2, (height() - m_pixlist.at(m_index).height()) / 2
            , m_pixlist.at(m_index).width()
            , m_pixlist.at(m_index).height(), m_pixlist.at(m_index));//画图画到中间
    }

    void enterEvent(QEvent*) override
    {
        m_index = 1;
        m_enter = true;
        update();
    }

    void leaveEvent(QEvent*) override
    {
        m_index = 0;
        m_enter = false;
        update();
    }

    void mousePressEvent(QMouseEvent* e) override
    {
        if (e->button() == Qt::LeftButton)//如果是左键按下
        {
            m_index = 2;
            update();
            QPushButton::mousePressEvent(e);//返回到上级
        }
    }

    void mouseReleaseEvent(QMouseEvent* e) override
    {
        if (e->button() == Qt::LeftButton)//如果是左键放下
        {
            m_index = 1;
            update();
            QPushButton::mouseReleaseEvent(e);//返回到上级 引出按钮clicked信号
        }
    }
private:
    QList<QPixmap> m_pixlist;
    int m_index;
    bool m_enter;

    int m_nWidth;
    int m_nHeight;
};
#endif // BUTTON4_H
