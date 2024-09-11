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
	// 将当前颜色值保存到全局数据对象中
	KGlobalData::getGlobalDataIntance()->setLineColor(str);


	// 设置按钮背景颜色为当前选择的颜色 (颜色选择按钮)
	m_pColorBtn->setStyleSheet(QString("border-width: 0px; background-color:%1; border-style:outset;margin:5px").arg(str));

	emit pickedColor(); // 发出信号	
}