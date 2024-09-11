







### 1.项目需求

[week04 - week05 周作业 (kdocs.cn)](https://365.kdocs.cn/l/cpyq8ALEVt07?openfrom=docs)



- 项目目标:

  - [x] 实现SVG编辑器, 支持对SVG文件进行编辑,保存,查看

- 图形绘制:

  - [x] {直线,矩形, 五边形,六边形,星型} {铅笔自由线},{文本}
  - [x] 绘制,获取,高亮显示,拖拽移动, 拖拽改变形状
  
- 画布参数栏

  - [x] 高度,宽度, 颜色参数
  - [x] 画布倍率缩放, 缩放时图形同步缩放
  - [x] 线宽设置, 线型设置
  - [x] 图形的填充色,图形的边框色

- 文件菜单栏

  - [x] 新建画布
  - [x] 打开SVG
  - [x] 保存
  - [x] 导出为PNG

- 右键菜单栏/快捷键

  - [x] 全选以及全选快捷键
  - [x] 剪切,复制,粘贴
  - [x] 修改画布颜色,尺寸,边框, 样式,边框宽度
  - [x] redo/undo --- 以下所有操作都是在一个操作栈中,即实现不同操作混合的redo/undo
    - 图形添加删除的 redo/undo
    - 图形线型,线颜色,填充颜色的redo/undo
    - canvas颜色,尺寸设置的redo/undo

- 额外实现

  - [x] 不同操作统一的撤回,重做操作
  - [x] 按下ctrl连续单击实现多选
  - [x] 选中图形支持置于顶层和置于底层
  - [x] Text文本的二次编辑
  - [x] svg载入后二次编辑

- 展望实现

  - [ ] Text: fontsize, fonttype, fontcolor的box

  - [ ] 支持网络中外部资源svg的查看,载入,二次编辑

    

  

### 2.结构设计

按照功能,可分为以下模块

- 图形绘制模块
  - 多边形绘制
  - 文本绘制
- 图形 移动,缩放,拖拽模块
- 参数设置模块
- SVG载入,导出模块
- 撤销/重做模块



#### 2.1图形绘制模块



![image-20240803010544255](https://test4projectwf.oss-cn-hangzhou.aliyuncs.com/imageimage-20240803010544255.png)



##### 1.多边形绘制, 线段绘制, 圆形绘制

五边形,六边形,矩形,五角星都属于多边形绘制,由于线段和圆形的绘制逻辑较为相似,故而一起讲解

![image-20240803012624988](https://test4projectwf.oss-cn-hangzhou.aliyuncs.com/imageimage-20240803012624988.png)



1. KToolButton
   1. 左侧通过button选择对应的绘制图形,同时该flag会同步至GlobalData中
2. mousePress
   1. 按下左键,则会根据flag创建对应图形, 同时设置下startPoint, 并将该图形通过currentShape指针指向
3. mouseMove
   1. 判断左键是否持续按下, 左键按下并拖动意味着图形的绘制过程
   2. 调用setEndPoint, 并通过update触发PaintEvent,即描绘当前图形
   3. 同时对于startPoint-endPoint的框图矩形,也会进行绘制
4. mouseRelease
   1. 判断当前是否是图形绘制状态, 如果是则意味着此时当前图形绘制完毕,故而对currentShape进行归档,并清空状态





##### 2.多段线绘制

![image-20240803022841448](https://test4projectwf.oss-cn-hangzhou.aliyuncs.com/imageimage-20240803022841448.png)

1. KToolButton
   1. 通过按键设置当前绘制图形为PenLine
2. mousePress
   1. 按下左键即意味着开始绘制图形, KPenLine实例中含有painterpath, 此时按下左键,会触发setStartPoint, 同时path moveTo该点并开始绘制PenLine
3. mouseMove
   1. 如果持续按下左键且当前flag为绘制penline, 则此时会调用path的LineTo方法, 对当前的path计算出其外接矩形, 在此对startPoint和endPoint进行赋值
4. mouseRelease
   1. 当释放鼠标时,则会对当前图形归档





##### 3.文本绘制

文本编辑与图形绘制流程差别较大

1. button设置绘图flag
2. 鼠标单击
   1. 如果当前状态为isEditing,且currShape类型为Text,且当前点击shape并不是之前的currShape
      1. 则判断当前文本输入已经结束,会对文本归档并清空所有状态
   2. 在当前位置绘制一个输入框矩形,表示输入位置
   3. 构建Text实例
   4. 将isEditing置为true
3. 释放鼠标
   1. 如果判断isEditing = true, 则并不会释放选中
4. 键盘输入
   1. 如果键盘输入的是普通字符
      1. 文本在光标idx插入字符
      2. 光标idx++
   2. 如果是esc enter 键, 表示结束输入
      1. 将Text实例归档
      2. 清空所有状态
      3. 且实例的isEditing置为false, 此时不会再绘制边框
   3. 如果是 左右键, 退格键
      1. 执行对应的光标移动逻辑





#### 2.2 SVG载入导出模块

![image-20240803010544255](https://test4projectwf.oss-cn-hangzhou.aliyuncs.com/imageimageimage-20240803010544255.png)

##### 1.导出

SVG的导出依赖于QSvgGenerator, 其基类为PainterDevice, 即也是Painter的输出设备, 我们将painter的输出设置为QSvgGenerator即可实现图形的导出

同样地, 导出PNG也是相同的逻辑, 将输出设备改变为pixmap即可





##### 2.载入

在此我们以QSvgGenerator生成的svg文件作为输入案例



1. 解析svg文件

   1. svg采用xml格式, 通过逐个解析标签,可获得对应图形

      1. 如果是g标签,则其attribute是group的共有属性
      2. rect - 矩形  ellipse-椭圆  path-多段线

   2. 为处理嵌套group属性, 对xml树进行dfs遍历,维护一个gStack栈,存储g标签内容

      1. 如果是g标签的开始, 则入栈element
      2. 如果是g标签的结束, 则出栈
      3. 对于所有标签, 都会从栈顶取出一份Element拷贝,以此为基础,将当前xml标签属性赋值给element

   3. factory中实现以element为参数的createShape方法

      1. 对于一般图形,通过element.attribute计算出start,end即可构造图形
      2. 对于文本, 通过xml.readText,读取标签内的文本内容
      3. 对于path, 通过正则表达式解析标签出中的M L字段, 执行对应的move, line来实现多段线的绘制

      









#### 2.3图形 移动,缩放,拖拽模块



##### 1.图形缩放

首先定义两个尺寸

1. 逻辑尺寸:
   1. 图形在于基准画布上的尺寸, 基准画布即为参数width*height
2. 物理尺寸:
   1. 缩放之后的画布, 其尺寸为基准画布*缩放比例scale



所有关于坐标改变的操作, 首先作用于逻辑坐标

所有关于绘图的逻辑:通过缩放比例计算出物理坐标执行绘图



##### 2.图形移动

1. 在mousePress时记录一个lastPos,作为向量起点
2. 在mouseMove时,lastPos->currPos即为一个向量, 将其作用于shape的startPoint和endPoint实现实时的移动效果, 同时更新lastPos作为下一次move的起点



##### 3.图形拖拽变形

1. 对于外接矩形框, 定义8个方向,表对应的拖拽方向,捕捉鼠标位置,修改光标风格以显示拖拽类别
2. 根据拖拽类型,对对应的start.x start.y end.x end.y进行赋值,例如,向上拖拽是修改start.y,而右下拖拽则是修改end.x end.y
3. start和end的位置改变,重新绘制图形即可显示拖拽变形后的结果



#### 2.4 参数设置

![image-20240803033506605](https://test4projectwf.oss-cn-hangzhou.aliyuncs.com/imageimage-20240803033506605.png)

将paramBox分为3个类型, 分类标准为输入方式

1. valueBox, 即内容为数值
   1. 画布尺寸, 图形线宽, 字体大小
2. colorBox,内容为颜色rgb
   1. 画布颜色, 图形填充颜色, 图形线型颜色, 字体颜色
3. combox,内容为Item
   1. 图形线型, 字体



参数设置流程

1. ParamBox中输入对应内容, 发出selectParam信号, 不同的Box对应不同的属性设置信号

2. HandleSelect槽函数中,

   1. 将Box中组件同步改变为对应状态
   2. 在GlobalData中对参数进行保存
   3. 调用validate函数使其生效

   



#### 2.5撤销重做

![image-20240803110828405](https://test4projectwf.oss-cn-hangzhou.aliyuncs.com/imageimage-20240803110828405.png)

首先我们对于所有操作进行分类:

1. Add Shape 创建图形,粘贴图形
2. Delete Shape  删除图形
3. Set  shape parameter 设置图形属性
4. Set  canvas parameter 设置画布属性



所有操作都派生于基类Opeartion

基类中必须实现以下虚函数:

1. reverse:  获得逆操作
2. execute: 执行操作







###### reverse

对于Add操作reverse得到逆操作Delete, 同样地Delete操作逆操作是Add

而setParam from a to b 的逆操作即为: setParam from b to a

```cpp
Operation* OpAdd::reverse()
{
	// 将还原的shape删除
	return new OpDel(m_pShape);
}

Operation* OpDel::reverse()
{
	return new OpAdd(m_pShape);
}

Operation* OpShapePara::reverse()
{
	return new OpShapePara(m_pShape, m_type, m_new, m_old);
}

Operation* OpCanvasPara::reverse()
{
	return new OpCanvasPara(m_type, m_new, m_old);
}
```



###### execute





**对于Add, Del操作**

```cpp
void OpAdd::exec()
{
	// 1. deleteStack -> shapeList
	deletePop();
	ShapeListPrepend(shape);
}

void OpDel::exec()
{
	// 1. shapeList -> deletStack
	removeFromShapeList(m_pShape);
	deletePush(m_pShape);
}
```

add, del并不执行shape的new, delete操作, 而是会在GlobalData中维护一个delete栈

add: 将delete栈弹栈, 同时将对象加入到shapeList中

delete: 将shapeList中指定对象take出来, 将对象加入到deleteStack中

之所以这么设计,原因是我们在删除对象时, 并不能真正地将其析构掉, 因为这样我们在执行撤销时就需要重新new一个对象出来, 这样对象指针就已经发生改变, 而之前undoStack中存储的关于这个对象的其他参数设置操作就无法再生效了,因为句柄发生了改变.

换句话来说: add的语义为还原删除而不是重新构造



对于setParam

```cpp
void OpShapePara::exec()
{
	switch (m_type)
	{
	case KShapeParaType::LineColor:
		m_pShape->setShapeLineColor(m_new.toString());
		break;
	case KShapeParaType::ShapeColor:
		m_pShape->setShapeColor(m_new.toString());
		break;
	case KShapeParaType::LineWidth:
		m_pShape->setShapeLineWidth(m_new.toInt());
		break;
	case KShapeParaType::LineType:
		m_pShape->setShapeLineType(m_new.value<Qt::PenStyle>());
		break;
	default:
		break;
	}
	return;
}
```



对于setParam操作而言, 我们会存储 (shape, type, old, new), 即将shape的type属性从old赋值为new, 由于参数的类型不同, 故而用varient进行存储, 并将type定义为 num class, 执行时根据不同类型执行shape的对应set操作





###### 撤销重做

同时针对于撤销/重做,还需要有两个操作栈进行支持, undoStack和redoStack

```cpp
void KSvgMainWindow::actionUndoTriggered()
{
	// 1. 将undoStack弹栈
	Operation* op = popUndo();

	// 2. exec
	op->exec();
	m_pCanvas->update();

	// 3. redo
	KGlobalData::getGlobalDataIntance()->pushRedo(op->reverse());
}

// 1. 重做
void KSvgMainWindow::actionRedoTriggered()
{
	// 1. 将redoStack弹栈
	Operation* op = popRedo();

	// 2. exec
	op->dump();
	op->exec();
	m_pCanvas->update();

	// 3. redo
	KGlobalData::getGlobalDataIntance()->pushUndo(op->reverse());
}
```



Operation的exec职责为撤销重做操作,而不是广义的操作执行

当我们在非撤销重做场景下进行操作时, 我们会在对应位置对该操作进行记录: 构造逆操作operation并压栈undo

```cpp
m_pCurrentShape = KShapeFactory::createShape(getDrawFlag());
pushUndo(new OpDel(m_pCurrentShape));
```

 撤销时: 我们从undostack弹栈, 执行操作, 压栈redostack

重做时: 我们从redostack弹栈, 执行操作, 压栈undostack











### 3.实现笔记



#### 1.绘制逻辑是什么

派生类实现基类KShape的drawShape





##### 1.1矩形绘制逻辑是什么

通过其矩形框的start-end作为矩形参数



##### 12.圆形绘制逻辑是什么

通过矩形框绘制椭圆,  所绘为矩形框中的最大椭圆





##### 1.3五边形,六边形,五角星绘制逻辑是什么

找到对应顶点, 调用构造多边形方法绘制





##### 1.4文本框的绘制逻辑是什么

点击 -> 获取 pos, 显示一个矩形区域, 光标闪烁 

切换至文本输入状态, 可以获取键盘输入, 





##### 1.5画笔线的绘制逻辑是什么



1. drawshape函数职责是重绘而不是初次绘制





1. 初绘逻辑

   1. 由于shape的绘图时,会有一个startpoint和endpoint的输入, 所以我们的penline可以在这里进行一个设置, startpoint和endpoint都会向当前的qpaintpath中输入点, 这样可以不用修改主体mousepress,mousemove,mouserelease的逻辑

      1. 

      

   2. 鼠标 按下->移动->释放 过程中所有点集, 两两相连绘制成的一条曲线

   







#### 2.拖拽变形/移动 逻辑是什么?

拖拽和移动的接口都在**mouseMoveEvent** - **dragMoveShape**中

因为其触发的flag判断逻辑设置都是在选中之后, 每个图形都会有一个shapeRect, 图形上会有8个点位, getTransType通过鼠标位置来判断,对应的事件

> 所以所有的图形都应该有shaperect,
>
> 那么penLine的shapeRect是什么?
>
> - 尝试: 将vector中的所有point, 找到其max_x, max_y, min_x, min_y, 从而定义其rect



##### 2.1变形逻辑是什么?

当点击那八个变形点时,会触发对应的变形函数, 比如点击左上,就是修改start的x,y坐标, 而点击上, 就是修改start的y坐标, 同时在实现中, start,end没有位置上面的限制



> 记录一个BUG:
>
> 当通过拖拽使得start位于end的右下角之后, 此时getTransType函数会失效,因为其中判断四条边的逻辑中,
>
> ```cpp
> 	if (qAbs(rect.bottom() - pos.y()) < 5 
> 			&& pos.x() > rect.bottomLeft().x() + 5 
> 			&& pos.x() < rect.bottomRight().x() - 5)
> 		return KTransType::Bottom;
> ```
>
> 可以看到会有一个位置判断, 这里会失效



这里给的启发是, 所有图形的重绘都和rect有关, 但是shape重绘只需要标记start,end就可以绘制出新图形, 但是penLine不一样,所以应该为penLine的拖拽单独设计, 当然参数也是start,end

> 应该有一个高宽比例, 即当我们将现有的rect --> 新的rect时, 以start为基点进行绘制
>
> 新point的x:   新start.x + (旧point.x - 旧start.x)* (新width/旧width)
>
> y同理







#### 3.获取,高亮显示 逻辑是什么?

##### 3.1获取逻辑是什么?

单击鼠标时,判断当前是否是mouseDraw, 即是否是鼠标绘制

`getCurrentShape` 然后判断当前鼠标点击位置在哪个shapeRect内, 来判断所点击的是哪个对象

```cpp
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
```



所以这里有一个问题: 点击shapeRect时,是遍历shapelist, 所以如果有一个大的图形覆盖, 其内部的小图形是无法捕捉到的



##### 3.2高亮显示逻辑是什么?

高亮并不是对图形高亮,而是对shapeRect高亮, 

在canvas的paintevent中,如果当前是isselect状态,即有对象被选中,则会调用drawOutline,绘制rect







#### 4.缩放逻辑是什么?

在这里首先定义两个尺寸

1. 物理尺寸:
   1. 缩放之后的画布, 其尺寸为基准画布*scale
2. 逻辑尺寸:
   1. 图形在于基准画布上的尺寸, 基准画布为参数width*height所绘制的画布



`KSvgMainWindow::wheelEvent`对于滚轮变化事件有一个处理

首先有一个基准 `KGlobalData::m_canvasWidth`, 用来作为基准的高宽,即逻辑尺寸

在此基础上有一个缩放比例 scale, 这个是由滚轮控制的, 最终基准高宽乘以scale即物理尺寸,然后绘制于canvas上



一个尝试:

1. start, end 存储的是在 参数尺寸中的位置
2. startInCanvas, endInCanvas 存储的是在缩放之后的物理画布上存储的位置, 相应的鼠标范围逻辑也应该与这个进行比较





坐标使用: 

> 1. 鼠标对应坐标: 物理坐标
> 2. 捕捉时 :  `getShapeRect` 将pos与shape的start, end坐标进行比较,所以应该使用物理坐标
>    1. 捕捉之后的高亮显示`KShape::drawOutLine`也需要使用 物理坐标
> 3. 缩放之后的paintevent中, 这里绘制的是缩放之后的范围矩形,使用物理坐标
> 4. 绘图 `drawShape`: 物理坐标
> 5. 绘图事件: 
>    1. 单击和释放操作:  逻辑坐标
> 6. 移动, 拖拽
>    1. 会通过pos来判断所选择点位,  物理坐标





#### 5.按钮设置逻辑是什么?



1. ToolBar->ToolButton: 左侧按钮, 设置当前绘制图形
2. KCanvasParamsBar-> Box: 右侧属性盒子, 设置属性
   1. 画布属性:
   2. 图形属性:  





##### 5.1.绘制按钮逻辑是什么?







##### 5.2 属性Box逻辑是什么?



**KCanvasParamsBar**



按照属性作用的对象: 属性Box应该分为**全局Box**和**图形属性Box**两种, 

按照属性的类别, 即所设置的参数类型, 应该分为: 

- **数值**(高,宽,线宽)  - lineEdit
- **颜色**(画布颜色, 填充颜色, 线型颜色) - button 
- **线型**

这里不同的类型会有不同的选择方式, 限制逻辑, Box显示逻辑, 组件类型





> ###### 两种维度的分类方式
>
> 显然   图形线宽属性 同时符合**图形属性**和**数值属性**两种设定
>
> 所以这里是否应该使用一种 **多重继承**的方法来描述我们的Box?
>
> 这里可能会有菱形继承的问题, 目前还是选择一层的继承关系, 为每种情况独立设置





图形属性应该有以下几种:

1. 线型  KLineType
2. 线宽  KLineWidth
3. 线颜色
4. 填充颜色

这几种之前的全局属性不同的是, 这些都是shape独有的, 当捕捉到图形之后,应显示出该图形的这些信息



即在is_selected之后,应该有一个对属性设置的逻辑









###### 1.设置完属性,box的样式是如何改变的?

因为修改的地方正是box中的lineeidt, button这种控件, 用户输入属性,控件第一时间拿到属性并对自身进行设置, 这就是box上的外观变化时机

同时变化之后会发出一个pickedcolor/editfinished信号, 同时mainwindow中对属性进行更新



###### 2.应该如何实现点击shape,在box中显示出对应属性的逻辑?

1. 点击shape, 在mousePress - select逻辑中发出select信号
2. Box中接受到信号对当前的组件进行赋值,只改变显示不作其他改变



###### 3.如何实现box中修改,在对应shape中做出改变的逻辑?

1. shape中存有 线宽, 线型, 线色, 填充色的属性
2. 我们会拿到通过select传递过来的shape对象, 然后直接对其set即可
   1. 这些可以抽象到基类中的 setShapeParam,







颜色属性

```cpp 
// 选色器
QColor color = QColorDialog::getColor(QColor(str.toUInt(NULL, 16)), nullptr, QStringLiteral("选择颜色"), QColorDialog::ShowAlphaChannel);

// Qcolor -> str
str = QString::number(color.rgb(), 16);

```



- global中都是没#的
- shape中都是有#的





##### 5.3如何设置属性



信号链:

shapecolorbox

​						-> KGlobalData::getGlobalDataIntance()->setShapeColor(str); 

​						-> pickedColor信号

​										-> validateShapeColor槽 

​													-> KGlobalData::getGlobalDataIntance()->getShapeColor

​															-> shape->setShapeColor(color);

​																	-> drawShape









   					

​										-









#### 6.右键快捷键逻辑



##### 6.1全选逻辑是什么

现在的业务逻辑



- 点击左侧箭头按钮  -> 单选逻辑

- 右键全选 -> 全选逻辑
  - 点击空白释放全选, 和单选逻辑一样

- (todo) 点击左侧箭头-> 按住ctrl单击 -> 多选逻辑
- (todo) 框选 点击左侧箭头 -> 按住左键 移动 -> 绘制框选 -> 释放时进行捕捉计算 -> 多选逻辑





###### 1.全选选择显示框图逻辑

1. 右键点击全选触发槽函数 `actionSelectAllTriggered`
   1. 注册全选list,  m_pSelectedShapeList
   2. 全选flag  m_isSelectedAll 
2. 当前对象是一个vector, 选择了所有的对象 `QVector<KShape*> m_pSelectedShape;`
3.  全选之后, 所描绘的捕捉框是所有图形的捕捉框
   1. 在paintevent中, 绘制框图逻辑
4. 点击其他地方释放全选
   1. 在press中, 判断点击的是否是空白



###### 2.全选移动逻辑

1. 原有的单个物体的移动逻辑
   1. mousemove 中更新鼠标命中的捕捉点
   2. mousemove 中 如果持续按住左键移动, 则对当前图形进行移动/拖拽



全选移动逻辑:

1. 在全选之后, mousemove 中更新鼠标捕捉点, 但是transtype应都是 contain, 即移动
2. 在后续的移动渲染中
   1. 如果是全选 -> 全选移动
   2. 如果没有m_pCurrentShape -> return
   3. 如果没有全选 -> 单选移动
   4. 最后update()
3. MoveSelectedShape中对所选图形进行移动, 这个逻辑对框选,多选也是适用的



###### 3.多选选择设置box属性逻辑





- 左键单击, 如果是箭头选择按钮
  - 获取当前对象
  - 如果对象是已选择
    - 如果按了ctrl - nothing
    - 如果没按ctrl - nothing
  - 如果对象是未选择
    - 如果按了ctrl
      - 在当前的selected容器中增加
    - 如果没按ctrl
      - 重置当前seleted容器,在当前的selected容器中增加



###### 2.全选选择设置box属性逻辑









##### 6.2 复制粘贴剪切如何设置?

- 选择集合:  仅做浅拷贝
- 剪切板: 在ctrl c 之后做深拷贝, 且重置时会clear其中内容





1. 对于选择图形进行复制

   1. 在ctrl c时执行深拷贝, 在global中存储 selectList的复制

2. 如何兼容单击选择

   1. 在单击选择时, 将current图形加入到 selectList中

3. ctrl + c 之后,  将selectList中的图形 深拷贝至  剪切板中 **copySeleted**

4. ctrl +v: 将剪切板拷贝至图形上

   





###### 6.3剪切逻辑是?



1. 将当前selected复制
2. 将当前selected删除
3. ctrl +v: 将剪切板拷贝至图形上









#### 7.撤销的逻辑



2个栈, 1个redo_stack, 1个undo_stack



##### 7.1操作分类



首先我们对于所有操作进行分类:

1. Add Shape
2. Delete Shape
3. Set  shape parameter
4. Set  canvas parameter

![image-20240801103208905](https://test4projectwf.oss-cn-hangzhou.aliyuncs.com/imageimage-20240801103208905.png)

>  特地说明:
>
> 1. 对于图形的Move和Drag, 应当算是shape的startPoint, endPoint的赋值



对于所有操作, 都应当实现getReverse和do



> 当我们执行操作时:
>
> 1. 我们使用Operation来执行操作 (需要重写很多代码)
> 2. 我们按照原有逻辑来实现操作, 通过Operation来记录 (先用这个方法来实现撤销, 后续再将之前的第一次执行用operation执行)
>    1. 注意这里的记录指的是 --> 当我们执行了add之后, 我们会在redostack中压入 delete记录



##### 7.2撤销的算法设计



我们存有两个栈 Stack<Operation>

1. undoStack
2. redoStack



对于每个Operation, 都会有一个getReverse来获取其逆操作

- Add --> Delete
- Delete --> Add

- set A  para = a  (from b)      ------>      set A para = b



- 当我们执行每个操作时, 都会将压栈进 undoStack

- 执行 undo:   undoStack弹栈,   redoStack压栈
- 执行 redo:    redoStack弹栈,  undoStack压栈



> 需要注意的点:
>
> 1. delete: 设置一个delete栈, 删除的shape不会析构,而是从shapeList中移动至deleteStack中, add操作时从delete中拿出来而不是自己new ()











假设有这么一个场景





假设一个场景,我们顺序执行以下操作:

1. add rect
2. set rect color
3. delete rect
4. ctrl + z
5. ctrl + z
6. ctrl + y









































