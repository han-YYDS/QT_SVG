#include "ksvgmainwindow.h"
#include <QApplication>
#include <QWheelEvent>
#include <QMenu>
#include <QFileDialog>
#include <QSvgGenerator>
#include <QMessageBox>
#include "svgmanager.h"
#include "kglobaldata.h"
#include "koperation.h"
#include <QStack>

KSvgMainWindow::KSvgMainWindow(QWidget *parent)
	: QScrollArea(parent)
	, m_pCanvas(Q_NULLPTR)
{
	setFrameShape(QFrame::NoFrame); // �����ޱ߿�

	// �����ݳ�����ʾ����ʱ���ֹ�����
	setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setStyleSheet("QScrollArea{background-color:#3f3f3c;}");
	setAlignment(Qt::AlignCenter);
	
	m_pCanvas = new KCanvas;
	KGlobalData::getGlobalDataIntance()->setCanvas(m_pCanvas);
	setWidget(m_pCanvas);// ���õ�ǰ�ؼ�Ϊ����������ӿؼ�

	// �����˵��Ͳ˵���
	

	// �˵���
	m_pMenubar = new QMenuBar();
	m_pFileMenu = new QMenu(QString::fromLocal8Bit("�ļ�"), m_pMenubar);
	QAction* createAction = new QAction(QString::fromLocal8Bit("�½��ļ�"), m_pMenubar);
	QAction* openAction = new QAction(QString::fromLocal8Bit("��SVG"), m_pMenubar);
	QAction* saveAction = new QAction(QString::fromLocal8Bit("����"), m_pMenubar);
	QAction* exportAction = new QAction(QString::fromLocal8Bit("����PNG"), m_pMenubar);

	m_pFileMenu->addAction(createAction);
	m_pFileMenu->addAction(openAction);
	m_pFileMenu->addAction(saveAction);
	m_pFileMenu->addAction(exportAction);
	m_pMenubar->addMenu(m_pFileMenu);

	m_pMenubar->setStyleSheet(
		"QMenuBar { background-color: #3d3d3d; color: white; }"  // ���ò˵����ı�����ɫ��������ɫ
		"QMenu { background-color: #3CB371; color: white; }"     // ���ò˵���ı�����ɫ��������ɫ
		"QMenu::item:selected { background-color: #4CAF50; }"    // ����ѡ�в˵���ı�����ɫ
	);


	(void)connect(createAction, &QAction::triggered, this, &KSvgMainWindow::createFile);
	(void)connect(openAction, &QAction::triggered, this, &KSvgMainWindow::openFile);
	(void)connect(saveAction, &QAction::triggered, this, &KSvgMainWindow::saveFile);
	(void)connect(exportAction, &QAction::triggered, this, &KSvgMainWindow::exportFile);

	createAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
	openAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
	saveAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
	exportAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_E));



	// ���ò���
	QVBoxLayout* layout = new QVBoxLayout();
	layout->setMenuBar(m_pMenubar); // ���˵������õ�������
	setLayout(layout);


	// �Ҽ���ݼ�
	actionSelectAll = new QAction(QString::fromLocal8Bit("ȫѡ"), this);
	actionCut = new QAction(QString::fromLocal8Bit("����"), this);
	actionCopy = new QAction(QString::fromLocal8Bit("����"), this);
	actionPaste = new QAction(QString::fromLocal8Bit("ճ��"), this);
	actionDelete = new QAction(QString::fromLocal8Bit("ɾ��"), this);
	actionRedo = new QAction(QString::fromLocal8Bit("����"), this);
	actionUndo = new QAction(QString::fromLocal8Bit("����"), this);

	actionTop = new QAction(QString::fromLocal8Bit("���ڶ���"), this);
	actionBottom = new QAction(QString::fromLocal8Bit("���ڵײ�"), this);


	actionSelectAll->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_A));
	actionCut->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_X));
	actionCopy->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));
	actionPaste->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_V));
	actionDelete->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_D));
	actionRedo->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Y));
	actionUndo->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z));



	actionSelectAll->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	actionCut->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	actionCopy->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	actionPaste->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	actionDelete->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	actionRedo->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	actionUndo->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	actionTop->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	actionBottom->setShortcutContext(Qt::WidgetWithChildrenShortcut);
	
	addAction(actionSelectAll);
	addAction(actionCut);
	addAction(actionCopy);
	addAction(actionPaste);
	addAction(actionDelete);
	addAction(actionRedo);
	addAction(actionUndo);
	addAction(actionTop);
	addAction(actionBottom);

	(void)connect(actionSelectAll, &QAction::triggered, this, &KSvgMainWindow::actionSelectAllTriggered);
	(void)connect(actionCut, &QAction::triggered, this, &KSvgMainWindow::actionCutTriggered);
	(void)connect(actionCopy, &QAction::triggered, this, &KSvgMainWindow::actionCopyTriggered);
	(void)connect(actionPaste, &QAction::triggered, this, &KSvgMainWindow::actionPasteTriggered);
	(void)connect(actionDelete, &QAction::triggered, this, &KSvgMainWindow::actionDeleteTriggered);
	(void)connect(actionRedo, &QAction::triggered, this, &KSvgMainWindow::actionRedoTriggered);
	(void)connect(actionUndo, &QAction::triggered, this, &KSvgMainWindow::actionUndoTriggered);
	(void)connect(actionTop, &QAction::triggered, this, &KSvgMainWindow::actionTopTriggered);
	(void)connect(actionBottom, &QAction::triggered, this, &KSvgMainWindow::actionBottomTriggered);
}

KSvgMainWindow::~KSvgMainWindow()
{
}

void KSvgMainWindow::setCustorStyle(KGlobalData::KDrawFlag drawFlag)
{
	switch (drawFlag)
	{
	case KGlobalData::KDrawFlag::PenDrawFlag:
		setCursor(Qt::ArrowCursor);
		break;
	case KGlobalData::KDrawFlag::LineDrawFlag:
	case KGlobalData::KDrawFlag::RectDrawFlag:
	case KGlobalData::KDrawFlag::CircleDrawFlag:

		setCursor(Qt::CrossCursor);
		break;

	case KGlobalData::KDrawFlag::TextDrawFlag:
		setCursor(Qt::IBeamCursor);
		break;
	default:
		setCursor(Qt::ArrowCursor);
		break;
	}
}

void KSvgMainWindow::enterEvent(QEvent* event)
{
	KGlobalData::KDrawFlag flag = KGlobalData::getGlobalDataIntance()->getDrawFlag();
	setCustorStyle(flag);
	QScrollArea::enterEvent(event);
}

void KSvgMainWindow::wheelEvent(QWheelEvent* event) // ���ֿؼ�
{
	if (QApplication::keyboardModifiers() == Qt::ControlModifier)
	{
		qreal scale = 1.0;
		if (event->delta() > 0)
			scale = KGlobalData::getGlobalDataIntance()->getCanvasScale() + 0.05;
		else
			scale = KGlobalData::getGlobalDataIntance()->getCanvasScale() - 0.05;

		qint32 width = KGlobalData::getGlobalDataIntance()->getCanvasWidth();
		qint32 height = KGlobalData::getGlobalDataIntance()->getCanvasHeight();
		KGlobalData::getGlobalDataIntance()->setCanvaScale(scale);
		m_pCanvas->resize(width * scale, height * scale);
	}
}

void KSvgMainWindow::contextMenuEvent(QContextMenuEvent* event) {
	QMenu menu(this);

	// Create actions for the menu
	// Add actions to the menu
	menu.addAction(actionSelectAll);
	menu.addAction(actionCut);
	menu.addAction(actionCopy);
	menu.addAction(actionPaste);
	menu.addAction(actionDelete);
	menu.addAction(actionRedo);
	menu.addAction(actionUndo);
	menu.addAction(actionTop);
	menu.addAction(actionBottom);


	// Show the context menu at the position of the cursor
	menu.exec(event->globalPos());
}


void KSvgMainWindow::actionSelectAllTriggered()
{
	// 1. ��selected����Ϊȫ��ͼ��, ����ȫѡfalg
	m_pCanvas->selectAll();

	// 2. 

}


void KSvgMainWindow::actionCutTriggered()
{
	// 1. ��ѡ��������ݽ������
	m_pCanvas->copySeleted();

	// 2. ��ѡ��������ݽ���ɾ��
	m_pCanvas->deleteSelected();
}

void KSvgMainWindow::actionCopyTriggered()
{
	// 1. ��ѡ��������ݽ������
	m_pCanvas->copySeleted();
}

void KSvgMainWindow::actionPasteTriggered()
{
	// 1. �����а��е����������shapelist��
	m_pCanvas->paste();
}

void KSvgMainWindow::actionDeleteTriggered()
{
	// 1. ɾ��ѡ������
	m_pCanvas->deleteSelected();
}


// 1. ����
void KSvgMainWindow::actionRedoTriggered()
{
	qDebug() << "[DEBUG] REDO undo size = " << KGlobalData::getGlobalDataIntance()->getUndo().size();
	qDebug() << "[DEBUG] REDO redo size = " << KGlobalData::getGlobalDataIntance()->getRedo().size();

	if (KGlobalData::getGlobalDataIntance()->getRedo().size() == 0)
		return;
	// 1. ��rndoStack��ջ
	Operation* op = KGlobalData::getGlobalDataIntance()->popRedo();

	// 2. exec
	op->dump();
	op->exec();
	m_pCanvas->update();

	// 3. redo
	KGlobalData::getGlobalDataIntance()->pushUndo(op->reverse());
}

// 1.����
void KSvgMainWindow::actionUndoTriggered()
{
	qDebug() << "[DEBUG] UNDO undo size = " << KGlobalData::getGlobalDataIntance()->getUndo().size();
	qDebug() << "[DEBUG] UNDO redo size = " << KGlobalData::getGlobalDataIntance()->getRedo().size();

	if (KGlobalData::getGlobalDataIntance()->getUndo().size() == 0)
		return;

	// 1. ��undoStack��ջ
	Operation* op = KGlobalData::getGlobalDataIntance()->popUndo();

	// 2. exec
	op->exec();
	m_pCanvas->update();

	// 3. redo
	KGlobalData::getGlobalDataIntance()->pushRedo(op->reverse());
}

void KSvgMainWindow::actionTopTriggered()
{
	auto canvas = KGlobalData::getGlobalDataIntance()->getCanvas();
	if (canvas->getCurrentShape() == Q_NULLPTR)
	{
		return;
	}
	canvas->removeFromShapeList(canvas->getCurrentShape());
	canvas->ShapeListPrepend(canvas->getCurrentShape());
}

void KSvgMainWindow::actionBottomTriggered()
{
	
	auto canvas = KGlobalData::getGlobalDataIntance()->getCanvas();
	if (canvas->getCurrentShape() == Q_NULLPTR)
	{
		return;
	}
	canvas->removeFromShapeList(canvas->getCurrentShape());
	canvas->ShapeListAppend(canvas->getCurrentShape());
}


void KSvgMainWindow::openFile()
{
	// ���ļ�ѡ��Ի���
	QString fileName = QFileDialog::getOpenFileName(nullptr, "Open SVG File", "", "SVG Files (*.svg)");

	// ����svgpaser��xml���н���
	SvgManager parser;
	parser.parseFile(fileName);
	m_pCanvas->setShapeList(parser.loadElements());
	qDebug() << "[DEBUG] size = " << m_pCanvas->getShapeList().size();
	for (auto & it : m_pCanvas->getShapeList())
	{
		qDebug() << it->dump();
	}
	update();
}

// �½�����
void KSvgMainWindow::createFile()
{
	// 1. �������л���
	saveFile();

	// 2. ���û���
	KCanvas* newC = new KCanvas;
	std::swap(m_pCanvas, newC);
	setWidget(m_pCanvas);
	// delete newC;
}

// ����ΪPNG
void KSvgMainWindow::exportFile()
{
	QString fileName = QFileDialog::getSaveFileName(this, "Save PNG File", "", "PNG Files (*.png);;All Files (*)");
	if (fileName.isEmpty()) {
		return;  // �û�ȡ���˶Ի���
	}

	QPixmap pixmap(m_pCanvas->width(), m_pCanvas->height());
	QPainter* painter = KGlobalData::getGlobalDataIntance()->getPainter();
	painter->begin(&pixmap);
	for (auto it : m_pCanvas->getShapeList())
	{
		it->output();
	}
	painter->end();

	// ����Ϊ PNG �ļ�
	if (pixmap.save(fileName, "PNG")) {
		QMessageBox::information(nullptr, "Success", "Image saved successfully!");
	}
	else {
		QMessageBox::warning(nullptr, "Error", "Failed to save image.");
	}
}

void KSvgMainWindow::saveFile()
{
	QString fileName = QFileDialog::getSaveFileName(this, "Save SVG File", "", "SVG Files (*.svg);;All Files (*)");
	if (fileName.isEmpty()) {
		return;  // �û�ȡ���˶Ի���
	}

	// ���� SVG �ļ�
	QSvgGenerator generator;
	generator.setFileName(fileName);
	generator.setSize(size());
	generator.setViewBox(rect());
	generator.setTitle(tr("SVG Example"));
	generator.setDescription(tr("An SVG drawing created by the SvgEditor."));
	QPainter* painter = KGlobalData::getGlobalDataIntance()->getPainter();

	painter->begin(&generator);
	for (auto it : m_pCanvas->getShapeList())
	{
		it->output();
	}
	painter->end();

}

