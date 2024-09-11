#ifndef __K_PARAMS_H_
#define __K_PARAMS_H_

#include <QWidget>
#include <QLabel>
#include <QGridlayout>
#include <QVBoxLayout>

class KParamsBar : public QWidget
{
	Q_OBJECT

public:
	KParamsBar(const QString &title,QWidget *parent);
	~KParamsBar();
protected:
	QGridLayout* m_pGridLayout;

private:
	QVBoxLayout* m_pVBoxLayout;
	QString m_title;
	QLabel* m_pTitleLabel;
};

#endif
