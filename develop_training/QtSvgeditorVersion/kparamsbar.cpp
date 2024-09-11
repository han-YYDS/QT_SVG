#include "kparamsbar.h"

KParamsBar::KParamsBar(const QString &title,QWidget *parent)
	: QWidget(parent)
	, m_pVBoxLayout(Q_NULLPTR)
	, m_pGridLayout(Q_NULLPTR)
	, m_pTitleLabel(Q_NULLPTR)
	, m_title(title)
{
	setMaximumWidth(170);
	setFocusPolicy(Qt::ClickFocus);//单击获得焦点
	setStyleSheet("background-color:#2f2f2c");
	
	m_pVBoxLayout = new QVBoxLayout(this);
	m_pVBoxLayout->setContentsMargins(10, 0, 10, 0);
	m_pVBoxLayout->setAlignment(Qt::AlignTop);

	m_pTitleLabel = new QLabel(m_title, this);
	m_pTitleLabel->setStyleSheet("background-color:#2f2f2c; font-size:14px; color:#FFFFFF; font-family: Microsoft YaHei");

	m_pGridLayout = new QGridLayout(this);
	m_pGridLayout->setSpacing(10);

	m_pVBoxLayout->addWidget(m_pTitleLabel);
	m_pVBoxLayout->addLayout(m_pGridLayout);
	
	setLayout(m_pVBoxLayout);
}

KParamsBar::~KParamsBar()
{
}
