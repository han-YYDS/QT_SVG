#include "kstar.h"
#include "kglobaldata.h"
#include <QPainter>

KStar::KStar(QObject *parent)
	: KShape(parent)
{
}

KStar::~KStar()
{
}

void KStar::drawShape(QPaintDevice* parent)
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

	painter.drawPolygon(getPoints(), 10);
}

void KStar::output()
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

	painter->drawPolygon(getPoints(), 10);

}


KShapeType KStar::getShapeType()
{
	return KShapeType::RectShapeType;
}

QPoint* KStar::getPoints()
{
	int x1 = getStartPointInCanvas().x();
	int y1 = getStartPointInCanvas().y();
	int x2 = getEndPointInCanvas().x();
	int y2 = getEndPointInCanvas().y();

	// 五角星的十个点
	QPoint* points = new QPoint[10];
	points[0] = QPoint(0.5 * x1 + 0.5 * x2, y1);
	points[1] = QPoint(0.4 * x1 + 0.6 * x2, 0.6 * y1 + 0.4 * y2);
	points[2] = QPoint(x2, 0.6 * y1 + 0.4 * y2);
	points[3] = QPoint(0.34 * x1 + 0.66 * x2, 0.34 * y1 + 0.66 * y2);
	points[4] = QPoint(0.25 * x1 + 0.75 * x2, y2);
	points[5] = QPoint(0.5 * x1 + 0.5 * x2, 0.2 * y1 + 0.8 * y2);
	points[6] = QPoint(0.75 * x1 + 0.25 * x2, y2);
	points[7] = QPoint(0.66 * x1 + 0.34 * x2, 0.34 * y1 + 0.66 * y2);
	points[8] = QPoint(x1, 0.6 * y1 + 0.4 * y2);
	points[9] = QPoint(0.6 * x1 + 0.4 * x2, 0.6 * y1 + 0.4 * y2);
	return points;
}