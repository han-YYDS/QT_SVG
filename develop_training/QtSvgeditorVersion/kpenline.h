#ifndef __K_PEN_LINE_H_
#define __K_PEN_LINE_H_
#include <QPainter>
#include "kshape.h"
#include <QPainterPath>
class KPenLine : public KShape
{
	Q_OBJECT

public:
	KPenLine(QObject *parent = Q_NULLPTR);
	~KPenLine();
	virtual void drawShape(QPaintDevice* parent = Q_NULLPTR) override; // 这里是负责重绘而不是初次绘制
	virtual KShapeType getShapeType();
	
	virtual void setStartPoint(const QPoint& point) override;
	virtual void setEndPoint(const QPoint& point)override;

	void loadEndPoint(const QPoint& point); // 补丁:用于修补loadfile时的setstart和setend
	void loadStartPoint(const QPoint& point);

	void updateRect(); // 更新框
	void transform(qreal offsetX, qreal offsetY, qreal fixX, qreal fixY);
	

	static void printPath(const QPainterPath& path) {
		// 遍历路径中的所有元素
		for (int i = 0; i < path.elementCount(); ++i) {
			QPainterPath::Element element = path.elementAt(i);

			switch (element.type) {
			case QPainterPath::MoveToElement:
				qDebug() << "MoveTo:" << element.x << element.y;
				break;
			case QPainterPath::LineToElement:
				qDebug() << "LineTo:" << element.x << element.y;
				break;
			case QPainterPath::CurveToElement:
				qDebug() << "CurveTo (control point):" << element.x << element.y;
				break;
			case QPainterPath::CurveToDataElement:
				qDebug() << "CurveToData:" << element.x << element.y;
				break;
			}
		}
	}

	// 由于movedrag中存在path的重绘, 所以重载
	// 这里传入的point都是逻辑坐标
	virtual void move(QPoint offset) override;
	virtual void moveTop(QPoint pos)override;
	virtual void moveBottom(QPoint pos)override;
	virtual void moveLeft(QPoint pos)override;
	virtual void moveRight(QPoint pos)override;
	virtual void moveTopLeft(QPoint pos)override;
	virtual void moveTopRight(QPoint pos)override;
	virtual void moveBottomLeft(QPoint pos)override;
	virtual void moveBottomRight(QPoint pos)override;

	void setPath(QPainterPath path) { m_path = path; }
private:
	QPainterPath m_path; // 逻辑坐标
};

#endif
