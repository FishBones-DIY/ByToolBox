#ifndef ROTATEWIDGET_H
#define ROTATEWIDGET_H

#include <QPixmap>
#include <QString>
#include <QWidget>
#include <vector>
#include <tuple>

/**
* ͼƬ��ת�ؼ�
* ֧�ֵ��Ӷ�����ת��ÿ����ӿ��Ե�������
* ���ӱ���ͼƬ���ýӿڣ�stop״̬��չʾ����ͼƬ

* �����ϵ�����˳��������ӵ�����ײ�
* ͼƬ����Ĭ����ؼ������غϣ������ĵ������ת���ݲ�֧�ֱ������λ��
*/


class CRotateWidgetBase : public QWidget
{
    Q_OBJECT

public:
    CRotateWidgetBase(QWidget* parent = 0);
    virtual ~CRotateWidgetBase();

    int32_t AddPixmap(const QString&, float fAngleStep = 10);//���ͼƬ, fAngleStep���Ƕ�����������֧��С�����Լ�������
    int32_t AddPixmap(QPixmap, float fAngleStep = 10);
    bool    AddPixmapGroup(const QString&, int nCnt, float fa);
    bool    SetPixmap(int32_t, const QString&);             //�޸�ͼƬ
    bool    SetPixmapVisible(int32_t, bool bVisible, bool butOther = false);               //�޸���ʾ

    void    lockUpdate(bool);   //�Ż�ˢ��
    void    redraw();

    int32_t AddBkPixmap(const QString&, bool bVisible);     //����ͼƬ
    void    SetBkPixmapVisible(int32_t, bool bVisible, bool butOther = false);   //butOther��trueʱ������������Ϊfalse
    bool    AddBkPixmapGroup(const QString&, int nCnt);

    QSize   imageSize();

    void Play(int32_t sleepVal = 25);                       //��������
    void Pause() { m_bPausing = true; onRotateWidgetEndRun(); }                     //��ͣ
    void Resume() {
        m_bPausing = false; Play();
    }                   //����
    void Stop();                                            //ֹͣ
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
    int32_t m_nTimerId = 0;    //��¼��ʱ��id.
    bool    m_bLockUpdate = false;
    bool    m_bNeedRedraw = false;

    bool    m_bPausing = false; //�Ƿ�����ͣ״̬

    std::vector< std::tuple<QPixmap, float, float, bool>>   m_dataArr;  // image, angle, visible

    std::vector < std::tuple<QPixmap, bool>>            m_bkPixmapArr;
};



#endif // ROTATEWIDGET_H
