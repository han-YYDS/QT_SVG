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
    // 垂直布局
    m_pVLayout = new QVBoxLayout(this);
    m_pVLayout->setSpacing(0);
    m_pVLayout->setMargin(0);

    // 水平布局
    m_pHLayout = new QHBoxLayout(this);
    m_pHLayout->setSpacing(0);

    // 工具栏, 主窗口, 画布参数栏
    m_pToolBar = new KToolBar(this);
    m_pSvgMainWin = new KSvgMainWindow(this);
    m_pCanvasParamsBar = new KCanvasParamsBar(QStringLiteral("画布"),this);
    KGlobalData::getGlobalDataIntance()->setCanvasParamBar(m_pCanvasParamsBar);

    m_pHLayout->addWidget(m_pToolBar);
    m_pHLayout->addWidget(m_pSvgMainWin);
    m_pHLayout->addWidget(m_pCanvasParamsBar);

    m_pVLayout->addLayout(m_pHLayout);
    

    setLayout(m_pVLayout);
 }

void KMainWindow::initConnection()
{
    // 画布编辑器 --> 画布
    // 宽度编辑器
    (void)connect(m_pCanvasParamsBar->m_pWidthBox->m_pParamEdit, &QLineEdit::editingFinished, this, &KMainWindow::validateCanvasWidth);
    // 高度编辑器
    (void)connect(m_pCanvasParamsBar->m_pHeightBox->m_pParamEdit, &QLineEdit::editingFinished, this, &KMainWindow::validateCanvasHeight);
    // 颜色编辑器
    (void)connect(m_pCanvasParamsBar->m_pColorBox, &KColorBox::pickedColor, this, &KMainWindow::validateCanvasColor);

    // 选中图形 --> 编辑器
    (void)connect(m_pSvgMainWin->m_pCanvas, &KCanvas::selected, this, & KMainWindow::onHandleSeleted);

    // 编辑器 --> 选中图形
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
        // 释放选择
        return;
    }
    
    // 选择图形, 反映至box上

    if (false) // 如果是text对象
    {
        
        return;
    }



    // 如果是闭合图形对象
    int linewidth = selectedShape->getShapeLineWidth();
    m_pCanvasParamsBar->m_pShapeLineWidthBox->m_pParamEdit->setText(QString::number(linewidth));

	QString linecolor= selectedShape->getShapeLineColor();
    m_pCanvasParamsBar->m_pShapeLineColorBox->setButtonColor(linecolor);

	QString shapeColor = selectedShape->getShapeColor();
	m_pCanvasParamsBar->m_pShapeColorBox->setButtonColor(shapeColor);

}


//  使box输入的属性生效
void KMainWindow::validateCanvasWidth()
{
	qint32 oldHeight = m_pSvgMainWin->m_pCanvas->height();
	qint32 oldWidth = m_pSvgMainWin->m_pCanvas->width();
	qint32 newWidth = m_pCanvasParamsBar->m_pWidthBox->m_pParamEdit->text().toInt();

	// 同步至全局
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

    // 同步至全局
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
	// 同步
    m_pSvgMainWin->m_pCanvas->setColor(newColor);
    m_pSvgMainWin->m_pCanvas->setStyleSheet(QString("background-color:%1").arg(newColor));

    // undo
    KGlobalData::getGlobalDataIntance()->pushUndo(new OpCanvasPara(KCanvasParaType::Color, newColor, oldColor));
}



void KMainWindow::validateShapeLineWidth()
{
    // 取出线宽
    qint32 width = m_pCanvasParamsBar->m_pShapeLineWidthBox->m_pParamEdit->text().toInt();
    KGlobalData::getGlobalDataIntance()->setLineWidth(width);

    // 设置当前图形的线宽
    for (auto shape : m_pSvgMainWin->m_pCanvas->getSelected())
    {
        if (shape != Q_NULLPTR)
        {
            int oldwidth = shape->getShapeLineWidth();
            shape->setShapeLineWidth(width);
            KGlobalData::getGlobalDataIntance()->pushUndo(new OpShapePara(shape, KShapeParaType::LineWidth, width, oldwidth)); // 逆向操作时将para设为old (最后一个参数即为exec所设内容)
        }
    }
}

void KMainWindow::validateShapeLineColor()
{
	// 取出颜色
    QString color = KGlobalData::getGlobalDataIntance()->getLineColor();

	// 设置当前图形的颜色
    for (auto shape : m_pSvgMainWin->m_pCanvas->getSelected())
    {
        if (shape != Q_NULLPTR)
        {
            QString oldcolor = shape->getShapeLineColor();
            shape->setShapeLineColor(color);
            KGlobalData::getGlobalDataIntance()->pushUndo(new OpShapePara(shape, KShapeParaType::LineColor, color, oldcolor)); // 逆向操作时将para设为old (最后一个参数即为exec所设内容)
        }
    }
}

void KMainWindow::validateShapeLineType()
{
	// 取出线型
	Qt::PenStyle style = KGlobalData::getGlobalDataIntance()->getShapeLineType();

	// 设置当前图形的线型
    for (auto shape : m_pSvgMainWin->m_pCanvas->getSelected())
    {
        if (shape != Q_NULLPTR)
        {
            Qt::PenStyle oldstyle = shape->getShapeLineType();
            shape->setShapeLineType(style);
            KGlobalData::getGlobalDataIntance()->pushUndo(new OpShapePara(shape, KShapeParaType::LineType, QVariant::fromValue(style), QVariant::fromValue(oldstyle))); // 逆向操作时将para设为old (最后一个参数即为exec所设内容)
        }
    }
}

void KMainWindow::validateShapeColor()
{
	// 取出颜色
    QString color = KGlobalData::getGlobalDataIntance()->getShapeColor();
    qDebug() << "validateShapeColor color " << color;
	// 设置当前图形的颜色

    for (auto shape : m_pSvgMainWin->m_pCanvas->getSelected())
    {
        if (shape != Q_NULLPTR)
        {
            QString oldColor = shape->getShapeColor();
            shape->setShapeColor(color);
            KGlobalData::getGlobalDataIntance()->pushUndo(new OpShapePara(shape, KShapeParaType::ShapeColor, color, oldColor)); // 逆向操作时将para设为old (最后一个参数即为exec所设内容)
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
