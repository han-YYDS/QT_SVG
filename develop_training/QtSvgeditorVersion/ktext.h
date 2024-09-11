#ifndef __K_TEXT_H__
#define __K_TEXT_H__

#include "kshape.h"

class KText : public KShape
{
	Q_OBJECT

public:
	KText(QObject* parent = Q_NULLPTR);
	KText(const QString& text, const QString& fontFamily, int fontSize, QObject* parent = Q_NULLPTR);
	KText(const KText& other);
	~KText();
	virtual void drawShape(QPaintDevice* parent = Q_NULLPTR) override; // 重绘
	virtual void output() override;
	virtual void drawRect(QPaintDevice* parent = Q_NULLPTR); // 外框
	virtual KShape* clone() override;
	virtual KShapeType getShapeType() override;

	virtual void setStartPoint(const QPoint& point) override;
	virtual void setEndPoint(const QPoint& point) override;
	QString getCurrentText() const;
	void setCurrentText(const QString& text);

	void setIsEditing(bool isEditing);

	//void setCursorPosition(QPoint);
private:
	QString m_currentText;
	QString m_oldText;
	bool m_hasFocus;
	QPoint m_cursorPosition;	// 光标
	bool m_isEditing;			// 编辑

	QRect m_textRect;			// 矩形大小(物理坐标)
	int m_fontSize;				// 字体大小
	QFont m_font;				// 字体
	QString m_fontFamily;		// 字体类型
};

#endif //!__K_TEXT_H__
