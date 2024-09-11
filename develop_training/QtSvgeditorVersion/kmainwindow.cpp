#include "kmainwindow.h"
#include "koperation.h"
#include <QLineEdit>
#include <QPen>
#include <QPainter>
#include <QPaintDevice>

KMainWindow::KMainWindow(QWidget *parent)
    : QWidget(parent)
    ,m_pVLayout(Q_NULLPTR)
    ,m_pHLayout(Q_NULLPTR)
    ,m_pSvgMainWin(Q_NULLPTR)
    ,m_pToolBar(Q_NULLPTR)
{
    ui.setupUi(this);
    initWindows();
    initConnection();
}

void KMainWindow::initWindows()
{
    // ��ֱ����
    m_pVLayout = new QVBoxLayout(this);
    m_pVLayout->setSpacing(0);
    m_pVLayout->setMargin(0);

    // ˮƽ����
    m_pHLayout = new QHBoxLayout(this);
    m_pHLayout->setSpacing(0);

    // ������, ������, ����������
    m_pToolBar = new KToolBar(this);
    m_pSvgMainWin = new KSvgMainWindow(this);
    m_pCanvasParamsBar = new KCanvasParamsBar(QStringLiteral("����"),this);
    KGlobalData::getGlobalDataIntance()->setCanvasParamBar(m_pCanvasParamsBar);

    m_pHLayout->addWidget(m_pToolBar);
    m_pHLayout->addWidget(m_pSvgMainWin);
    m_pHLayout->addWidget(m_pCanvasParamsBar);

    m_pVLayout->addLayout(m_pHLayout);
    

    setLayout(m_pVLayout);
 }

void KMainWindow::initConnection()
{
    // �����༭�� --> ����
    // ��ȱ༭��
    (void)connect(m_pCanvasParamsBar->m_pWidthBox->m_pParamEdit, &QLineEdit::editingFinished, this, &KMainWindow::validateCanvasWidth);
    // �߶ȱ༭��
    (void)connect(m_pCanvasParamsBar->m_pHeightBox->m_pParamEdit, &QLineEdit::editingFinished, this, &KMainWindow::validateCanvasHeight);
    // ��ɫ�༭��
    (void)connect(m_pCanvasParamsBar->m_pColorBox, &KColorBox::pickedColor, this, &KMainWindow::validateCanvasColor);

    // ѡ��ͼ�� --> �༭��
    (void)connect(m_pSvgMainWin->m_pCanvas, &KCanvas::selected, this, & KMainWindow::onHandleSeleted);

    // �༭�� --> ѡ��ͼ��
    (void)connect(m_pCanvasParamsBar->m_pShapeLineWidthBox->m_pParamEdit, &QLineEdit::editingFinished, this, &KMainWindow::validateShapeLineWidth);
    (void)connect(m_pCanvasParamsBar->m_pShapeColorBox, &KColorBox::pickedColor, this, &KMainWindow::validateShapeColor);
    (void)connect(m_pCanvasParamsBar->m_pShapeLineColorBox, &KColorBox::pickedColor, this, &KMainWindow::validateShapeLineColor);
    (void)connect(m_pCanvasParamsBar->m_pShapeLineTypeBox, &KShapeLineTypeBox::pickedItem, this, &KMainWindow::validateShapeLineType);
}

void KMainWindow::onHandleSeleted(KShape* selectedShape)
{
    // qDebug() << "onHandleSeleted";
    if (selectedShape == Q_NULLPTR)
    {
        // �ͷ�ѡ��
        return;
    }
    
    // ѡ��ͼ��, ��ӳ��box��

    if (false) // �����text����
    {
        
        return;
    }



    // ����Ǳպ�ͼ�ζ���
    int linewidth = selectedShape->getShapeLineWidth();
    m_pCanvasParamsBar->m_pShapeLineWidthBox->m_pParamEdit->setText(QString::number(linewidth));

	QString linecolor= selectedShape->getShapeLineColor();
    m_pCanvasParamsBar->m_pShapeLineColorBox->setButtonColor(linecolor);

	QString shapeColor = selectedShape->getShapeColor();
	m_pCanvasParamsBar->m_pShapeColorBox->setButtonColor(shapeColor);

}


//  ʹbox�����������Ч
void KMainWindow::validateCanvasWidth()
{
	qint32 oldHeight = m_pSvgMainWin->m_pCanvas->height();
	qint32 oldWidth = m_pSvgMainWin->m_pCanvas->width();
	qint32 newWidth = m_pCanvasParamsBar->m_pWidthBox->m_pParamEdit->text().toInt();

	// ͬ����ȫ��
	KGlobalData::getGlobalDataIntance()->setCanvasWidth(newWidth);
	m_pSvgMainWin->m_pCanvas->resize(newWidth, oldHeight);
    qDebug() << "[DEBUG] validateCanvasWidth newidth = " << newWidth << " oldHeiht" << oldHeight << " oldiwdth" << oldWidth;
	// undo
	KGlobalData::getGlobalDataIntance()->pushUndo(new OpCanvasPara(KCanvasParaType::Width, newWidth, oldWidth));
}

void KMainWindow::validateCanvasHeight()
{
    qint32 oldHeight = m_pSvgMainWin->m_pCanvas->height();
    qint32 oldWidth = m_pSvgMainWin->m_pCanvas->width();
    qint32 newHeight = m_pCanvasParamsBar->m_pHeightBox->m_pParamEdit->text().toInt();

    // ͬ����ȫ��
    KGlobalData::getGlobalDataIntance()->setCanvasHeight(newHeight);    
	m_pSvgMainWin->m_pCanvas->resize(oldWidth, newHeight);

    // undo
    KGlobalData::getGlobalDataIntance()->pushUndo(new OpCanvasPara(KCanvasParaType::Height, newHeight, oldHeight));
}

void KMainWindow::validateCanvasColor()
{
    
    QString newColor = KGlobalData::getGlobalDataIntance()->getCanvasColor();
    QString oldColor = m_pSvgMainWin->m_pCanvas->getColor();
    qDebug() << "validateCanvasColor newcolor is " << newColor << "oldcolor " << oldColor;
	// ͬ��
    m_pSvgMainWin->m_pCanvas->setColor(newColor);
    m_pSvgMainWin->m_pCanvas->setStyleSheet(QString("background-color:%1").arg(newColor));

    // undo
    KGlobalData::getGlobalDataIntance()->pushUndo(new OpCanvasPara(KCanvasParaType::Color, newColor, oldColor));
}



void KMainWindow::validateShapeLineWidth()
{
    // ȡ���߿�
    qint32 width = m_pCanvasParamsBar->m_pShapeLineWidthBox->m_pParamEdit->text().toInt();
    KGlobalData::getGlobalDataIntance()->setLineWidth(width);

    // ���õ�ǰͼ�ε��߿�
    for (auto shape : m_pSvgMainWin->m_pCanvas->getSelected())
    {
        if (shape != Q_NULLPTR)
        {
            int oldwidth = shape->getShapeLineWidth();
            shape->setShapeLineWidth(width);
            KGlobalData::getGlobalDataIntance()->pushUndo(new OpShapePara(shape, KShapeParaType::LineWidth, width, oldwidth)); // �������ʱ��para��Ϊold (���һ��������Ϊexec��������)
        }
    }
}

void KMainWindow::validateShapeLineColor()
{
	// ȡ����ɫ
    QString color = KGlobalData::getGlobalDataIntance()->getLineColor();

	// ���õ�ǰͼ�ε���ɫ
    for (auto shape : m_pSvgMainWin->m_pCanvas->getSelected())
    {
        if (shape != Q_NULLPTR)
        {
            QString oldcolor = shape->getShapeLineColor();
            shape->setShapeLineColor(color);
            KGlobalData::getGlobalDataIntance()->pushUndo(new OpShapePara(shape, KShapeParaType::LineColor, color, oldcolor)); // �������ʱ��para��Ϊold (���һ��������Ϊexec��������)
        }
    }
}

void KMainWindow::validateShapeLineType()
{
	// ȡ������
	Qt::PenStyle style = KGlobalData::getGlobalDataIntance()->getShapeLineType();

	// ���õ�ǰͼ�ε�����
    for (auto shape : m_pSvgMainWin->m_pCanvas->getSelected())
    {
        if (shape != Q_NULLPTR)
        {
            Qt::PenStyle oldstyle = shape->getShapeLineType();
            shape->setShapeLineType(style);
            KGlobalData::getGlobalDataIntance()->pushUndo(new OpShapePara(shape, KShapeParaType::LineType, QVariant::fromValue(style), QVariant::fromValue(oldstyle))); // �������ʱ��para��Ϊold (���һ��������Ϊexec��������)
        }
    }
}

void KMainWindow::validateShapeColor()
{
	// ȡ����ɫ
    QString color = KGlobalData::getGlobalDataIntance()->getShapeColor();
    qDebug() << "validateShapeColor color " << color;
	// ���õ�ǰͼ�ε���ɫ

    for (auto shape : m_pSvgMainWin->m_pCanvas->getSelected())
    {
        if (shape != Q_NULLPTR)
        {
            QString oldColor = shape->getShapeColor();
            shape->setShapeColor(color);
            KGlobalData::getGlobalDataIntance()->pushUndo(new OpShapePara(shape, KShapeParaType::ShapeColor, color, oldColor)); // �������ʱ��para��Ϊold (���һ��������Ϊexec��������)
        }
    }

}


void KMainWindow::keyPressEvent(QKeyEvent* event) {
	if (event->key() == Qt::Key_Control) {
		KGlobalData::getGlobalDataIntance()->m_isPressCtrl = true;
		// qDebug() << "ctrl perss";
	}

	QWidget::keyPressEvent(event);
}

void KMainWindow::keyReleaseEvent(QKeyEvent* event) {
	if (event->key() == Qt::Key_Control) {
		KGlobalData::getGlobalDataIntance()->m_isPressCtrl = false;
	}
	QWidget::keyReleaseEvent(event);
}
