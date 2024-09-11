#ifndef __K_SHAPE_H_
#define __K_SHAPE_H_


#include <QObject>
#include <QPoint>
#include <QPaintDevice>
#include <QPainter>
#include <QDebug>


enum class KShapeType
{
	None = 0,
	PenShapeType,
	LineShapeType,
	CircleShapeType,// 圆形
	RectShapeType,
	PentShapeType,	// 五边形
	HexaShapeType,	// 六边形
	TextShapeType,
	StarShapeType,	// 星型
};

enum class KShapeParaType
{
	LineColor = 0,
	ShapeColor,
	LineWidth,
	LineType,
};

class KShape : public QObject
{
	Q_OBJECT

public:
	KShape(QObject *parent = Q_NULLPTR);
	KShape(const KShape& other);
	~KShape();
	
	// 在子类中重写，实现不同类型的图形的绘制
	virtual void drawShape(QPaintDevice *parent = Q_NULLPTR) = 0;
	virtual void output() {}
	virtual void move(QPoint offset);
	virtual void moveTop(QPoint pos);
	virtual void moveBottom(QPoint pos);
	virtual void moveLeft(QPoint pos);
	virtual void moveRight(QPoint pos);
	virtual void moveTopLeft(QPoint pos);
	virtual void moveTopRight(QPoint pos);
	virtual void moveBottomLeft(QPoint pos);
	virtual void moveBottomRight(QPoint pos);
	virtual KShapeType getShapeType();
	

	// 参数画布中的坐标
	QPoint getStartPoint();
	QPoint getEndPoint();

	// 物理画布中的坐标
	QPoint getStartPointInCanvas() const;	
	QPoint getEndPointInCanvas() const;

	QString dumpPoint (const QPoint & p) const { return QString("[" + QString::number(p.x()) + "," + QString::number(p.y()) + "]"); }

	virtual void setStartPoint(const QPoint &point);
	virtual void setEndPoint(const QPoint &point);
	

	void drawOutLine(QPaintDevice* parent = Q_NULLPTR);
	QRect getShapeRect() const;
	bool isValid();

	// 图形属性
	QString m_shapeLineColor = Qt::black;		// 轮廓颜色
	QString m_shapeColor = Qt::white;			// 填充颜色
	int m_shapeLineWidth = 1;		// 线宽
	Qt::PenStyle m_shapeLineType = Qt::SolidLine;		// 线型

	// QString getShapeLineColor();
	QString getShapeColor() { return m_shapeColor; }
	int getShapeLineWidth() { return m_shapeLineWidth; }
	Qt::PenStyle getShapeLineType() { return m_shapeLineType; }
	QString getShapeLineColor() { return m_shapeLineColor; }

	void setShapeLineType(Qt::PenStyle style) { m_shapeLineType = style; }
	void setShapeLineWidth(int width) { m_shapeLineWidth = width; }
	void setShapeColor(QString color) { m_shapeColor = color; }
	void setShapeLineColor(QString color) { m_shapeLineColor = color; }
	
	virtual KShape* clone() { return Q_NULLPTR; }
	QString dump() { return dumpPoint(m_startPoint) + " " + dumpPoint(m_endPoint); }
public slots:


protected:
	QPoint m_startPoint;// 起始逻辑坐标
	QPoint m_endPoint; // 结束逻辑坐标

	QPoint m_physical_start;// 起始物理坐标
	QPoint m_physical_end; // 结束物理坐标
};

#endif
