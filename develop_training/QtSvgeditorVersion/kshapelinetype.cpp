#include "kshapelinetype.h"


KShapeLineTypeBox::KShapeLineTypeBox(const QString& title, QWidget* parent)
	: KComboBox(title, parent)
{
	m_pComboBox->addItem("solid");
	m_pComboBox->addItem("dot");
}

KShapeLineTypeBox::~KShapeLineTypeBox()
{
}

void KShapeLineTypeBox::setItem(int index)
{
	Qt::PenStyle style = Qt::NoPen;
	switch (index)
	{
	case 0:
		style = Qt::SolidLine;
		break;
	case 1:
		style = Qt::DashLine;
		break;
	case 2:
		style = Qt::DotLine;
		break;
	default:
		qDebug() << "[ERROR] setItem ";
		break;
	}
	KGlobalData::getGlobalDataIntance()->setShapeLineType(style);
	emit pickedItem(); // ·¢³öÐÅºÅ	
}


