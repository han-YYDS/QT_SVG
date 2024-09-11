#ifndef __K_SHAPEFACTORY_H_
#define __K_SHAPEFACTORY_H_

#include <QObject>

#include "svgmanager.h"
#include "kglobaldata.h"
#include "krect.h"
#include "kcircle.h"
#include "kline.h"
#include "kpenline.h"
#include "khexa.h"
#include "kpent.h"
#include "kstar.h"
#include "ktext.h"


class Operation;
class OpDel;
class OpAdd;

class KShapeFactory : public QObject
{
	Q_OBJECT
		
public:
	friend class Operation;
	KShapeFactory(QObject *parent);
	~KShapeFactory();
	static KShape* createShape(KGlobalData::KDrawFlag drawFlag);
	static KShape* createShape(SvgManager::SvgElement element);

signals:
	void createFinished(KShape* shape);
};

#endif
