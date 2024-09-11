#ifndef __K_SHAPE_LINE_WIDTH_BOX_H_
#define __K_SHAPE_LINE_WIDTH_BOX_H_


#include "kvaluebox.h"


class KShapeLineWidthBox : public KValueBox
{
	Q_OBJECT

	friend class KMainWindow;
public:
	KShapeLineWidthBox(const QString &title,const int value,QWidget *parent);
	~KShapeLineWidthBox();

signals:
	void setShapeLineWidth(int value);
	
};

#endif
