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
	None = 0, // ���ͼ��λ��
	TopLeft,	
	Top,
	TopRight,
	Left,
	Contains,// �ƶ�ͼ��, ����Ϊ��ק����
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
	void keyPressEvent(QKeyEvent* event) override; // �����ı�

	KShape* getCurrentShape(const QPoint &pos); // ��ȡ��ǰλ��ͼ��
	KShape* getCurrentShape() { return m_pCurrentShape; } // ���ص�ǰͼ��
	QList<KShape*> getShapeList() { return m_pShapeList; }; // ȫѡʱ��ͼ��
	QList<KShape*> getSelected() { return m_pSelectedShapeList; }; // ��ѡʱ��ͼ��
	QStack<KShape*> getDeleteStack() { return m_pDeleteStack; }; // ��ѡʱ��ͼ��
	void setShapeList(QList<KShape*> list) { m_pShapeList = list; }
	KTransType getTransType(const QPoint &pos); // ��ȡ�ƶ�����
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
	KShape* m_pCurrentShape;				// ��ǰ����ͼ��
	QList<KShape*> m_pShapeList;			// �洢��ǰ�����е�ͼ��
	QList<KShape*> m_pClipboard;			// ���а�
	QList<KShape*> m_pSelectedShapeList;	// ��ѡʱ��ͼ��
	QStack<KShape*> m_pDeleteStack;			// ɾ������
	int m_cursorPosition = 0;					// �ı����й��λ��
	
	QPoint m_lastPos;// ��¼ǰһ�ε�λ��
	KTransType m_TransType;// ��¼�ƶ�����
	
	bool m_isLPress;// ���������
	KText* currediting;
	bool m_isDrawing;// �Ƿ��ͼ
	bool m_isSelected;// �Ƿ�Ϊѡ��
	bool m_isEditing = false;	// �Ƿ����������ı�
	bool m_isSelectedAll = false;// �Ƿ�Ϊȫѡ״̬

	QString m_color = "#FFFFFF";

};

#endif
