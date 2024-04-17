#ifndef ROTATEWIDGET_H
#define ROTATEWIDGET_H

#include <QPixmap>
#include <QString>
#include <QWidget>
#include <vector>
#include <tuple>

/**
* 图片旋转控件
* 支持叠加多重旋转，每层叠加可以单独配速
* 增加背景图片设置接口，stop状态才展示背景图片

* 层叠关系按添加顺序，最先添加的在最底层
* 图片中心默认与控件中心重合，以中心点进行旋转，暂不支持变更中心位置
*/


class CRotateWidgetBase : public QWidget
{
    Q_OBJECT

public:
    CRotateWidgetBase(QWidget* parent = 0);
    virtual ~CRotateWidgetBase();

    int32_t AddPixmap(const QString&, float fAngleStep = 10);//添加图片, fAngleStep：角度增量，精度支持小数，以及负方向
    int32_t AddPixmap(QPixmap, float fAngleStep = 10);
    bool    AddPixmapGroup(const QString&, int nCnt, float fa);
    bool    SetPixmap(int32_t, const QString&);             //修改图片
    bool    SetPixmapVisible(int32_t, bool bVisible, bool butOther = false);               //修改显示

    void    lockUpdate(bool);   //优化刷新
    void    redraw();

    int32_t AddBkPixmap(const QString&, bool bVisible);     //背景图片
    void    SetBkPixmapVisible(int32_t, bool bVisible, bool butOther = false);   //butOther传true时，将其他设置为false
    bool    AddBkPixmapGroup(const QString&, int nCnt);

    QSize   imageSize();

    void Play(int32_t sleepVal = 25);                       //启动动画
    void Pause() { m_bPausing = true; onRotateWidgetEndRun(); }                     //暂停
    void Resume() {
        m_bPausing = false; Play();
    }                   //继续
    void Stop();                                            //停止
    bool IsPlaying() {
        return m_nTimerId > 0;
    }
    void resetAngle(int32_t);
protected:
    virtual void onRotateWidgetStartRun(){}
    virtual void onRotateWidgetEndRun() {}
    virtual void onRotateWidgetRunning() {}

protected:
    void timerEvent(QTimerEvent* event) override;
    void paintEvent(QPaintEvent*) override;
    void update() {
        QWidget::update();
    }
protected:
    int32_t m_nTimerId = 0;    //记录定时器id.
    bool    m_bLockUpdate = false;
    bool    m_bNeedRedraw = false;

    bool    m_bPausing = false; //是否处于暂停状态

    std::vector< std::tuple<QPixmap, float, float, bool>>   m_dataArr;  // image, angle, visible

    std::vector < std::tuple<QPixmap, bool>>            m_bkPixmapArr;
};



#endif // ROTATEWIDGET_H
