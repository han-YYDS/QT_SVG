#include "kpent.h"
#include "kglobaldata.h"
#include <QPainter>

KPent::KPent(QObject *parent)
	: KShape(parent)
{
}

KPent::~KPent()
{
}

void KPent::drawShape(QPaintDevice* parent)
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
	painter.drawPolygon(getPoints(), 5);
}

void KPent::output()
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

	painter->drawPolygon(getPoints(), 5);
	
}

KShapeType KPent::getShapeType()
{
	return KShapeType::RectShapeType;
}

QPoint* KPent::getPoints()
{
	int x1 = getStartPointInCanvas().x();
	int y1 = getStartPointInCanvas().y();
	int x2 = getEndPointInCanvas().x();
	int y2 = getEndPointInCanvas().y();

	// 计算五边形的五个顶点
	QPoint* points = new QPoint[5];
	points[0] = QPoint(0.5 * x1 + 0.5 * x2 , y1);
	points[1] = QPoint(x2, 0.6 * y1 + 0.4 * y2);
	points[2] = QPoint(0.2 * x1 + 0.8 * x2, y2);
	points[3] = QPoint(0.8 * x1 + 0.2 * x2, y2);
	points[4] = QPoint(x1, 0.6 * y1 + 0.4 * y2);
	return points;
}


