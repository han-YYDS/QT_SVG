#include "kcombobox.h"
#include <QComboBox>
KComboBox::KComboBox(const QString &title,QWidget *parent)
	: KParamBox(title,parent)
	, m_pComboBox(Q_NULLPTR)
{
	// ����combox��ʽ

	m_pComboBox = new QComboBox(this);
	m_pComboBox->setStyleSheet("background-color:#3f3f3c; font-size:18px; color:#4f80ff; font-family: Microsoft YaHei; font-weight: bold; border-width:0; border-style:outset");
	m_pVLayout->addWidget(m_pComboBox);
	// (void)connect(m_pComboBox, &QComboBox::currentIndexChanged,this, &KComboBox::pickedItem); // ѡ��item
	(void)connect(m_pComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &KComboBox::pickedItem);

	(void)connect(m_pComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &KComboBox::onComboBoxIndexChanged);

	// �ۺ�������
	
}

KComboBox::~KComboBox()
{
}

void KComboBox::setItem(int index)
{

}

void KComboBox::onComboBoxIndexChanged(int index) {
	qDebug() << "Selected index:" << index;
	setItem(index);
}