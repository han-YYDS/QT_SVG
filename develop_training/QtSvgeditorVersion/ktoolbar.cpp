#include "ktoolbar.h"
#include "kglobaldata.h"

KToolBar::KToolBar(QWidget *parent)
	: QWidget(parent)
	, m_pVLayout(NULL)
	, m_pBtnGroup(NULL)
	, m_pMouseBtn(NULL)
	, m_pPenBtn(NULL)
	, m_pLineBtn(NULL)
	, m_pRectBtn(NULL)
	, m_pCircleBtn(NULL)
	, m_pTextBtn(NULL)
{
	setMaximumWidth(50);// ���������Ϊ 50
	setAttribute(Qt::WA_StyledBackground, true);// �Զ���ؼ���ʽ����Ч
	setStyleSheet("background-color:#000000");// ������ʽ��

	//��ֱ���ֶ���
	m_pVLayout = new QVBoxLayout(this);
	m_pVLayout->setAlignment(Qt::AlignVCenter | Qt::AlignTop);// ���ö��뷽ʽ
	m_pVLayout->setSpacing(25);// ���ò������е�����ļ��

	m_pBtnGroup = new QButtonGroup(this);
	m_pBtnGroup->setExclusive(true);// ���û��⣬ֻ����һ����ť���� checked ״̬

	// ���� �Զ��尴ť
	m_pMouseBtn = new KToolButton(KGlobalData::KDrawFlag::MouseDrawFlag, this);
	m_pPenBtn = new KToolButton(KGlobalData::KDrawFlag::PenDrawFlag, this);
	m_pLineBtn = new KToolButton(KGlobalData::KDrawFlag::LineDrawFlag, this);
	m_pRectBtn = new KToolButton(KGlobalData::KDrawFlag::RectDrawFlag, this);
	m_pCircleBtn = new KToolButton(KGlobalData::KDrawFlag::CircleDrawFlag, this);
	m_pTextBtn = new KToolButton(KGlobalData::KDrawFlag::TextDrawFlag, this);
	m_pHexaBtn = new KToolButton(KGlobalData::KDrawFlag::HexaDrawFlag, this);
	m_pPentBtn = new KToolButton(KGlobalData::KDrawFlag::PentDrawFlag, this);
	m_pStarBtn = new KToolButton(KGlobalData::KDrawFlag::StarDrawFlag, this);

	// ����ť��ӵ� ��ť����
	m_pBtnGroup->addButton(m_pMouseBtn);
	m_pBtnGroup->addButton(m_pPenBtn);
	m_pBtnGroup->addButton(m_pLineBtn);
	m_pBtnGroup->addButton(m_pRectBtn);
	m_pBtnGroup->addButton(m_pCircleBtn);
	m_pBtnGroup->addButton(m_pTextBtn);
	m_pBtnGroup->addButton(m_pHexaBtn);
	m_pBtnGroup->addButton(m_pPentBtn);
	m_pBtnGroup->addButton(m_pStarBtn);

	// ���Զ��尴ť��ӵ���������
	m_pVLayout->addWidget(m_pMouseBtn);
	m_pVLayout->addWidget(m_pPenBtn);
	m_pVLayout->addWidget(m_pLineBtn);
	m_pVLayout->addWidget(m_pRectBtn);
	m_pVLayout->addWidget(m_pCircleBtn);
	m_pVLayout->addWidget(m_pTextBtn);
	m_pVLayout->addWidget(m_pHexaBtn);
	m_pVLayout->addWidget(m_pPentBtn);
	m_pVLayout->addWidget(m_pStarBtn);
	
	// ����������ӵ� qwidget ��
	setLayout(m_pVLayout);
}


KToolBar::~KToolBar()
{
}
