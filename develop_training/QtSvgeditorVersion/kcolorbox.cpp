#include "kcolorbox.h"


KColorBox::KColorBox(const QString &title,const QString &rgbColorStr,QWidget *parent)
	: KParamBox(title,parent)
	, m_pColorBtn(Q_NULLPTR)
{
	m_pColorBtn = new QPushButton(this);
	m_pColorBtn->setMaximumSize(70, 50);
	m_pColorBtn->setStyleSheet(QString("border-width: 0px; background-color:%1; border-style:outset;margin:5px").arg(rgbColorStr));
	m_pVLayout->addWidget(m_pColorBtn); // 添加到布局器

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
	// 将当前颜色值保存到全局数据对象中
	KGlobalData::getGlobalDataIntance()->setCanvasColor(str);

	// 设置按钮背景颜色为当前选择的颜色 (颜色选择按钮)
	setButtonColor(str);


	emit pickedColor(); // 发出信号	
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
	// QColorDialog : 系统实现颜色选择框
	QColor color = QColorDialog::getColor(QColor(str.toUInt(NULL, 16)), nullptr, QStringLiteral("选择颜色"), QColorDialog::ShowAlphaChannel);

	if (!color.isValid())
		return;

	str = QString::number(color.rgb(), 16); // 将 16进制的颜色值转换成 str 字符串
	qDebug() << "color " <<  str;
	setColor("#"+str);
}

