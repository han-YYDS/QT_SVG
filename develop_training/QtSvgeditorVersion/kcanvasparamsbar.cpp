#include "kcanvasparamsbar.h"

KCanvasParamsBar::KCanvasParamsBar(const QString &title,QWidget *parent)
	: KParamsBar(title,parent)
	, m_pWidthBox(Q_NULLPTR)
	, m_pHeightBox(Q_NULLPTR)
	, m_pColorBox(Q_NULLPTR)
{
	// ��ʼ����� �߶� ��ɫ�� ����box
	m_pWidthBox = new KValueBox(QStringLiteral("���"), KGlobalData::getGlobalDataIntance()->getCanvasWidth(), this);
	m_pHeightBox = new KValueBox(QStringLiteral("�߶�"), KGlobalData::getGlobalDataIntance()->getCanvasHeight(), this);
	m_pColorBox = new KColorBox(QStringLiteral("��ɫ"), KGlobalData::getGlobalDataIntance()->getCanvasColor(), this);

	// TODO: ��ʼֵ����
	// ͼ�������ɫ, ͼ�������߿�, ͼ��������ɫ, ͼ����������
	m_pShapeColorBox= new KShapeColorBox(QStringLiteral("ͼ�������ɫ"), KGlobalData::getGlobalDataIntance()->getCanvasColor(), this);
	m_pShapeLineWidthBox = new KShapeLineWidthBox(QStringLiteral("ͼ�������߿�"), 1 , this);
	m_pShapeLineColorBox = new KShapeLineColorBox(QStringLiteral("ͼ��������ɫ"), KGlobalData::getGlobalDataIntance()->getCanvasColor(), this);
	m_pShapeLineTypeBox = new KShapeLineTypeBox(QStringLiteral("ͼ����������"), this);

	// ��ӵ����񲼾�
	m_pGridLayout->addWidget(m_pWidthBox, 0, 0);// �� 0 �� �� 0 ��
	m_pGridLayout->addWidget(m_pHeightBox, 0, 1);// �� 0 �� �� 1 ��
	m_pGridLayout->addWidget(m_pColorBox, 1, 0);

	m_pGridLayout->addWidget(m_pShapeColorBox, 2, 0); 
	m_pGridLayout->addWidget(m_pShapeLineWidthBox, 2, 1);
	m_pGridLayout->addWidget(m_pShapeLineTypeBox, 3, 0);
	m_pGridLayout->addWidget(m_pShapeLineColorBox, 3, 1);

}

KCanvasParamsBar::~KCanvasParamsBar()
{
}
