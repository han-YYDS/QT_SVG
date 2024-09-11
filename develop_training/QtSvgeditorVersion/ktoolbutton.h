#ifndef __KTOOLBUTTON_H_
#define __KTOOLBUTTON_H_

#include <QRadioButton>
#include <QtSvg/QSvgRenderer>
#include <QPainter>

#include "kglobaldata.h"

class KToolButton : public QRadioButton
{
	Q_OBJECT
public:
	KToolButton(KGlobalData::KDrawFlag drawflag,QWidget *parent);
	~KToolButton();

	void initToolButton();

	virtual void paintEvent(QPaintEvent* event) override;// ��ͼ�¼�
	virtual void enterEvent(QEvent* event) override; // �����������
	virtual void leaveEvent(QEvent* event) override; // �뿪�������
	virtual void mouseReleaseEvent(QMouseEvent* e) override;// ����ͷ��¼�


private slots:
	void onToggledButton(bool checked);
private:
	QString m_iconName;//svg ͼ������
	QSvgRenderer* m_pSvgRenderer;//���� svg ͼ��/ͼƬ�Ķ���ָ��
	KGlobalData::KDrawFlag m_drawFlag;//��¼��ͼ����
};

#endif
