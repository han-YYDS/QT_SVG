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
	virtual void drawShape(QPaintDevice* parent = Q_NULLPTR) override; // �ػ�
	virtual void output() override;
	virtual void drawRect(QPaintDevice* parent = Q_NULLPTR); // ���
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
	QPoint m_cursorPosition;	// ���
	bool m_isEditing;			// �༭

	QRect m_textRect;			// ���δ�С(��������)
	int m_fontSize;				// �����С
	QFont m_font;				// ����
	QString m_fontFamily;		// ��������
};

#endif //!__K_TEXT_H__
