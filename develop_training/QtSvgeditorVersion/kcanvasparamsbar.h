#ifndef __K_CANVAS_PARAMS_BAR_H_
#define __K_CANVAS_PARAMS_BAR_H_

#include "kparamsbar.h"
#include "kcolorbox.h"
#include "kvaluebox.h"
#include "kshapecolorbox.h"
#include "kshapelinewidthbox.h"
#include "kshapelinecolorbox.h"
#include "kcombobox.h"
#include "kshapelinetype.h"


class KCanvasParamsBar : public KParamsBar
{
	Q_OBJECT
	friend class KMainWindow;
public:
	KCanvasParamsBar(const QString &title,QWidget *parent);
	~KCanvasParamsBar();

	// 画布属性
	KValueBox *m_pWidthBox;
	KValueBox *m_pHeightBox;
	KColorBox *m_pColorBox; // 画布底色

	// 图形属性
	KShapeLineColorBox* m_pShapeLineColorBox;
	KShapeLineWidthBox* m_pShapeLineWidthBox;
	KShapeColorBox* m_pShapeColorBox;
	KShapeLineTypeBox* m_pShapeLineTypeBox;


};

#endif
