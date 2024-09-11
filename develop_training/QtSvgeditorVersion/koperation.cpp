#include "koperation.h"
#include "kshape.h"
#include "kglobaldata.h"
#include "kcanvas.h"
#include "kcanvasparamsbar.h"
#include <QObject>

// add shape
OpAdd::OpAdd(KShape* shape)
	: m_pShape(shape)	
{

}

Operation* OpAdd::reverse()
{
	// 将还原的shape删除
	return new OpDel(m_pShape);
}

void OpAdd::exec()
{
	// 1. 将delete的shape还原

	KShape* shape = KGlobalData::getGlobalDataIntance()->getCanvas()->deletePop();
	KGlobalData::getGlobalDataIntance()->getCanvas()->ShapeListPrepend(shape);
}

OpDel::OpDel(KShape* shape)
	: m_pShape(shape)
{
}

Operation* OpDel::reverse()
{
	return new OpAdd(m_pShape);
}

void OpDel::exec()
{
	// 1. 从shapelist中将shape删除, 加至deletestack中
	KGlobalData::getGlobalDataIntance()->getCanvas()->removeFromShapeList(m_pShape);
	KGlobalData::getGlobalDataIntance()->getCanvas()->deletePush(m_pShape);
}

OpShapePara::OpShapePara(KShape* shape, KShapeParaType type, QVariant oldval, QVariant newval)
	:m_pShape(shape)
	,m_type(type)
	,m_old(oldval)
	,m_new(newval)
{
}

Operation* OpShapePara::reverse()
{
	return new OpShapePara(m_pShape, m_type, m_new, m_old);
}

void OpShapePara::exec()
{
	switch (m_type)
	{
	case KShapeParaType::LineColor:
		m_pShape->setShapeLineColor(m_new.toString());
		break;
	case KShapeParaType::ShapeColor:
		m_pShape->setShapeColor(m_new.toString());
		break;
	case KShapeParaType::LineWidth:
		m_pShape->setShapeLineWidth(m_new.toInt());
		break;
	case KShapeParaType::LineType:
		m_pShape->setShapeLineType(m_new.value<Qt::PenStyle>());
		break;
	default:
		break;
	}
	
	qDebug() << "[DEBUG] opShapePara execute " << m_new.toString();
	return;
}

OpCanvasPara::OpCanvasPara(KCanvasParaType type, QVariant oldval, QVariant newval)
	:m_type(type)
	,m_old(oldval)
	,m_new(newval)
{
}

Operation* OpCanvasPara::reverse()
{
	return new OpCanvasPara(m_type, m_new, m_old);
}

void OpCanvasPara::exec()
{
	auto canvas = KGlobalData::getGlobalDataIntance()->getCanvas();
	auto bar = KGlobalData::getGlobalDataIntance()->getCanvasParamBar();
	switch (m_type)
	{
	case KCanvasParaType::Width:
		bar->m_pWidthBox->blockSignals(true);
		KGlobalData::getGlobalDataIntance()->setCanvasWidth(m_new.toInt());
		bar->m_pWidthBox->setButtonValue(m_new.toInt());
		bar->m_pWidthBox->blockSignals(false);
		break;
	case KCanvasParaType::Height:
		bar->m_pHeightBox->blockSignals(true);
		KGlobalData::getGlobalDataIntance()->setCanvasHeight(m_new.toInt());
		bar->m_pHeightBox->setButtonValue(m_new.toInt());
		bar->m_pHeightBox->blockSignals(false);
		break;
	case KCanvasParaType::Color:
		bar->m_pColorBox->blockSignals(true);
		KGlobalData::getGlobalDataIntance()->setCanvasColor(m_new.toString());
		bar->m_pColorBox->setButtonColor(m_new.toString());
		bar->m_pColorBox->blockSignals(false);
		break;
	default:
		break;
	}
	
	qint32 width = KGlobalData::getGlobalDataIntance()->getCanvasWidth();
	qint32 height = KGlobalData::getGlobalDataIntance()->getCanvasHeight();
	QString color = KGlobalData::getGlobalDataIntance()->getCanvasColor();
	this->dump();
	qDebug() << "[DEBUG]  width = " << width << " height = " << height << " color = " << color;
	
	canvas->resize(width, height);
	canvas->setStyleSheet(QString("background-color:%1").arg(color));

}
