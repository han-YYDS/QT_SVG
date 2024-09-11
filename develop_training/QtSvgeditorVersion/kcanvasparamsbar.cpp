#include "kcanvasparamsbar.h"

KCanvasParamsBar::KCanvasParamsBar(const QString &title,QWidget *parent)
	: KParamsBar(title,parent)
	, m_pWidthBox(Q_NULLPTR)
	, m_pHeightBox(Q_NULLPTR)
	, m_pColorBox(Q_NULLPTR)
{
	// 初始化宽度 高度 颜色框 三个box
	m_pWidthBox = new KValueBox(QStringLiteral("宽度"), KGlobalData::getGlobalDataIntance()->getCanvasWidth(), this);
	m_pHeightBox = new KValueBox(QStringLiteral("高度"), KGlobalData::getGlobalDataIntance()->getCanvasHeight(), this);
	m_pColorBox = new KColorBox(QStringLiteral("颜色"), KGlobalData::getGlobalDataIntance()->getCanvasColor(), this);

	// TODO: 初始值设置
	// 图形填充颜色, 图形轮廓线宽, 图形轮廓颜色, 图形轮廓线型
	m_pShapeColorBox= new KShapeColorBox(QStringLiteral("图形填充颜色"), KGlobalData::getGlobalDataIntance()->getCanvasColor(), this);
	m_pShapeLineWidthBox = new KShapeLineWidthBox(QStringLiteral("图形轮廓线宽"), 1 , this);
	m_pShapeLineColorBox = new KShapeLineColorBox(QStringLiteral("图形轮廓颜色"), KGlobalData::getGlobalDataIntance()->getCanvasColor(), this);
	m_pShapeLineTypeBox = new KShapeLineTypeBox(QStringLiteral("图形轮廓线型"), this);

	// 添加到网格布局
	m_pGridLayout->addWidget(m_pWidthBox, 0, 0);// 第 0 行 第 0 列
	m_pGridLayout->addWidget(m_pHeightBox, 0, 1);// 第 0 行 第 1 列
	m_pGridLayout->addWidget(m_pColorBox, 1, 0);

	m_pGridLayout->addWidget(m_pShapeColorBox, 2, 0); 
	m_pGridLayout->addWidget(m_pShapeLineWidthBox, 2, 1);
	m_pGridLayout->addWidget(m_pShapeLineTypeBox, 3, 0);
	m_pGridLayout->addWidget(m_pShapeLineColorBox, 3, 1);

}

KCanvasParamsBar::~KCanvasParamsBar()
{
}
