#ifndef __K_SHAPE_COLOR_BOX_H_
#define __K_SHAPE_COLOR_BOX_H_

#include "kcolorbox.h"

// Õº–ŒÃÓ≥‰—’…´…Ë÷√



class KShapeColorBox : public KColorBox
{
	Q_OBJECT

	friend class KMainWindow;
public:
	KShapeColorBox(const QString& title, const QString& rgbColorStr, QWidget* parent);
	~KShapeColorBox();
	void KShapeColorBox::setColor(QString color) override;
};

#endif
