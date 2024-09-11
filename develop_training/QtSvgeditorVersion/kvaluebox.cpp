#include "kvaluebox.h"
#include <QRegExpValidator>

KValueBox::KValueBox(const QString &title,const int val,QWidget *parent)
	: KParamBox(title,parent)
	, m_pParamEdit(Q_NULLPTR)
{
	// 编辑器
	m_pParamEdit = new QLineEdit(this);
	m_pParamEdit->setText(QString::number(val));	// 初始值
	m_pParamEdit->setAlignment(Qt::AlignHCenter | Qt::AlignTop); // 文本是水平居中和顶端对齐
	m_pParamEdit->setStyleSheet("background-color:#3f3f3c; font-size:20px; color:#4f80ff; font-family: Microsoft YaHei; font-weight: bold; border-width:0; border-style:outset");
	m_pParamEdit->setMaxLength(4); // 设置最大字符数量
	/*
	[1-9] :匹配 1 到 9 任意数字
	+ : 匹配 + 号前面的字符 一次或者多次
	$ : 以前面匹配的字符结尾

	[0-9] :匹配 0 到 9 任意数字
	匹配任意一个由两位或更多数字组成的正整数
*/
	QRegExp reg("[1-9][0-9]+$"); // 正则表达式
	QValidator* validator = new QRegExpValidator(reg, m_pParamEdit); // 正则表达式验证器, 应该是对当前输入文本有一个过滤
	m_pParamEdit->setValidator(validator); // 设置正则表达式
	m_pVLayout->addWidget(m_pParamEdit);// 加入到布局中 m_pVLayout 设置在父类中
}

KValueBox::~KValueBox()
{
}

// 在 QLineEdit 获得焦点时自动选择所有文本来增强用户体验。这样用户可以更方便地编辑和替换文本
void KValueBox::focusInEvent(QFocusEvent* event)
{
	m_pParamEdit->selectAll();// 全选文本
}
