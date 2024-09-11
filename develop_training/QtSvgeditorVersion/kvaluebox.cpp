#include "kvaluebox.h"
#include <QRegExpValidator>

KValueBox::KValueBox(const QString &title,const int val,QWidget *parent)
	: KParamBox(title,parent)
	, m_pParamEdit(Q_NULLPTR)
{
	// �༭��
	m_pParamEdit = new QLineEdit(this);
	m_pParamEdit->setText(QString::number(val));	// ��ʼֵ
	m_pParamEdit->setAlignment(Qt::AlignHCenter | Qt::AlignTop); // �ı���ˮƽ���кͶ��˶���
	m_pParamEdit->setStyleSheet("background-color:#3f3f3c; font-size:20px; color:#4f80ff; font-family: Microsoft YaHei; font-weight: bold; border-width:0; border-style:outset");
	m_pParamEdit->setMaxLength(4); // ��������ַ�����
	/*
	[1-9] :ƥ�� 1 �� 9 ��������
	+ : ƥ�� + ��ǰ����ַ� һ�λ��߶��
	$ : ��ǰ��ƥ����ַ���β

	[0-9] :ƥ�� 0 �� 9 ��������
	ƥ������һ������λ�����������ɵ�������
*/
	QRegExp reg("[1-9][0-9]+$"); // ������ʽ
	QValidator* validator = new QRegExpValidator(reg, m_pParamEdit); // ������ʽ��֤��, Ӧ���ǶԵ�ǰ�����ı���һ������
	m_pParamEdit->setValidator(validator); // ����������ʽ
	m_pVLayout->addWidget(m_pParamEdit);// ���뵽������ m_pVLayout �����ڸ�����
}

KValueBox::~KValueBox()
{
}

// �� QLineEdit ��ý���ʱ�Զ�ѡ�������ı�����ǿ�û����顣�����û����Ը�����ر༭���滻�ı�
void KValueBox::focusInEvent(QFocusEvent* event)
{
	m_pParamEdit->selectAll();// ȫѡ�ı�
}
