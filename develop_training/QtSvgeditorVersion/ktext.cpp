#include "ktext.h"
#include <QPainter>

#include "kglobaldata.h"
#include "kcanvas.h"
KText::KText(QObject* parent)
	: KShape(parent)
	, m_currentText("")
	, m_isEditing(true)
	, m_textRect(0, 0, 200, 100)
	, m_fontFamily("")
	, m_fontSize(12)
{
	m_font = QFont(m_fontFamily, m_fontSize);
}

KText::KText(const QString& text, const QString& fontFamily, int fontSize, QObject* parent)
	: KShape(parent)
	, m_currentText(text)
	, m_isEditing(false)
	, m_textRect(0, 0, 200, 100)
	, m_fontFamily(fontFamily)
	, m_fontSize(fontSize)
{
	m_font = QFont(m_fontFamily, m_fontSize);
}

KText::KText(const KText& other)
	:KShape(other)
	, m_textRect(other.m_textRect)
	, m_currentText(other.m_currentText)
	, m_oldText(other.m_oldText)
	, m_fontFamily(other.m_fontFamily)
	, m_fontSize(other.m_fontSize)
{
	m_font = QFont(m_fontFamily, m_fontSize);
}

KText::~KText()
{
}

void KText::drawShape(QPaintDevice* parent)
{
	if (this == KGlobalData::getGlobalDataIntance()->getCanvas()->getCurrentShape())
	{
		drawRect(parent);// 绘制文本矩形
	}
	

	QPainter painter(parent);
	// 创建并设置画笔
	QPen pen;
	pen.setWidth(1);
	pen.setColor(Qt::black);
	painter.setPen(pen);
	painter.setFont(m_font);


	painter.drawText(getShapeRect(), Qt::AlignCenter, m_currentText);
}

void KText::output()
{
	QPainter* painter = KGlobalData::getGlobalDataIntance()->getPainter();
	QPen pen;
	pen.setColor(Qt::black);
	pen.setWidth(1);
	pen.setStyle(Qt::DashLine);
	painter->setPen(pen);
	painter->drawText(getShapeRect(), Qt::AlignCenter, m_currentText);
}

void KText::drawRect(QPaintDevice* parent)
{
	QPainter painter(parent);
	QPen pen;

	pen.setColor(Qt::black);
	pen.setWidth(1);
	pen.setStyle(Qt::DashLine);
	painter.setPen(pen);

	painter.drawRect(getShapeRect());
}

KShape* KText::clone()
{
	return new KText(*this);
}

KShapeType KText::getShapeType()
{
	return KShapeType::TextShapeType;
}

void KText::setStartPoint(const QPoint& point) // 输入的是物理坐标
{
	qreal scale = KGlobalData::getGlobalDataIntance()->getCanvasScale();
	QPoint start = QPoint(point.x() / scale, point.y() / scale);
	m_startPoint = start - QPoint(100, 20); //设置编辑矩形
	m_textRect.setTopLeft(QPoint(m_startPoint.x()*scale, m_startPoint.y() * scale));

}

void KText::setEndPoint(const QPoint& point)
{
	qreal scale = KGlobalData::getGlobalDataIntance()->getCanvasScale();
	QPoint end = QPoint(point.x() / scale, point.y() / scale);
	m_endPoint = end + QPoint(100, 50); //设置编辑矩形
	m_textRect.setBottomLeft(QPoint(m_endPoint.x() * scale, m_endPoint.y() * scale));
}

QString KText::getCurrentText() const
{
	return m_currentText;
}

void KText::setCurrentText(const QString& text)
{
	m_currentText = text;
	int rectWidth = getShapeRect().width();
	if (m_currentText.size() * 12 > rectWidth)
	{
		m_endPoint += QPoint(100, 0);
		// m_oldEndPoint = m_endPoint / KGlobalData::getGlobalDataIntance()->getCanvasScale();
	}
}

void KText::setIsEditing(bool isEditing)
{
	m_isEditing = isEditing;
}
