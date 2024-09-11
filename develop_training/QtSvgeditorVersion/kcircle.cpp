#include "kcircle.h"
#include "kcalculate.h"
#include "kglobaldata.h"
#include <QPainter>

KCircle::KCircle(QObject* parent)
	: KShape(parent)
{
}

KCircle::~KCircle()
{
}


void KCircle::drawShape(QPaintDevice* parent)
{
	QPen pen;
	pen.setWidth(m_shapeLineWidth);
	pen.setColor(m_shapeLineColor);
	QPainter painter(parent);
	pen.setStyle(m_shapeLineType);
	painter.setPen(pen);
	QColor fillColor(m_shapeColor);
	QBrush brush(fillColor);
	painter.setBrush(brush);

	painter.drawEllipse(QRect(getStartPointInCanvas(), getEndPointInCanvas()));
}

void KCircle::output()
{
	QPainter* painter = KGlobalData::getGlobalDataIntance()->getPainter();
	QPen pen;
	pen.setWidth(m_shapeLineWidth);
	pen.setColor(m_shapeLineColor);
	pen.setStyle(m_shapeLineType);
	painter->setPen(pen);
	QColor fillColor(m_shapeColor);
	QBrush brush(fillColor);
	painter->setBrush(brush);

	painter->drawEllipse(QRect(getStartPointInCanvas(), getEndPointInCanvas()));
}

KShapeType KCircle::getShapeType()
{
	return KShapeType::RectShapeType;
}
