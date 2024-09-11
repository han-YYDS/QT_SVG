#ifndef __K_CANVAS_H_
#define __K_CANVAS_H_

#include <QWidget>
#include <QStack>
#include "kshape.h"
#include "kglobaldata.h"

class KShapeFactory;
class KText;
enum class KTransType
{
	None = 0, // 点击图形位置
	TopLeft,	
	Top,
	TopRight,
	Left,
	Contains,// 移动图形, 其余为拖拽变形
	Right,
	BottomLeft,
	Bottom,
	BottomRight
};

enum class KCanvasParaType
{
	Width = 0,
	Height,
	Color,
};


class KCanvas : public QWidget
{
	Q_OBJECT

public:
	KCanvas(QWidget *parent = Q_NULLPTR);
	~KCanvas();

	virtual void paintEvent(QPaintEvent* event) override;
	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override; // 输入文本

	KShape* getCurrentShape(const QPoint &pos); // 获取当前位置图形
	KShape* getCurrentShape() { return m_pCurrentShape; } // 返回当前图形
	QList<KShape*> getShapeList() { return m_pShapeList; }; // 全选时的图形
	QList<KShape*> getSelected() { return m_pSelectedShapeList; }; // 多选时的图形
	QStack<KShape*> getDeleteStack() { return m_pDeleteStack; }; // 多选时的图形
	void setShapeList(QList<KShape*> list) { m_pShapeList = list; }
	KTransType getTransType(const QPoint &pos); // 获取移动类型
	void updateCusorStyle(KGlobalData::KDrawFlag flag, KTransType transType);
	void dragMoveShape(KTransType transType,const QPoint &pos);
	void MoveSelectedShape(KTransType transType, const QPoint& pos);
	void selectAll();
	void copySeleted();
	void paste();
	void deleteSelected();
	void finishedEditing();

	KShape* deletePop();
	void deletePush(KShape*);
	void ShapeListPrepend(KShape*);
	void ShapeListAppend(KShape*);
	void removeFromShapeList(KShape*);
	QString getColor() { return m_color; }
	void setColor(QString color) { m_color = color; }
signals:
	void selected(KShape* selectedShape);



private:
	KShape* m_pCurrentShape;				// 当前绘制图形
	QList<KShape*> m_pShapeList;			// 存储当前画布中的图形
	QList<KShape*> m_pClipboard;			// 剪切板
	QList<KShape*> m_pSelectedShapeList;	// 多选时的图形
	QStack<KShape*> m_pDeleteStack;			// 删除缓存
	int m_cursorPosition = 0;					// 文本框中光标位置
	
	QPoint m_lastPos;// 记录前一次的位置
	KTransType m_TransType;// 记录移动类型
	
	bool m_isLPress;// 标记鼠标左键
	KText* currediting;
	bool m_isDrawing;// 是否绘图
	bool m_isSelected;// 是否为选中
	bool m_isEditing = false;	// 是否正在输入文本
	bool m_isSelectedAll = false;// 是否为全选状态

	QString m_color = "#FFFFFF";

};

#endif
