#ifndef __K_PENT_H_
#define __K_PENT_H_

#include "kshape.h"

class KPent : public KShape
{
	Q_OBJECT

public:
	KPent(QObject *parent = Q_NULLPTR);
	~KPent();
	virtual void drawShape(QPaintDevice* parent = Q_NULLPTR) override;
	virtual void output() override;
	virtual KShapeType getShapeType();
	QPoint* getPoints();
};

#endif
