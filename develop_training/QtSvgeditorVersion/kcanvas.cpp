#include "kcanvas.h"
#include "kshapefactory.h"
#include "koperation.h"
#include <QMouseEvent>
#include <QDebug>




KCanvas::KCanvas(QWidget* parent)
	: QWidget(parent)
	, m_pCurrentShape(Q_NULLPTR) // ��ǰ���ڻ��Ƶ�ͼ��
	, m_TransType(KTransType::None) // ��ǰ�ı任����
	, m_isDrawing(false)
	, m_isLPress(false)
	, m_isSelected(false)
{
	setAttribute(Qt::WA_StyledBackground, true);

	// ���ñ�����ɫΪ��ɫ
	setStyleSheet("background-color:#FFFFFF");

	KGlobalData::getGlobalDataIntance()->setCanvasColor("#FFFFFF"); // ����
	resize(KGlobalData::getGlobalDataIntance()->getCanvasWidth(), // ͨ��ȫ�����ø߿�
		KGlobalData::getGlobalDataIntance()->getCanvasHeight());

	setMouseTracking(true);//�������׷��
	setFocusPolicy(Qt::ClickFocus);// ������ȡ����
}

KCanvas::~KCanvas()
{
	for (auto shape : m_pShapeList)
		delete shape;
	m_pShapeList.clear();
}

// ��ͼ�¼�����
void KCanvas::paintEvent(QPaintEvent* event)
{

	// ����ͼ�λ���
	if (!m_pShapeList.isEmpty())
	{
		auto it = m_pShapeList.rbegin();
		while (it != m_pShapeList.rend())
		{
			(*it)->drawShape(this);
			it++;
		}
	}

	// ��ǰ���ڻ���ͼ�ε����
	if (m_pCurrentShape != Q_NULLPTR)
	{
		if (m_isDrawing) // ͼ�λ��Ʊ�־Ϊ true�����ʾ���ڻ��Ƹ�ͼ�Σ���ʱ��Ҫ�������»���
			m_pCurrentShape->drawShape(this);

		if (m_isSelected) // ͼ��ѡ�У������ѡ�п�
			m_pCurrentShape->drawOutLine(this);

		if (m_isEditing) // ���editing,�����
		{
			m_pCurrentShape->drawShape(this);
		}
		
	}

	// ȫѡͼ�ο�ͼ
	if (!m_pSelectedShapeList.empty())
	{
		for (auto shape : m_pSelectedShapeList)
		{
			shape->drawOutLine(this);
		}
	}
}

void KCanvas::mousePressEvent(QMouseEvent* event)
{
	//��ȡ��ǰ��Ҫ���Ƶ�ͼ������
	KGlobalData::KDrawFlag flag = KGlobalData::getGlobalDataIntance()->getDrawFlag();
	
	// ���û�е����ఴť,�򲻻���
	if (flag == KGlobalData::KDrawFlag::NoneDrawFlag)
		return;
	

	if (event->buttons() == Qt::LeftButton)
	{
		m_isLPress = true;

		if (flag == KGlobalData::KDrawFlag::MouseDrawFlag) // button����
		{
			
			m_pCurrentShape = getCurrentShape(event->pos());	// ��ȡ�˴ε�����ѡ��Ķ���
			if (m_pCurrentShape != Q_NULLPTR)					// ����������ͼ��, ��׽�߼�
			{
				// �������δѡ��
				if (std::find(m_pSelectedShapeList.begin(), m_pSelectedShapeList.end(), m_pCurrentShape) == m_pSelectedShapeList.end()) {
					if (!KGlobalData::getGlobalDataIntance()->m_isPressCtrl) // ���û����ctrl
					{
						m_pSelectedShapeList.clear();
					}
					else
					{
						// qDebug() << "ctrl is press";
					}
					m_pSelectedShapeList.append(m_pCurrentShape);	// ����ѡ��
				}
				qreal scale = KGlobalData::getGlobalDataIntance()->getCanvasScale();
				m_lastPos = QPoint(event->pos().x()/scale, event->pos().y() / scale);	// ����move

				m_TransType = getTransType(event->pos());	// ��ȡ�ƶ�����
				m_isSelected = true;						// ����ѡ�б�־
				emit selected(m_pCurrentShape);				// ���ڷ����Ҳ����Կ�ͬ��

				if (m_pCurrentShape->getShapeType() == KShapeType::TextShapeType)
				{
					m_cursorPosition = static_cast<KText*>(m_pCurrentShape)->getCurrentText().size();
				}
			}
			else // δѡ��ͼ��(����հ�)ȡ����׽
			{
				m_isSelected = false;
				m_isSelectedAll = false;
				m_pSelectedShapeList.clear();
				emit selected(Q_NULLPTR);
			}
		}
		else if (flag == KGlobalData::KDrawFlag::TextDrawFlag)
		{
			// ���m_cursorPosition != 0, ���ͷ�����
			if (m_cursorPosition != 0 )
			{
				finishedEditing();
				update();
				return;
			}



			// �����ı���
			m_pCurrentShape = KShapeFactory::createShape(KGlobalData::getGlobalDataIntance()->getDrawFlag());
			assert(m_pCurrentShape != Q_NULLPTR);
			m_isSelected = false;
			m_isEditing = true;
			KText* textShape = static_cast<KText*>(m_pCurrentShape);
			QString text = textShape->getCurrentText();
			m_cursorPosition = 0;
			qreal scale = KGlobalData::getGlobalDataIntance()->getCanvasScale();
			m_pCurrentShape->setStartPoint(event->pos());
			m_pCurrentShape->setEndPoint(event->pos());
			currediting = textShape;
			update();			
		}
		else
		{ 
			m_isSelected = false;
			m_pCurrentShape = KShapeFactory::createShape(KGlobalData::getGlobalDataIntance()->getDrawFlag());
			KGlobalData::getGlobalDataIntance()->pushUndo(new OpDel(m_pCurrentShape));

			if (m_pCurrentShape != Q_NULLPTR)
			{
				m_pCurrentShape->setStartPoint(event->pos());
				m_pCurrentShape->setEndPoint(event->pos());
			}
				
				
		}
	}
	
	update();
}

void KCanvas::mouseMoveEvent(QMouseEvent* event)
{
	// qDebug() << "[DEBUG] mousemove: [" << QString::number(event->pos().x()) << "," << QString::number(event->pos().y()) << "]";
	KGlobalData::KDrawFlag flag = KGlobalData::getGlobalDataIntance()->getDrawFlag();	// ���button���õĵ�ǰ��ͼ�¼�
	
	if (flag == KGlobalData::KDrawFlag::NoneDrawFlag)
		return;

	KTransType transType;
	if (m_pSelectedShapeList.size() > 1) // ��ѡ״̬��,��ǰ״̬�����ƶ�
	{
		transType = KTransType::Contains;
	}
	else
	{
		transType = getTransType(event->pos());	// ʵʱ���²�׽��
	}

	updateCusorStyle(flag, transType); // ��Ⱦ��׽��

	if (event->buttons() != Qt::LeftButton)//bug : �ڲ����������������ѡ��״̬���ƶ������ƶ�ͼ��
			return;


	// ��ǰ��ͼ
	if (flag != KGlobalData::KDrawFlag::MouseDrawFlag)
	{
		if (m_isLPress && !m_isDrawing)
			m_isDrawing = true;
		if (m_pCurrentShape != Q_NULLPTR)
			m_pCurrentShape->setEndPoint(event->pos());
	}

	else // ��ק/�ƶ�
	{
		// ȫѡ
		if (m_pSelectedShapeList.size() > 1) 
		{
			MoveSelectedShape(transType, event->pos());
		}

		// ��ѡ
		if (m_pCurrentShape == Q_NULLPTR)
		{
			return;
		}
		
		if(m_pSelectedShapeList.size() == 1)
		{
			dragMoveShape(transType, event->pos());//�ƶ�ѡ��ͼ��
		}
	}
	update();
}

void KCanvas::mouseReleaseEvent(QMouseEvent* event)
{
	KGlobalData::KDrawFlag flag = KGlobalData::getGlobalDataIntance()->getDrawFlag();

	// �����ɿ������ʱ��
		
	if (m_isLPress) // ���������º��ͷţ����ʾ�ƶ����߻�ͼ���
	{	
		// �����ǰ���ڱ༭,��Ӧ��ִ��ͼ�ι鵵�߼�, ���¾��ο�ͼ������
		if (m_isEditing && flag == KGlobalData::KDrawFlag::TextDrawFlag)
		{
			update();
			return;
		}
		if (flag != KGlobalData::KDrawFlag::MouseDrawFlag) // ��Ϊmousedraw���߼���������ִ�е�,���Բ���Ҫ��release��ʱ����д���
		{
			if (m_pCurrentShape != Q_NULLPTR) // ��ͼ֮����߼�
			{
				if (m_pCurrentShape->isValid())			// �ж�ͼ�ξ����Ƿ���Ч
					m_pCurrentShape->setEndPoint(event->pos());

				m_pShapeList.prepend(m_pCurrentShape);	// ͼ�ι鵵
				m_pCurrentShape = Q_NULLPTR;
				//m_iselected = true;
				//m_isDrawing = false;
					
			}
			
			else // ������ʵ�ǿ�ѡ���߼�
			{
				// TODO: ����delete��һ����ָ��
				// TODO: ����������ӿ�ѡ�߼�
				// delete m_pCurrentShape;
				m_pCurrentShape = Q_NULLPTR;

			}
		}

		m_isLPress = false;
		m_isDrawing = false;
		m_TransType = KTransType::None;
	}

	update();
}



void KCanvas::keyPressEvent(QKeyEvent* event)
{
	KGlobalData::KDrawFlag flag = KGlobalData::getGlobalDataIntance()->getDrawFlag();

	if (m_pCurrentShape == Q_NULLPTR || m_pCurrentShape->getShapeType() != KShapeType::TextShapeType)
	{
		QWidget::keyPressEvent(event);
		return;
	}

	
	KText* textEditShape = static_cast<KText*>(m_pCurrentShape); // ��ǰ���ڻ����ı�
	QString text = textEditShape->getCurrentText();				
	if (event->key() == Qt::Key_Escape || event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) { // esc enter �˳��༭		
		if (text.size() > 0 && !m_pShapeList.contains(m_pCurrentShape)) // �����ͼ
			m_pShapeList.prepend(textEditShape);

		textEditShape->setIsEditing(false);
		m_pCurrentShape = nullptr;
		m_isEditing = false;
	}
	else if (event->key() == Qt::Key_Backspace && !text.isEmpty()) { // �˸�ɾ���ı�
		text.chop(1);
		textEditShape->setCurrentText(text);
		m_cursorPosition = qMax(0, m_cursorPosition - 1); // ���¹��λ��
	}
	else if (event->key() == Qt::Key_Left) {		// ���ͷ�����ƶ����λ��
		m_cursorPosition = qMax(0, m_cursorPosition - 1);
	}
	else if (event->key() == Qt::Key_Right) {		// �Ҽ�ͷ�����ƶ����λ��
		m_cursorPosition = qMin(text.length(), m_cursorPosition + 1);
	}
	else if (!event->text().isEmpty()) {		// �����ı�����
		text.insert(m_cursorPosition, event->text());
		textEditShape->setCurrentText(text);
		m_cursorPosition++;
	}
	update();
	QWidget::keyPressEvent(event);
}

KShape* KCanvas::getCurrentShape(const QPoint &pos)
{
	// ����
	for (QList<KShape*>::iterator it = m_pShapeList.begin();
		it != m_pShapeList.end(); ++it)
	{
		// �ж�pos�Ƿ���shapeRect��
		if ((*it)->getShapeRect().contains(pos))
			return *it;
	}

	return Q_NULLPTR;
}

// ��ק����/�ƶ�
KTransType KCanvas::getTransType(const QPoint& pos)
{
	if (!m_pCurrentShape)
		return KTransType::None;

	// ��ȡͼ�ξ���
	QRect rect = m_pCurrentShape->getShapeRect();

	// ��ȫ�������õ�ǰͼ��

	if (!rect.contains(pos))
		return KTransType::None;
	

	// ͨ������������λ�� �ж��������ƶ����ֲ���ק�ƶ�
	if (qAbs(pos.x() - rect.topLeft().x()) < 5 
			&& qAbs(pos.y() - rect.topLeft().y()) < 5)
		return KTransType::TopLeft;

	if (qAbs(rect.topRight().x() - pos.x()) < 5 
			&& qAbs(pos.y() - rect.topRight().y()) < 5)
		return KTransType::TopRight;

	if (qAbs(rect.bottomRight().x() - pos.x()) < 5 
			&& qAbs(rect.bottomRight().y() - pos.y()) < 5)
		return KTransType::BottomRight;

	if (qAbs(pos.x() - rect.bottomLeft().x()) < 5 
			&& qAbs(rect.bottomLeft().y() - pos.y()) < 5)
		return KTransType::BottomLeft;

	if (qAbs(pos.y() - rect.top()) < 5 
			&& pos.x() > rect.topLeft().x() + 5
			&& pos.x() < rect.topRight().x() - 5)
		return KTransType::Top;

	if (qAbs(rect.right() - pos.x()) < 5
			&& pos.y() > rect.topRight().y() + 5
			&& pos.y() < rect.bottomRight().y() - 5)
		return KTransType::Right;

	if (qAbs(rect.bottom() - pos.y()) < 5 
			&& pos.x() > rect.bottomLeft().x() + 5 
			&& pos.x() < rect.bottomRight().x() - 5)
		return KTransType::Bottom;

	if (qAbs(pos.x() - rect.left()) < 5 
			&& pos.y() > rect.topLeft().y() + 5 
			&& pos.y() < rect.bottomLeft().y() - 5)
		return KTransType::Left;

	return KTransType::Contains;
}

void KCanvas::updateCusorStyle(KGlobalData::KDrawFlag flag, KTransType transType)
{
	// ����Ѿ�ѡ��
	if (m_isSelected
		&& flag == KGlobalData::KDrawFlag::MouseDrawFlag
		&& transType != KTransType::None)
	{
		//TODO���任ͼ��,���ò�ͬ�������ʽ,���ݵ��λ�ã����ò�ͬ�������ʽ
		if (transType == KTransType::TopLeft || transType == KTransType::BottomRight)
			setCursor(Qt::SizeFDiagCursor);
		else if (transType == KTransType::TopRight || transType == KTransType::BottomLeft)
			setCursor(Qt::SizeBDiagCursor);
		else if (transType == KTransType::Top || transType == KTransType::Bottom)
			setCursor(Qt::SizeVerCursor);
		else if (transType == KTransType::Left || transType == KTransType::Right)
			setCursor(Qt::SizeHorCursor);

		else if (transType == KTransType::Contains)
			setCursor(Qt::SizeAllCursor);
	}
	else
		unsetCursor();
}

void KCanvas::dragMoveShape(KTransType transType,const QPoint &ppos)
{
	if (m_pCurrentShape == NULL)
		return;

	/*
		���ƶ������У���Ϊ�����ƶ���ʽ
			(1) �����ƶ�  contains
			(2) ����ĳһ���������ק

	*/
	qreal scale = KGlobalData::getGlobalDataIntance()->getCanvasScale();
	QPoint pos = QPoint(ppos.x() / scale, ppos.y() / scale);

	switch (m_TransType)
	{
	case KTransType::None:
		return;
	case KTransType::TopLeft:
		// ��������Ͻǣ��޸�start
		m_pCurrentShape->moveTopLeft(pos);
		break;
	case KTransType::Top:
		// ����������ƶ����޸�start_y
		m_pCurrentShape->moveTop(pos);
		break;
	case KTransType::TopRight:
		m_pCurrentShape->moveTopRight(pos);
		break;
	case KTransType::Left:
		m_pCurrentShape->moveLeft(pos);
		break;
	case KTransType::Contains: // �����ƶ�
	{
		// m_lastPos Ϊѡ��ʱ���λ�ã����ƶ������в��ϸ�ֵΪǰһ�ε�λ��
		QPoint newpos = pos -  m_lastPos; // ����Ҫ�ƶ���ƫ��
		m_pCurrentShape->move(newpos);
		m_lastPos = pos;
	}
	break;
	case KTransType::Right:
		m_pCurrentShape->moveRight(pos);
		break;
	case KTransType::BottomLeft:
		m_pCurrentShape->moveBottomLeft(pos);
		break;
	case KTransType::Bottom:
		m_pCurrentShape->moveBottom(pos);
		break;
	case KTransType::BottomRight:
		m_pCurrentShape->moveBottomRight(pos);
		break;
	default:
		break;
	}
}

void KCanvas::MoveSelectedShape(KTransType transType, const QPoint& ppos)
{
	// assert(m_isSelectedAll);
	/*
		���ƶ������У���Ϊ�����ƶ���ʽ
			(1) �����ƶ�  contains
			(2) ����ĳһ���������ק

	*/

	qreal scale = KGlobalData::getGlobalDataIntance()->getCanvasScale(); // �����߼�
	QPoint pos = QPoint(ppos.x() / scale, ppos.y() / scale);

	QPoint newpos = pos - m_lastPos; // ����Ҫ�ƶ���ƫ��
	for (auto it : m_pSelectedShapeList)
	{
		// m_lastPos Ϊѡ��ʱ���λ�ã�
		it->move(newpos);
		m_lastPos = pos;
	}
}

void KCanvas::selectAll()
{
	// ��������, ״̬����
	m_pSelectedShapeList = m_pShapeList; // ǳ����, ֻ����ָ��
	m_isSelectedAll = true;

	// update
	update();
}

void KCanvas::copySeleted()
{
	// 1. ��ռ��а�
	for (auto it : m_pClipboard)
	{
		delete it;
	}
	m_pClipboard.clear();


	// 2. ��selected���������
	for (auto it : m_pSelectedShapeList)
	{
		m_pClipboard.append(it->clone());
	}
}

void KCanvas::paste()
{

	// 1.��һ��ƫ��
	for (auto it : m_pClipboard)
	{
		it->move(QPoint(20,20));

	}

	// 2. �����а��е����ݿ�����shaplist
	for (auto it : m_pClipboard)
	{
		m_pShapeList.append(it->clone());
	}

	update();
}

void KCanvas::deleteSelected()
{
	// 1. ������shape��ɾ��ѡ��ͼ��
	QList<KShape*> ret;


	// 1. shapelist��Ų��deletestack,  ������add��redostack
	for (int i = m_pShapeList.size() -1; i >= 0; i--)
	{
		if (! m_pSelectedShapeList.contains(m_pShapeList[i]))
			continue;
		KShape* shape = m_pShapeList.takeAt(i);
		m_pDeleteStack.push(shape);

		Operation* add = new OpAdd(shape);
		KGlobalData::getGlobalDataIntance()->pushUndo(add);
	}


	qDebug() << "delete size = " << m_pDeleteStack.size();
	qDebug() << "shapelist size = " << m_pShapeList.size();
	qDebug() << "selected size = " << m_pSelectedShapeList.size();


	for (auto it : ret)
	{
		it->drawOutLine(this);
	}

	// 2. ���ѡ��
	m_pSelectedShapeList.clear();
	m_pCurrentShape = Q_NULLPTR;

	// 3.
	update();
}

void KCanvas::finishedEditing()
{
	KText* textEditShape = currediting;
	QString text = textEditShape->getCurrentText();
	if (text.size() > 0 && !m_pShapeList.contains(m_pCurrentShape)) // �����ͼ
		m_pShapeList.prepend(textEditShape);
	textEditShape->setIsEditing(false);
	m_pCurrentShape = nullptr;
	m_isEditing = false;
	m_cursorPosition = 0;
}

KShape* KCanvas::deletePop()
{
	auto it = m_pDeleteStack.top();
	m_pDeleteStack.pop();
	return it;
}

void KCanvas::deletePush(KShape* shape)
{
	m_pDeleteStack.push(shape);
}

void KCanvas::ShapeListPrepend(KShape* shape)
{
	m_pShapeList.prepend(shape);
}

void KCanvas::ShapeListAppend(KShape* shape)
{
	m_pShapeList.append(shape);
}

void KCanvas::removeFromShapeList(KShape* shape)
{
	for (int i = m_pShapeList.size() - 1; i >= 0; i--)
	{
		if (m_pShapeList[i] == shape)
		{
			m_pShapeList.takeAt(i);
			return;
		}
	}
	qDebug() << "[DEBUG] KCanvas::removeFromShapeList missing";
}

