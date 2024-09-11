#ifndef __K_GLOBAL_DATA__
#define __K_GLOBAL_DATA__

#include <QObject>
#include <QDebug>
#include <QPainter>
#include <QStack>

class Operation;
class KCanvas;
class KShape;
class KCanvasParamsBar;
class KGlobalData : public QObject
{
	Q_OBJECT

public:
	enum class KDrawFlag
	{
		NoneDrawFlag = 0,
		MouseDrawFlag,	// 鼠标箭头
		PenDrawFlag,	// 画笔
		LineDrawFlag,	// 直线
		RectDrawFlag,	// 矩形
		CircleDrawFlag, // 圆形
		TextDrawFlag,	// 文本
		HexaDrawFlag,	// 六边形
		PentDrawFlag,	// 五边形
		StarDrawFlag	// 星型
	};



	KGlobalData(QObject *parent = Q_NULLPTR);
	~KGlobalData();

	static KGlobalData *getGlobalDataIntance();

	void setDrawFlag(KGlobalData::KDrawFlag drawflag);
	KDrawFlag getDrawFlag();

	void setCanvasWidth(const int width);
	void setCanvasHeight(const int height);

	int getCanvasWidth() const;
	int getCanvasHeight() const;

	void setCanvasColor(const QString & colorStr);
	QString getCanvasColor();
	

	void setShapeColor(const QString& colorStr) { m_shapeColor = colorStr; }
	QString getShapeColor() { return m_shapeColor; }

	void setShapeLineType(Qt::PenStyle style) { m_shapeLineType = style; }
	Qt::PenStyle getShapeLineType() { return m_shapeLineType; }

	void setLineColor(const QString& colorStr) { m_lineColor = colorStr; }
	QString getLineColor() { return m_lineColor; }

	void setLineWidth(int width) { m_lineWidth = width; }
	int getLineWidth() { return m_lineWidth; }


	void setCanvaScale(qreal scale);
	qreal getCanvasScale();

	KShape* getSelectedShape();	// 获得当前选中shape
	void setSelectedShape();

	KCanvas* getCanvas() { return m_pCanvas; }
	void setCanvas(KCanvas* canvas) { m_pCanvas = canvas; }

	KCanvasParamsBar* getCanvasParamBar() { return m_pParamBar; }
	void setCanvasParamBar(KCanvasParamsBar* bar) { m_pParamBar = bar; }

	bool m_isPressCtrl = false;
	QPainter* getPainter() { return m_pPainter; }

	Operation* popUndo();
	Operation* popRedo() { auto it = m_redoStack.top(); m_redoStack.pop(); return it; }

	QStack<Operation*> getRedo() { return m_redoStack; }
	QStack<Operation*> getUndo() { return m_undoStack; }

	void pushUndo(Operation* op) { m_undoStack.push(op); }
	void pushRedo(Operation* op) { m_redoStack.push(op); }

		
private:
	KGlobalData(const KGlobalData &other) = delete;
	KGlobalData(const KGlobalData &&other) = delete;
	void operator=(const KGlobalData) = delete;

	KDrawFlag m_drawFlag;
	QPainter* m_pPainter;
	int m_canvasWidth;
	int m_canvasHeight;
	QString m_canvasColor;
	QString m_shapeColor;
	QString m_lineColor;
	Qt::PenStyle m_shapeLineType;
	int m_lineWidth;
	KShape* m_pSelectedShape;
	qreal m_scale;
	KCanvas* m_pCanvas;
	KCanvasParamsBar* m_pParamBar;

	QStack<Operation*> m_redoStack;
	QStack<Operation*> m_undoStack;

	 
};


#endif
