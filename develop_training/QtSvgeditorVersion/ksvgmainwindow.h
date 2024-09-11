#ifndef __K_SVG_MAINWINDOW_H_
#define __K_SVG_MAINWINDOW_H_

#include <QVBoxLayout>
#include <QScrollArea>
#include <QMenuBar>
#include "kcanvas.h"

class KSvgMainWindow : public QScrollArea
{
	Q_OBJECT
	friend class KMainWindow;

public:
	KSvgMainWindow(QWidget *parent);
	~KSvgMainWindow();
	void setCustorStyle(KGlobalData::KDrawFlag drawFlag);

	void enterEvent(QEvent* event) override;
	void wheelEvent(QWheelEvent* event) override;
	void contextMenuEvent(QContextMenuEvent* event) override;

	bool isPressCtrl = false;
	QString m_color;
private:
	
	QVBoxLayout* layout = new QVBoxLayout();

	QMenuBar *m_pMenubar;
	QMenu* m_pFileMenu; // ÎÄ¼þ²Ëµ¥

	KCanvas *m_pCanvas;
	QAction* actionSelectAll;
	QAction* actionCut;
	QAction* actionCopy;
	QAction* actionPaste;
	QAction* actionDelete;
	QAction* actionRedo;
	QAction* actionUndo;

	QAction* actionTop;
	QAction* actionBottom;
	
	
	QAction* createAction;
	QAction* openAction;
	QAction* saveAction;
	QAction* exportAction;

private slots:
	void actionSelectAllTriggered();
	void actionCutTriggered();
	void actionCopyTriggered();
	void actionPasteTriggered();
	void actionDeleteTriggered();
	void actionRedoTriggered();
	void actionUndoTriggered();

	void actionTopTriggered();
	void actionBottomTriggered();

	void openFile();
	void createFile();
	void exportFile();
	void saveFile();


};

#endif
