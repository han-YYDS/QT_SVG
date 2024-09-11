#include "kline.h"
#include <QPainter>

KLine::KLine(QObject* parent)
	: KShape(parent)
{
}

KLine::~KLine()
{
}

void KLine::drawShape(QPaintDevice* parent)
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

	painter.drawLine(getStartPointInCanvas(), getEndPointInCanvas());
}

KShapeType KLine::getShapeType()
{
	return KShapeType::RectShapeType;
}
