#include "khexa.h"
#include "kglobaldata.h"
#include <QPainter>

KHexa::KHexa(QObject *parent)
	: KShape(parent)
{
}

KHexa::~KHexa()
{
}

void KHexa::drawShape(QPaintDevice* parent)
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

	painter.drawPolygon(getPoints(), 6);
}

void KHexa::output()
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

	painter->drawPolygon(getPoints(), 6);

}


KShapeType KHexa::getShapeType()
{
	return KShapeType::HexaShapeType;
}

QPoint* KHexa::getPoints()
{
	int x1 = getStartPointInCanvas().x();
	int y1 = getStartPointInCanvas().y();
	int x2 = getEndPointInCanvas().x();
	int y2 = getEndPointInCanvas().y();

	// 计算六边形的六个顶点
	QPoint* points = new QPoint[6];
	points[0] = QPoint(x1, 0.5*(y1 + y2));
	points[1] = QPoint(0.75 * x1 + 0.25 * x2, y1);
	points[2] = QPoint(0.25 * x1 + 0.75 * x2, y1);
	points[3] = QPoint(x2, 0.5 * (y1 + y2));
	points[4] = QPoint(0.25 * x1 + 0.75 * x2, y2);
	points[5] = QPoint(0.75 * x1 + 0.25 * x2, y2);

#if 0
	qDebug() << "[DEBUG] >>>>> ";
	for (int i = 0; i < 6; i++)
	{
		qDebug() << "[DEBUG] paint hexa: point i : " << dumpPoint(points[i]);
	}
	qDebug() << "[DEBUG] <<<<< \n";
#endif

	return points;
}

