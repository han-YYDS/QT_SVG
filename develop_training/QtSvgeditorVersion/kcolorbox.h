#ifndef __K_PARAMBOX_H_
#define __K_PARAMBOX_H_

#include <QPushButton>
#include <QColorDialog>

#include "kparambox.h"

class KColorBox : public KParamBox
{
	Q_OBJECT

public:
	KColorBox(const QString &title,const QString &rgbColorStr, QWidget *parent);
	~KColorBox();

	// ����ͷ��¼�,����ͷ�ʱ������ɫѡ��Ի���
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	virtual void setColor(QString color);
	void setButtonColor(QString color);
signals:
	void pickedColor();
private slots:
	void showPickColorDlg();
protected:
	QPushButton* m_pColorBtn;
};

#endif
