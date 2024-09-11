#include "ktoolbutton.h"

KToolButton::KToolButton(KGlobalData::KDrawFlag drawflag,QWidget *parent)
	: QRadioButton(parent)
	, m_pSvgRenderer(nullptr)
	, m_drawFlag(drawflag)
{
	initToolButton();
}

KToolButton::~KToolButton()
{
}

void KToolButton::initToolButton()
{
	switch (m_drawFlag)
	{
	case KGlobalData::KDrawFlag::MouseDrawFlag:
		m_iconName = "mouse.svg";
		break;
	case KGlobalData::KDrawFlag::PenDrawFlag:
		m_iconName = "pen.svg";
		break;
	case KGlobalData::KDrawFlag::LineDrawFlag:
		m_iconName = "line.svg";
		break;
	case KGlobalData::KDrawFlag::RectDrawFlag:
		m_iconName = "rect.svg";
		break;
	case KGlobalData::KDrawFlag::CircleDrawFlag:
		m_iconName = "circle.svg";
		break;
	case KGlobalData::KDrawFlag::TextDrawFlag:
		m_iconName = "text.svg";
		break;
	case KGlobalData::KDrawFlag::HexaDrawFlag:
		m_iconName = "hexa.svg";
		break;
	case KGlobalData::KDrawFlag::PentDrawFlag:
		m_iconName = "pent.svg";
		break;
	case KGlobalData::KDrawFlag::StarDrawFlag:
		m_iconName = "star.svg";
		break;
	default:
		break;
	}

	m_pSvgRenderer = new QSvgRenderer(":/icons/normal/" + m_iconName);
	this->setMinimumSize(32, 32);

	//toggled :在按钮状态发生变化时,发出此信号
	(void)connect(this, &KToolButton::toggled,this,&KToolButton::onToggledButton);
}


void KToolButton::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	m_pSvgRenderer->render(&painter,QRectF(0,0,32,32));

}

void KToolButton::enterEvent(QEvent* event)
{
	if (!isChecked())//判断按钮是否选中
		m_pSvgRenderer->load(":/icons/hover/" + m_iconName);//如果没有选中，加载 hover 的图标

	setCursor(Qt::PointingHandCursor); //设置光标样式

	update();// 触发绘图事件
}

void KToolButton::leaveEvent(QEvent* event)
{
	if (!isChecked())
		m_pSvgRenderer->load(":/icons/normal/" + m_iconName);

	setCursor(Qt::ArrowCursor);
	update();
}


void KToolButton::mouseReleaseEvent(QMouseEvent* e)
{
	setChecked(true);// 设置当前按钮为选中状态
}


void KToolButton::onToggledButton(bool checked)
{
	if (isChecked())
	{
		m_pSvgRenderer->load(":/icons/selected/" + m_iconName);
		KGlobalData::getGlobalDataIntance()->setDrawFlag(m_drawFlag);	// 点击button之后修改全局flag, 表示当前正在执行的绘图事件
	}
	else
		// 重复点击不会撤销当前button设置
		m_pSvgRenderer->load(":/icons/normal/" + m_iconName);
	update();
}

