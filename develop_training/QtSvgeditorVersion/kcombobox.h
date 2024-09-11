#ifndef __K_COMBO_BOX_H_
#define __K_COMBO_BOX_H_

#include <QComboBox>

#include "kparambox.h"


class KComboBox : public KParamBox
{
	Q_OBJECT

		friend class KMainWindow;
public:
	KComboBox(const QString& title, QWidget* parent);
	~KComboBox();
	virtual void setItem(int index);
signals:
	void pickedItem();
public slots:
	void KComboBox::onComboBoxIndexChanged(int index);
protected:
	QComboBox* m_pComboBox;
	
};

#endif
