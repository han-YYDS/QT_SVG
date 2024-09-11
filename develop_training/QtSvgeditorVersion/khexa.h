#ifndef __K_HEXA_H_
#define __K_HEXA_H_

#include "kshape.h"

class KHexa : public KShape
{
	Q_OBJECT

public:
	KHexa(QObject *parent = Q_NULLPTR);
	~KHexa();
	virtual void drawShape(QPaintDevice* parent = Q_NULLPTR) override;
	virtual void KHexa::output() override;
	virtual KShapeType getShapeType();
	QPoint* getPoints();
};

#endif
