#include "kshape.h"

#include "kglobaldata.h"

KShape::KShape(QObject *parent)
	: QObject(parent)
{
	m_shapeColor = KGlobalData::getGlobalDataIntance()->getShapeColor();
	m_shapeLineColor = KGlobalData::getGlobalDataIntance()->getLineColor();
	m_shapeLineWidth = KGlobalData::getGlobalDataIntance()->getLineWidth();
	m_shapeLineType = KGlobalData::getGlobalDataIntance()->getShapeLineType();
}

KShape::KShape(const KShape& other)
{
	// 属性
	m_shapeColor = other.m_shapeColor;
	m_shapeLineColor = other.m_shapeLineColor;
	m_shapeLineWidth = other.m_shapeLineWidth;
	m_shapeLineType = other.m_shapeLineType;


	// 起终点
	m_startPoint = other.m_startPoint;
	m_endPoint = other.m_endPoint;
}

KShape::~KShape()
{
}

void KShape::move(QPoint offset)
{
	m_startPoint += offset; 
	m_endPoint += offset;
}

void KShape::moveTop(QPoint pos)
{
	m_startPoint.ry() = pos.y();
}

void KShape::moveBottom(QPoint pos)
{
	m_endPoint.ry() = pos.y();
}

void KShape::moveLeft(QPoint pos)
{
	m_startPoint.rx() = pos.x();
}

void KShape::moveRight(QPoint pos)
{
	m_endPoint.rx() = pos.x();
}

void KShape::moveTopLeft(QPoint pos)
{
	m_startPoint = pos;
}

void KShape::moveTopRight(QPoint pos)
{
	m_startPoint.ry() = pos.y();
	m_endPoint.rx() = pos.x();
}

void KShape::moveBottomLeft(QPoint pos)
{
	m_startPoint.rx() = pos.x();
	m_endPoint.ry() = pos.y();
}

void KShape::moveBottomRight(QPoint pos)
{
	m_endPoint = pos;
}

// 绘制选中矩形框
void KShape::drawOutLine(QPaintDevice* parent)
{
	QPainter painter(parent);
	QPen pen;

	pen.setColor(QColor(79, 128, 255));
	pen.setWidth(3);
	painter.setPen(pen);

	QPoint start = getStartPoint();
	QPoint end = getEndPoint();

	painter.drawRect(QRect(getStartPointInCanvas(), getEndPointInCanvas()));
}

QPoint KShape::getStartPoint()
{
	return m_startPoint;
}

QPoint KShape::getEndPoint()
{
	return m_endPoint;
}

QPoint KShape::getStartPointInCanvas() const
{
	qreal scale = KGlobalData::getGlobalDataIntance()->getCanvasScale();
	return QPoint(m_startPoint.x() * scale, m_startPoint.y() * scale);
}

QPoint KShape::getEndPointInCanvas() const
{
	qreal scale = KGlobalData::getGlobalDataIntance()->getCanvasScale();
	return QPoint(m_endPoint.x() * scale, m_endPoint.y() * scale);
}

void KShape::setStartPoint(const QPoint & point) // 物理坐标
{
	m_physical_start = point; 
	qreal scale = KGlobalData::getGlobalDataIntance()->getCanvasScale();
	m_startPoint = QPoint(point.x() / scale, point.y() / scale);
}

void KShape::setEndPoint(const QPoint& point)
{
	m_physical_end = point; // 物理坐标
	qreal scale = KGlobalData::getGlobalDataIntance()->getCanvasScale();
	m_endPoint = QPoint(point.x() / scale, point.y() / scale);
}

KShapeType KShape::getShapeType()
{
	return KShapeType();
}

QRect KShape::getShapeRect() const // 物理坐标
{
	return QRect(getStartPointInCanvas(),getEndPointInCanvas());
}


bool KShape::isValid()
{
	return (m_startPoint.x() != m_endPoint.x() || m_startPoint.y() != m_endPoint.y());
}

