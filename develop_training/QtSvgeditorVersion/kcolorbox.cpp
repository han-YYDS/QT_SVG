#include "kcolorbox.h"


KColorBox::KColorBox(const QString &title,const QString &rgbColorStr,QWidget *parent)
	: KParamBox(title,parent)
	, m_pColorBtn(Q_NULLPTR)
{
	m_pColorBtn = new QPushButton(this);
	m_pColorBtn->setMaximumSize(70, 50);
	m_pColorBtn->setStyleSheet(QString("border-width: 0px; background-color:%1; border-style:outset;margin:5px").arg(rgbColorStr));
	m_pVLayout->addWidget(m_pColorBtn); // ��ӵ�������

	(void)connect(m_pColorBtn, &QPushButton::clicked, this, &KColorBox::showPickColorDlg);
}

KColorBox::~KColorBox()
{
}

void KColorBox::mouseReleaseEvent(QMouseEvent* event)
{
	m_pColorBtn->click();
}

void KColorBox::setColor(QString str)
{
	qDebug() << "KColorBox::setColor" << str;
	// ����ǰ��ɫֵ���浽ȫ�����ݶ�����
	KGlobalData::getGlobalDataIntance()->setCanvasColor(str);

	// ���ð�ť������ɫΪ��ǰѡ�����ɫ (��ɫѡ��ť)
	setButtonColor(str);


	emit pickedColor(); // �����ź�	
}

void KColorBox::setButtonColor(QString color)
{
	m_pColorBtn->setStyleSheet(QString("border-width: 0px; background-color:%1; border-style:outset;margin:5px").arg(color));
}

void KColorBox::showPickColorDlg()
{
	QString str = KGlobalData::getGlobalDataIntance()->getCanvasColor();
	qDebug() << "str = " << str;
	str = str.mid(1);
	qDebug() << "str = " <<  str;
	// QColorDialog : ϵͳʵ����ɫѡ���
	QColor color = QColorDialog::getColor(QColor(str.toUInt(NULL, 16)), nullptr, QStringLiteral("ѡ����ɫ"), QColorDialog::ShowAlphaChannel);

	if (!color.isValid())
		return;

	str = QString::number(color.rgb(), 16); // �� 16���Ƶ���ɫֵת���� str �ַ���
	qDebug() << "color " <<  str;
	setColor("#"+str);
}

