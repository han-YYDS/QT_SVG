#ifndef __K_LINE_COLOR_BOX_H_
#define __K_LINE_COLOR_BOX_H_


#include "kcolorbox.h"


class KShapeLineColorBox : public KColorBox
{
	Q_OBJECT

	friend class KMainWindow;
public:
	KShapeLineColorBox(const QString& title, const QString& rgbColorStr, QWidget* parent);
	~KShapeLineColorBox();

	void setColor(QString str);
private:

};

#endif
