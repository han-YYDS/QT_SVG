#ifndef __K_VALUE_BOX_H_
#define __K_VALUE_BOX_H_

#include <QLineEdit>

#include "kparambox.h"


class KValueBox : public KParamBox
{
	Q_OBJECT

	friend class KMainWindow;
public:
	KValueBox(const QString &title,const int value,QWidget *parent);
	~KValueBox();
	virtual void focusInEvent(QFocusEvent* event) override;
	void setButtonValue(int val) { m_pParamEdit->setText(QString::number(val)); }
protected:
	QLineEdit *m_pParamEdit;
};

#endif
