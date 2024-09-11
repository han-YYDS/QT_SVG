#ifndef __K_OPERATION_H_
#define __K_OPERATION_H_

#include <QVariant>

#include "kshape.h"
#include "kcanvas.h"

class KShape;

class Operation
{
public:
	friend class KShapeFactory;
	Operation()
	{
		// TODO: Ӧ�����ｫredostack���
	}
	virtual Operation*  reverse() = 0;	// ��������
	virtual void exec() = 0;	// ִ�в���
	virtual void dump() = 0;
};


class OpAdd : public Operation
{
public:
	OpAdd(KShape* shape = nullptr);
	virtual Operation* reverse() override;
	virtual void exec() override;
	virtual void dump() override { qDebug() << "[DEBUG] Add"; }
	KShape* m_pShape;
};

class OpDel : public Operation
{
public:
	OpDel(KShape* shape = nullptr);
	virtual Operation* reverse() override;
	virtual void exec() override;
	virtual void dump() override { qDebug() << "[DEBUG] Del"; }
	KShape* m_pShape;
};

class OpShapePara : public Operation
{
public:
	OpShapePara(KShape* shape, KShapeParaType type, QVariant oldval, QVariant newval);
	virtual Operation* reverse() override;
	virtual void exec() override;
	virtual void dump() override { qDebug() << "[DEBUG] Shape Para"; }

	KShapeParaType m_type;
	KShape* m_pShape;
	QVariant m_new;
	QVariant m_old;
};

class OpCanvasPara : public Operation
{
public:
	OpCanvasPara(KCanvasParaType type, QVariant oldval, QVariant newval);
	virtual Operation* reverse() override;
	virtual void exec() override;
	virtual void dump() override { qDebug() << "[DEBUG] Canvas Para"; }
	// set shape.para from old to new 
	KCanvasParaType m_type;
	KShape* m_pShape;
	QVariant m_new;
	QVariant m_old;
};

#endif
