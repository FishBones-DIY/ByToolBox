#pragma once
#pragma execution_character_set("utf-8")
#include "CRotateWidgetBase.h"
#include <QTimer>
#include <QEvent>
#include <QPainter>
#include <QStyleOption>
//#include <Public\Interface\platform\win32\win32_macro_type.h>
using namespace std;
/*QAXASSERT（“ss”）可以包含一些信息*/
CRotateWidgetBase::CRotateWidgetBase(QWidget* parent)
    : QWidget(parent)
{
    m_nTimerId = 0;
}

CRotateWidgetBase::~CRotateWidgetBase()
{
    Stop();
}

void CRotateWidgetBase::resetAngle(int32_t idx) {
    if (m_dataArr.size() > idx)
    {
        std::get<2>(m_dataArr[idx]) = 0;
    }
}

int CRotateWidgetBase::AddPixmap(const QString& pixmapfile, float fa)
{
    m_dataArr.push_back(std::make_tuple(QPixmap(pixmapfile), fa, 0, true));
    return m_dataArr.size() - 1;
}

int32_t CRotateWidgetBase::AddPixmap(QPixmap pixmap, float fa)
{
    m_dataArr.push_back(std::make_tuple(pixmap, fa, 0, true));
    return m_dataArr.size() - 1;
}

bool CRotateWidgetBase::AddPixmapGroup(const QString& imgGroup, int nCnt, float fa) {
    QPixmap pm(imgGroup);
    if (pm.isNull())
    {
        return false;
    }
    auto szpm = pm.size();
    auto onecx = szpm.width() / nCnt;
    if (szpm.width() % nCnt != 0)
    {
       // QAXASSERT(L"图像尺寸不可被个数整除");   //图像尺寸要能被个数整除
        return false;
    }

    QRect rc(0, 0, onecx, szpm.height());
    for (auto i = 0; i < nCnt; i++)
    {
        rc.moveLeft(i * onecx);
        QPixmap _pm = pm.copy(rc);
        auto st = _pm.size();
        m_dataArr.push_back(std::make_tuple(_pm, fa, 0, false));
    }
    return true;
}

bool CRotateWidgetBase::SetPixmap(int32_t idx, const QString& pixmapfile)
{
    if (idx >= m_dataArr.size())
    {
        return false;
    }
    auto& item = m_dataArr[idx];
    std::get<0>(item) = QPixmap(pixmapfile);
    return !std::get<0>(item).isNull();
}

bool CRotateWidgetBase::SetPixmapVisible(int32_t idx, bool bVisible, bool butOther)
{
    if (idx >= m_dataArr.size())
    {
        return false;
    }

    if (bVisible)
    {
        if (butOther)
        {
            for (auto& it : m_dataArr)
            {
                std::get<3>(it) = false;
            }
        }
    }
    auto& item = m_dataArr[idx];
    std::get<3>(item) = bVisible;
    return true;
}

int32_t CRotateWidgetBase::AddBkPixmap(const QString& pm, bool bVisible)
{
    m_bkPixmapArr.push_back(std::make_tuple(QPixmap(pm), bVisible));
    return m_bkPixmapArr.size() - 1;
}

bool CRotateWidgetBase::AddBkPixmapGroup(const QString& imgGroup, int nCnt)
{
    QPixmap pm(imgGroup);
    if (pm.isNull())
    {
        return false;
    }
    auto szpm = pm.size();
    auto onecx = szpm.width() / nCnt;
    if (szpm.width() % nCnt != 0)
    {
       // QAXASSERT(L"图像尺寸不可被个数整除");   //图像尺寸要能被个数整除
        return false;
    }

    QRect rc(0, 0, onecx, szpm.height());
    for (auto i = 0; i < nCnt; i++)
    {
        rc.moveLeft(i * onecx);
        QPixmap _pm = pm.copy(rc);
        auto st = _pm.size();
        m_bkPixmapArr.push_back(std::make_tuple(_pm, false));
    }
    return true;
}

QSize CRotateWidgetBase::imageSize()
{
    if (m_bkPixmapArr.size())
    {
       return std::get<0>(m_bkPixmapArr.at(0)).size();
    }
    return QSize();
}

void CRotateWidgetBase::lockUpdate(bool b) {
    if (m_bLockUpdate != b)
    {
        m_bLockUpdate = b;
        if (!b)
        {
            if (m_bNeedRedraw)
            {
                m_bNeedRedraw = false;
                redraw();
            }
        }
    } 
}

void CRotateWidgetBase::redraw() {
    if (m_bLockUpdate)
    {
        m_bNeedRedraw = true;
        return;
    }
    update();
}

void CRotateWidgetBase::SetBkPixmapVisible(int32_t idx, bool bVisible, bool butOther)
{
    if (idx >= m_bkPixmapArr.size())
    {
        return ;
    }

    if (bVisible)
    {
        if (butOther)
        {
            for (auto& it : m_bkPixmapArr)
            {
                std::get<1>(it) = false;
            }
        }
    }

    auto& item = m_bkPixmapArr[idx];
    std::get<1>(item) = bVisible;

    redraw();
}


void CRotateWidgetBase::Play(int32_t sleepVal)
{
    if (m_nTimerId == 0)
    {
        m_nTimerId = this->startTimer(sleepVal, Qt::PreciseTimer);
        m_bPausing = false;

        onRotateWidgetStartRun();
    }
}

void CRotateWidgetBase::Stop()
{
    if (m_nTimerId > 0)
    {
        this->killTimer(m_nTimerId);
        m_nTimerId = 0;
        m_bPausing = false;

        onRotateWidgetEndRun();
    }
}

void CRotateWidgetBase::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == m_nTimerId && !m_bPausing)
    {
        for (auto & iter : m_dataArr)
        {
            auto& val = std::get<2>(iter);

            val += std::get<1>(iter);   //累加角度

            if (val >= 360 ||
                val <= -360)
            {
                val = (int)val % 360;
            }
        }

        onRotateWidgetRunning();

        update();
    }
}

void CRotateWidgetBase::paintEvent(QPaintEvent* pe)
{
    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
    painter.setRenderHints(QPainter::HighQualityAntialiasing | QPainter::SmoothPixmapTransform);    //抗锯齿

    QWidget::paintEvent(pe);

    auto centerPt = rect().center();
  //  if (!IsPlaying())
    {
        for (auto & iter : m_bkPixmapArr)
        {
            if (std::get<1>(iter))  //是否可见
            {
                const auto& pix = std::get<0>(iter);
                if (!pix.isNull())
                {
                    auto szImg = pix.size();
                    painter.drawPixmap(centerPt.x() - szImg.width() / 2, centerPt.y() - szImg.height() / 2, szImg.width(), szImg.height(), pix);
                }
            }         
        } 
    }

    painter.save();
    
    
    for (auto iter : m_dataArr)
    {
        if (!std::get<3>(iter))
        {
            continue;
        }
        const auto& fAgl = std::get<2>(iter);
        QPixmap & pm = std::get<0>(iter);

        QPoint centerImage(pm.width() / 2, pm.height() / 2);
        painter.save();
        painter.translate(centerImage);          //让图片的中心作为旋转的中心
        painter.rotate(fAgl);     
        painter.translate(-centerPt.x(), -centerPt.y());
        //painter.drawPixmap(centerPt.x() - pm.width() / 2, centerPt.y() - pm.height() / 2, pm.width(), pm.height(), pm);  
        painter.drawPixmap(centerPt.x() - pm.width() / 2, centerPt.y() - pm.height() / 2, pm.width(), pm.height(), pm);
        painter.restore();
    }

    painter.restore();
}