#ifndef __K_STAR_H_
#define __K_STAR_H_

#include "kshape.h"

class KStar : public KShape
{
	Q_OBJECT

public:
	KStar(QObject *parent = Q_NULLPTR);
	~KStar();
	virtual void drawShape(QPaintDevice* parent = Q_NULLPTR) override;
	virtual void output() override;
	virtual KShapeType getShapeType();
	QPoint* getPoints();
};

#endif
