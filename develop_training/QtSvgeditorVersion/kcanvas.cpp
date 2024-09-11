#include "kcanvas.h"
#include "kshapefactory.h"
#include "koperation.h"
#include <QMouseEvent>
#include <QDebug>




KCanvas::KCanvas(QWidget* parent)
	: QWidget(parent)
	, m_pCurrentShape(Q_NULLPTR) // 当前正在绘制的图形
	, m_TransType(KTransType::None) // 当前的变换类型
	, m_isDrawing(false)
	, m_isLPress(false)
	, m_isSelected(false)
{
	setAttribute(Qt::WA_StyledBackground, true);

	// 设置背景颜色为白色
	setStyleSheet("background-color:#FFFFFF");

	KGlobalData::getGlobalDataIntance()->setCanvasColor("#FFFFFF"); // 设置
	resize(KGlobalData::getGlobalDataIntance()->getCanvasWidth(), // 通过全局设置高宽
		KGlobalData::getGlobalDataIntance()->getCanvasHeight());

	setMouseTracking(true);//设置鼠标追踪
	setFocusPolicy(Qt::ClickFocus);// 单击获取焦点
}

KCanvas::~KCanvas()
{
	for (auto shape : m_pShapeList)
		delete shape;
	m_pShapeList.clear();
}

// 绘图事件处理
void KCanvas::paintEvent(QPaintEvent* event)
{

	// 所有图形绘制
	if (!m_pShapeList.isEmpty())
	{
		auto it = m_pShapeList.rbegin();
		while (it != m_pShapeList.rend())
		{
			(*it)->drawShape(this);
			it++;
		}
	}

	// 当前正在绘制图形的描绘
	if (m_pCurrentShape != Q_NULLPTR)
	{
		if (m_isDrawing) // 图形绘制标志为 true，则表示正在绘制该图形，此时需要不断重新绘制
			m_pCurrentShape->drawShape(this);

		if (m_isSelected) // 图形选中，则绘制选中框
			m_pCurrentShape->drawOutLine(this);

		if (m_isEditing) // 如果editing,则绘制
		{
			m_pCurrentShape->drawShape(this);
		}
		
	}

	// 全选图形框图
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
	//获取当前需要绘制的图形类型
	KGlobalData::KDrawFlag flag = KGlobalData::getGlobalDataIntance()->getDrawFlag();
	
	// 如果没有点击左侧按钮,则不会做
	if (flag == KGlobalData::KDrawFlag::NoneDrawFlag)
		return;
	

	if (event->buttons() == Qt::LeftButton)
	{
		m_isLPress = true;

		if (flag == KGlobalData::KDrawFlag::MouseDrawFlag) // button设置
		{
			
			m_pCurrentShape = getCurrentShape(event->pos());	// 获取此次单击所选择的对象
			if (m_pCurrentShape != Q_NULLPTR)					// 如果点击的是图形, 捕捉逻辑
			{
				// 如果对象未选择
				if (std::find(m_pSelectedShapeList.begin(), m_pSelectedShapeList.end(), m_pCurrentShape) == m_pSelectedShapeList.end()) {
					if (!KGlobalData::getGlobalDataIntance()->m_isPressCtrl) // 如果没按下ctrl
					{
						m_pSelectedShapeList.clear();
					}
					else
					{
						// qDebug() << "ctrl is press";
					}
					m_pSelectedShapeList.append(m_pCurrentShape);	// 加入选择
				}
				qreal scale = KGlobalData::getGlobalDataIntance()->getCanvasScale();
				m_lastPos = QPoint(event->pos().x()/scale, event->pos().y() / scale);	// 用于move

				m_TransType = getTransType(event->pos());	// 获取移动类型
				m_isSelected = true;						// 设置选中标志
				emit selected(m_pCurrentShape);				// 用于反馈右侧属性框同步

				if (m_pCurrentShape->getShapeType() == KShapeType::TextShapeType)
				{
					m_cursorPosition = static_cast<KText*>(m_pCurrentShape)->getCurrentText().size();
				}
			}
			else // 未选中图形(点击空白)取消捕捉
			{
				m_isSelected = false;
				m_isSelectedAll = false;
				m_pSelectedShapeList.clear();
				emit selected(Q_NULLPTR);
			}
		}
		else if (flag == KGlobalData::KDrawFlag::TextDrawFlag)
		{
			// 如果m_cursorPosition != 0, 则释放输入
			if (m_cursorPosition != 0 )
			{
				finishedEditing();
				update();
				return;
			}



			// 创建文本框
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
	KGlobalData::KDrawFlag flag = KGlobalData::getGlobalDataIntance()->getDrawFlag();	// 获得button设置的当前绘图事件
	
	if (flag == KGlobalData::KDrawFlag::NoneDrawFlag)
		return;

	KTransType transType;
	if (m_pSelectedShapeList.size() > 1) // 多选状态下,当前状态都是移动
	{
		transType = KTransType::Contains;
	}
	else
	{
		transType = getTransType(event->pos());	// 实时更新捕捉点
	}

	updateCusorStyle(flag, transType); // 渲染捕捉点

	if (event->buttons() != Qt::LeftButton)//bug : 在不点击鼠标左键并在在选中状态，移动鼠标会移动图形
			return;


	// 当前绘图
	if (flag != KGlobalData::KDrawFlag::MouseDrawFlag)
	{
		if (m_isLPress && !m_isDrawing)
			m_isDrawing = true;
		if (m_pCurrentShape != Q_NULLPTR)
			m_pCurrentShape->setEndPoint(event->pos());
	}

	else // 拖拽/移动
	{
		// 全选
		if (m_pSelectedShapeList.size() > 1) 
		{
			MoveSelectedShape(transType, event->pos());
		}

		// 单选
		if (m_pCurrentShape == Q_NULLPTR)
		{
			return;
		}
		
		if(m_pSelectedShapeList.size() == 1)
		{
			dragMoveShape(transType, event->pos());//移动选中图形
		}
	}
	update();
}

void KCanvas::mouseReleaseEvent(QMouseEvent* event)
{
	KGlobalData::KDrawFlag flag = KGlobalData::getGlobalDataIntance()->getDrawFlag();

	// 即是松开左键的时刻
		
	if (m_isLPress) // 鼠标左键按下后释放，则表示移动或者绘图完毕
	{	
		// 如果当前正在编辑,则不应该执行图形归档逻辑, 留下矩形框图待输入
		if (m_isEditing && flag == KGlobalData::KDrawFlag::TextDrawFlag)
		{
			update();
			return;
		}
		if (flag != KGlobalData::KDrawFlag::MouseDrawFlag) // 因为mousedraw的逻辑都是连续执行的,所以不需要再release的时候进行处理
		{
			if (m_pCurrentShape != Q_NULLPTR) // 绘图之后的逻辑
			{
				if (m_pCurrentShape->isValid())			// 判断图形矩形是否有效
					m_pCurrentShape->setEndPoint(event->pos());

				m_pShapeList.prepend(m_pCurrentShape);	// 图形归档
				m_pCurrentShape = Q_NULLPTR;
				//m_iselected = true;
				//m_isDrawing = false;
					
			}
			
			else // 这里其实是框选的逻辑
			{
				// TODO: 这里delete了一个空指针
				// TODO: 这里可以增加框选逻辑
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

	
	KText* textEditShape = static_cast<KText*>(m_pCurrentShape); // 当前正在绘制文本
	QString text = textEditShape->getCurrentText();				
	if (event->key() == Qt::Key_Escape || event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) { // esc enter 退出编辑		
		if (text.size() > 0 && !m_pShapeList.contains(m_pCurrentShape)) // 加入绘图
			m_pShapeList.prepend(textEditShape);

		textEditShape->setIsEditing(false);
		m_pCurrentShape = nullptr;
		m_isEditing = false;
	}
	else if (event->key() == Qt::Key_Backspace && !text.isEmpty()) { // 退格删除文本
		text.chop(1);
		textEditShape->setCurrentText(text);
		m_cursorPosition = qMax(0, m_cursorPosition - 1); // 更新光标位置
	}
	else if (event->key() == Qt::Key_Left) {		// 左箭头键：移动光标位置
		m_cursorPosition = qMax(0, m_cursorPosition - 1);
	}
	else if (event->key() == Qt::Key_Right) {		// 右箭头键：移动光标位置
		m_cursorPosition = qMin(text.length(), m_cursorPosition + 1);
	}
	else if (!event->text().isEmpty()) {		// 其余文本键入
		text.insert(m_cursorPosition, event->text());
		textEditShape->setCurrentText(text);
		m_cursorPosition++;
	}
	update();
	QWidget::keyPressEvent(event);
}

KShape* KCanvas::getCurrentShape(const QPoint &pos)
{
	// 遍历
	for (QList<KShape*>::iterator it = m_pShapeList.begin();
		it != m_pShapeList.end(); ++it)
	{
		// 判断pos是否在shapeRect上
		if ((*it)->getShapeRect().contains(pos))
			return *it;
	}

	return Q_NULLPTR;
}

// 拖拽变形/移动
KTransType KCanvas::getTransType(const QPoint& pos)
{
	if (!m_pCurrentShape)
		return KTransType::None;

	// 获取图形矩形
	QRect rect = m_pCurrentShape->getShapeRect();

	// 在全局中设置当前图形

	if (!rect.contains(pos))
		return KTransType::None;
	

	// 通过鼠标所点击的位置 判断是整体移动，局部拖拽移动
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
	// 如果已经选中
	if (m_isSelected
		&& flag == KGlobalData::KDrawFlag::MouseDrawFlag
		&& transType != KTransType::None)
	{
		//TODO：变换图标,设置不同的鼠标样式,根据点击位置，设置不同的鼠标样式
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
		在移动过程中，分为两种移动方式
			(1) 整体移动  contains
			(2) 基于某一个方向的拖拽

	*/
	qreal scale = KGlobalData::getGlobalDataIntance()->getCanvasScale();
	QPoint pos = QPoint(ppos.x() / scale, ppos.y() / scale);

	switch (m_TransType)
	{
	case KTransType::None:
		return;
	case KTransType::TopLeft:
		// 如果是左上角，修改start
		m_pCurrentShape->moveTopLeft(pos);
		break;
	case KTransType::Top:
		// 如果是向上移动，修改start_y
		m_pCurrentShape->moveTop(pos);
		break;
	case KTransType::TopRight:
		m_pCurrentShape->moveTopRight(pos);
		break;
	case KTransType::Left:
		m_pCurrentShape->moveLeft(pos);
		break;
	case KTransType::Contains: // 整体移动
	{
		// m_lastPos 为选中时光标位置，在移动过程中不断赋值为前一次的位置
		QPoint newpos = pos -  m_lastPos; // 计算要移动的偏移
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
		在移动过程中，分为两种移动方式
			(1) 整体移动  contains
			(2) 基于某一个方向的拖拽

	*/

	qreal scale = KGlobalData::getGlobalDataIntance()->getCanvasScale(); // 缩放逻辑
	QPoint pos = QPoint(ppos.x() / scale, ppos.y() / scale);

	QPoint newpos = pos - m_lastPos; // 计算要移动的偏移
	for (auto it : m_pSelectedShapeList)
	{
		// m_lastPos 为选中时光标位置，
		it->move(newpos);
		m_lastPos = pos;
	}
}

void KCanvas::selectAll()
{
	// 容器设置, 状态设置
	m_pSelectedShapeList = m_pShapeList; // 浅拷贝, 只拷贝指针
	m_isSelectedAll = true;

	// update
	update();
}

void KCanvas::copySeleted()
{
	// 1. 清空剪切板
	for (auto it : m_pClipboard)
	{
		delete it;
	}
	m_pClipboard.clear();


	// 2. 将selected中内容深拷贝
	for (auto it : m_pSelectedShapeList)
	{
		m_pClipboard.append(it->clone());
	}
}

void KCanvas::paste()
{

	// 1.做一个偏移
	for (auto it : m_pClipboard)
	{
		it->move(QPoint(20,20));

	}

	// 2. 将剪切板中的内容拷贝至shaplist
	for (auto it : m_pClipboard)
	{
		m_pShapeList.append(it->clone());
	}

	update();
}

void KCanvas::deleteSelected()
{
	// 1. 在所有shape中删除选中图形
	QList<KShape*> ret;


	// 1. shapelist中挪至deletestack,  并载入add到redostack
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

	// 2. 清空选中
	m_pSelectedShapeList.clear();
	m_pCurrentShape = Q_NULLPTR;

	// 3.
	update();
}

void KCanvas::finishedEditing()
{
	KText* textEditShape = currediting;
	QString text = textEditShape->getCurrentText();
	if (text.size() > 0 && !m_pShapeList.contains(m_pCurrentShape)) // 加入绘图
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

