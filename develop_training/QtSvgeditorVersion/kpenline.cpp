#include "kpenline.h"
#include "kglobaldata.h"
#include <QPainter>

inline qreal getScale()
{
	return KGlobalData::getGlobalDataIntance()->getCanvasScale();
}

KPenLine::KPenLine(QObject* parent)
	: KShape(parent)
{
}

KPenLine::~KPenLine()
{
}

void KPenLine::drawShape(QPaintDevice* parent) // �����path�����Լ���m_path�ϸ���physical_start,pythiscal_end���ɵ�
{	
	// ���ֵ�ʱ����Ҫ��������
	m_physical_start = getStartPointInCanvas();
	m_physical_end = getEndPointInCanvas();

	// ���������drag��ʱ��, ������Ҫ������߼��ϲ���Ӧ���� 

	QPainter painter(parent);
	QPen pen;
	pen.setWidth(m_shapeLineWidth);
	pen.setColor(QColor(m_shapeColor));
	painter.setPen(pen);

	
	

	// 1. �߼����� m_startPoint, m_endPoint
	// 2. �߼�path
	// 3. scale
	// �������ϲ���������µ�path

	

	QRectF logical_rect(m_startPoint, m_endPoint); // �߼�����
	QRectF physical_rect(m_physical_start, m_physical_end); // ��������
	qDebug() << "[DEBUG] >>>>  scale is" << KGlobalData::getGlobalDataIntance()->getCanvasScale();
	qDebug() << "	logical start  [" << m_startPoint.x() << "," << m_startPoint.y() << "]   [" << m_endPoint.x() << "," << m_endPoint.y() << "]";
	qDebug() << "	physical start [" << m_physical_start.x() << "," << m_physical_start.y() << "]  [" << m_physical_end.x() << "," << m_physical_end.y() << "]";
	qDebug() << "[DEBUG] <<<< ";
	QTransform transform;

	// �����transelateӦ���Ǿ��ε����Ķ���������
	int logical_x = (m_startPoint.x() + m_endPoint.x()) / 2;
	int logical_y = (m_startPoint.y() + m_endPoint.y()) / 2;
	int physical_x = (m_physical_start.x() + m_physical_end.x()) / 2;
	int physical_y = (m_physical_start.y() + m_physical_end.y()) / 2;
	//  transform.translate(physical_x - logical_x, physical_y - logical_y);
	transform.scale(physical_rect.width() / logical_rect.width(), physical_rect.height() / logical_rect.height());
	QPainterPath transformedPath = transform.map(m_path);
	bool isClosed = transformedPath.currentPosition() == transformedPath.elementAt(0);
	printPath(transformedPath);
	if (isClosed) {
		// ���������ɫ
		painter.setBrush(QBrush(QColor(m_shapeColor)));
	}

	painter.drawPath(transformedPath);
}

KShapeType KPenLine::getShapeType()
{
	return KShapeType::RectShapeType;
}


void KPenLine::setStartPoint(const QPoint& point) // ��������������
{
	// ��ʼ��penline
	m_physical_start = point;
	qreal scale = getScale();
	m_path.moveTo(point.x() / scale, point.y() / scale);
	
}


void KPenLine::setEndPoint(const QPoint& point) //��������������
{
	m_physical_end = point;
	qreal scale = KGlobalData::getGlobalDataIntance()->getCanvasScale();
	m_path.lineTo(point.x() / scale, point.y() / scale);
	updateRect();
}

void KPenLine::loadEndPoint(const QPoint& point)
{
	m_endPoint= point;
}

void KPenLine::loadStartPoint(const QPoint& point)
{
	m_startPoint= point;
}

void KPenLine::updateRect() // �߼���ַ
{
	QRectF boundRect = m_path.boundingRect();
	qreal scale = KGlobalData::getGlobalDataIntance()->getCanvasScale();
	m_startPoint = QPoint(boundRect.topLeft().x(), boundRect.topLeft().y()); 
	m_endPoint = QPoint(boundRect.bottomRight().x(), boundRect.bottomRight().y());

	m_physical_start = getStartPointInCanvas();
	m_physical_end = getEndPointInCanvas();
}

// ��m_pathִ�б���
void KPenLine::transform(qreal offsetX, qreal offsetY, qreal fixX, qreal fixY)
{
	QRectF boundingRect = m_path.boundingRect(); // 1. ԭpath
	QPainterPath path; // 2. �ػ�path
	for (int i = 0; i < m_path.elementCount(); ++i)
	{
		QPainterPath::Element point = m_path.elementAt(i);
		qreal disY = abs(point.y - fixY);  
		qreal disX = abs(point.x - fixX);  
		// ��ֵ����x,y
		point.y += (disY / boundingRect.height()) * offsetY;
		point.x += (disX / boundingRect.width()) * offsetX;
		if (point.type == QPainterPath::MoveToElement)
			path.moveTo(point);
		else
			path.lineTo(point);
	}
	m_path = path;
	updateRect();
}

void KPenLine::move(QPoint offset)
{
	m_path.translate(offset.x(), offset.y());
	updateRect();
}

void KPenLine::moveTop(QPoint pos)
{
	// �����߼���ַ
	QRectF boundingRect = m_path.boundingRect();

	qreal offsetX = 0;
	qreal offsetY = pos.y() - boundingRect.top();
	qreal fixX = 0;
	qreal fixY = boundingRect.bottom();
	transform(offsetX, offsetY, fixX, fixY);
}

void KPenLine::moveBottom(QPoint pos)
{
	QRectF boundingRect = m_path.boundingRect();

	qreal offsetX = 0;
	qreal offsetY = pos.y() - boundingRect.bottom();
	qreal fixX = 0;
	qreal fixY = boundingRect.top();
	transform(offsetX, offsetY, fixX, fixY);
}

void KPenLine::moveLeft(QPoint pos)
{
	QRectF boundingRect = m_path.boundingRect();

	qreal offsetX = pos.x() - boundingRect.left();
	qreal offsetY = 0;
	qreal fixX = boundingRect.right();
	qreal fixY = 0;
	transform(offsetX, offsetY, fixX, fixY);
}

void KPenLine::moveRight(QPoint pos)
{
	QRectF boundingRect = m_path.boundingRect();

	qreal offsetX = pos.x() - boundingRect.right();
	qreal offsetY = 0;
	qreal fixX = boundingRect.left();
	qreal fixY = 0;
	transform(offsetX, offsetY, fixX, fixY);
}

void KPenLine::moveTopLeft(QPoint pos)
{
	QRectF boundingRect = m_path.boundingRect();

	qreal offsetX = pos.x() - boundingRect.left();
	qreal offsetY = pos.y() - boundingRect.top();
	qreal fixX = boundingRect.right();
	qreal fixY = boundingRect.bottom();
	transform(offsetX, offsetY, fixX, fixY);
}

void KPenLine::moveTopRight(QPoint pos)
{
	QRectF boundingRect = m_path.boundingRect();

	qreal offsetX = pos.x() - boundingRect.right();
	qreal offsetY = pos.y() - boundingRect.top();
	qreal fixX = boundingRect.left();
	qreal fixY = boundingRect.bottom();
	transform(offsetX, offsetY, fixX, fixY);
}

void KPenLine::moveBottomLeft(QPoint pos)
{
	QRectF boundingRect = m_path.boundingRect();

	qreal offsetX = pos.x() - boundingRect.left();
	qreal offsetY = pos.y() - boundingRect.bottom();
	qreal fixX = boundingRect.right();
	qreal fixY = boundingRect.top();
	transform(offsetX, offsetY, fixX, fixY);
}

void KPenLine::moveBottomRight(QPoint pos)
{
	QRectF boundingRect = m_path.boundingRect();

	qreal offsetX = pos.x() - boundingRect.right();
	qreal offsetY = pos.y() - boundingRect.bottom();
	qreal fixX = boundingRect.left();
	qreal fixY = boundingRect.top();
	transform(offsetX, offsetY, fixX, fixY);
}
