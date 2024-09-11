#ifndef __K_MAINWINDOW_H_
#define __K_MAINWINDOW_H_

#include <QtWidgets/QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QKeyEvent>
#include "ksvgmainwindow.h"
#include "ktoolbar.h"
#include "kcanvasparamsbar.h"

#include "ui_kmainwindow.h"

class KMainWindow : public QWidget
{
    Q_OBJECT

public:
    KMainWindow(QWidget *parent = Q_NULLPTR);
    void initWindows();
    void initConnection();
  
	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;
private slots:
    void validateCanvasWidth();
    void validateCanvasColor();
    void validateCanvasHeight();

    void onHandleSeleted(KShape* selectedShape);
    void validateShapeLineWidth();
    void validateShapeLineColor();
    void validateShapeLineType();
    void validateShapeColor();
private:
    Ui::KMainWindowClass ui;
    QVBoxLayout *m_pVLayout;
    QHBoxLayout *m_pHLayout;
    KToolBar *m_pToolBar;
    KSvgMainWindow *m_pSvgMainWin;
    KCanvasParamsBar *m_pCanvasParamsBar;
};

#endif
