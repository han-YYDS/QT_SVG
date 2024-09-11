#ifndef __K_SHAPE_LINE_TYPE_BOX_H_
#define __K_SHAPE_LINE_TYPE_BOX_H_

#include "kcombobox.h"


class KShapeLineTypeBox : public KComboBox
{
	Q_OBJECT

	friend class KMainWindow;
public:
	KShapeLineTypeBox(const QString &title,QWidget *parent);
	~KShapeLineTypeBox();
	void setItem(int index) override;
protected:
};

#endif
