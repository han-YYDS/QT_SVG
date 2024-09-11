#include "krect.h"
#include "kglobaldata.h"
#include <QPainter>

KRect::KRect(QObject *parent)
	: KShape(parent)
{
	
}

KRect::~KRect()
{
}


void KRect::drawShape(QPaintDevice* parent)
{

	QPainter painter(parent);
	painter.begin(parent);
	QPen pen;
	pen.setWidth(m_shapeLineWidth);
	pen.setColor(m_shapeLineColor);
	pen.setStyle(m_shapeLineType);
	painter.setPen(pen);
	QColor fillColor(m_shapeColor);
	QBrush brush(fillColor);
	painter.setBrush(brush);

	 // qDebug() << "fillcolor color is " << m_shapeColor << " line color is " << m_shapeLineColor << " linewidth = " << m_shapeLineWidth;
	painter.drawRect(QRect(getStartPointInCanvas(), getEndPointInCanvas()));

}
// 
void KRect::output()
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
	painter->drawRect(QRect(getStartPointInCanvas(), getEndPointInCanvas()));
}


KShapeType KRect::getShapeType()
{
	return KShapeType::RectShapeType;
}

KShape* KRect::clone()
{
	return new KRect(*this);
}
