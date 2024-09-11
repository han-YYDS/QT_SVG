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
	CircleShapeType,// Բ��
	RectShapeType,
	PentShapeType,	// �����
	HexaShapeType,	// ������
	TextShapeType,
	StarShapeType,	// ����
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
	
	// ����������д��ʵ�ֲ�ͬ���͵�ͼ�εĻ���
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
	

	// ���������е�����
	QPoint getStartPoint();
	QPoint getEndPoint();

	// �������е�����
	QPoint getStartPointInCanvas() const;	
	QPoint getEndPointInCanvas() const;

	QString dumpPoint (const QPoint & p) const { return QString("[" + QString::number(p.x()) + "," + QString::number(p.y()) + "]"); }

	virtual void setStartPoint(const QPoint &point);
	virtual void setEndPoint(const QPoint &point);
	

	void drawOutLine(QPaintDevice* parent = Q_NULLPTR);
	QRect getShapeRect() const;
	bool isValid();

	// ͼ������
	QString m_shapeLineColor = Qt::black;		// ������ɫ
	QString m_shapeColor = Qt::white;			// �����ɫ
	int m_shapeLineWidth = 1;		// �߿�
	Qt::PenStyle m_shapeLineType = Qt::SolidLine;		// ����

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
	QPoint m_startPoint;// ��ʼ�߼�����
	QPoint m_endPoint; // �����߼�����

	QPoint m_physical_start;// ��ʼ��������
	QPoint m_physical_end; // ������������
};

#endif
