#include "kshapelinecolorbox.h"


KShapeLineColorBox::KShapeLineColorBox(const QString& title, const QString& rgbColorStr, QWidget* parent)
	: KColorBox(title, rgbColorStr, parent)
{
}

KShapeLineColorBox::~KShapeLineColorBox()
{
}

void KShapeLineColorBox::setColor(QString str)
{
	qDebug() << "KShapeColorBox::setColor" << str;
	// ����ǰ��ɫֵ���浽ȫ�����ݶ�����
	KGlobalData::getGlobalDataIntance()->setLineColor(str);


	// ���ð�ť������ɫΪ��ǰѡ�����ɫ (��ɫѡ��ť)
	m_pColorBtn->setStyleSheet(QString("border-width: 0px; background-color:%1; border-style:outset;margin:5px").arg(str));

	emit pickedColor(); // �����ź�	
}