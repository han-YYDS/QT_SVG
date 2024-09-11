#ifndef __SVG_MANAGER_H_
#define __SVG_MANAGER_H_

#include <QFile>
#include <QXmlStreamReader>
#include <QDebug>
#include <QHash>
#include "kshape.h"

class SvgManager	
{

public:
	SvgManager();
	~SvgManager();
	struct SvgElement {
		QString text;
		QString type;
		QHash<QString, QString> attributes;
	};
public:
	void dumpElement(SvgElement ele);

	void parseFile(const QString& fileName);

	QList<KShape*> loadElements();

	QList<SvgElement> getElements() const {
		return elements;
	}

private:
	QList<SvgElement> elements;
};

#endif
