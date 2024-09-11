#ifndef __K_SVG_TOOL_BAR_H_
#define __K_SVG_TOOL_BAR_H_

#include <QMenuBar>
#include <QtSvg/qsvgrenderer.h>
#include <qpainter.h>
#include <qlabel.h>
#include <qbuttongroup.h>
#include <qstyleoption.h>
#include <qlayout.h>

#include "ktoolbutton.h"

class KToolBar : public QWidget
{
	Q_OBJECT

public:
	KToolBar(QWidget *parent);
	~KToolBar();
private:
	QVBoxLayout* m_pVLayout; // ´¹Ö±²¼¾Ö
	QButtonGroup* m_pBtnGroup;
	KToolButton* m_pMouseBtn;
	KToolButton* m_pPenBtn;
	KToolButton* m_pLineBtn;
	KToolButton* m_pRectBtn;
	KToolButton* m_pCircleBtn;
	KToolButton* m_pTextBtn;
	KToolButton* m_pHexaBtn;
	KToolButton* m_pPentBtn;
	KToolButton* m_pStarBtn;
};

#endif
