#ifndef __K_PARAM_BOX_H_
#define __K_PARAM_BOX_H_

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

#include "kglobaldata.h"

class KParamBox : public QWidget
{
	Q_OBJECT

public:
	KParamBox(const QString &title,QWidget *parent);
	~KParamBox();
	virtual void enterEvent(QEvent* event) override;
	virtual void leaveEvent(QEvent* event) override;
	virtual void onHandleSlected() { return; } // select shape÷Æ∫Û¥•∑¢
protected:
	QVBoxLayout* m_pVLayout;
private:
	QString m_title;
	QLabel* m_pTitleLabel;

};

#endif
