#ifndef __K_RECT_H_
#define __K_RECT_H_

#include "kshape.h"

class KRect : public KShape
{
	Q_OBJECT

public:
	KRect(QObject *parent = Q_NULLPTR);
	~KRect();
	virtual void drawShape(QPaintDevice* parent = Q_NULLPTR) override;
	virtual void output() override;
	virtual KShapeType getShapeType();

	KShape* clone() override;
};

#endif
