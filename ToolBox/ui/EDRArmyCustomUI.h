#pragma once
#include <QWidget>


class EDRArmyCustomUI : public QWidget
{
	Q_OBJECT
public:
	EDRArmyCustomUI(QWidget* parent = Q_NULLPTR);
protected:
	virtual void paintEvent(QPaintEvent* e) override;
private:
	void setBackgroungImage();
};

