#include "kshapefactory.h"
#include "ktext.h"
KShapeFactory::KShapeFactory(QObject *parent)
	: QObject(parent)
{
}

KShapeFactory::~KShapeFactory()
{
}

// 空参
KShape* KShapeFactory::createShape(KGlobalData::KDrawFlag drawFlag)
{
	KShape* ret;
	switch(drawFlag)
	{
	case KGlobalData::KDrawFlag::RectDrawFlag:
		ret = new KRect;
		break;
	case KGlobalData::KDrawFlag::CircleDrawFlag:
		ret = new KCircle;
		break;
	case KGlobalData::KDrawFlag::LineDrawFlag:
		ret = new KLine;
		break;
	case KGlobalData::KDrawFlag::PenDrawFlag:
		ret = new KPenLine;
		break;
	case KGlobalData::KDrawFlag::HexaDrawFlag:
		ret = new KHexa;
		break;
	case KGlobalData::KDrawFlag::PentDrawFlag:
		ret = new KPent;
		break;
	case KGlobalData::KDrawFlag::StarDrawFlag:
		ret = new KStar;
		break;
	case KGlobalData::KDrawFlag::TextDrawFlag:
		ret = new KText;
		break;
	default:
		qDebug() << "[ERROR] KShapeFactory::createShape ";
		return Q_NULLPTR;
	}
	return ret;
}

//QPainterPath parsePathData(const QString& data) {
//	QPainterPath path;
//	QRegExp moveToRegex("M\\s*(\\d+(?:\\.\\d+)?)\\s*,\\s*(\\d+(?:\\.\\d+)?)");
//	QRegExp lineToRegex("L\\s*(\\d+(?:\\.\\d+)?)\\s*,\\s*(\\d+(?:\\.\\d+)?)");
//	QRegExp closePathRegex("Z");
//
//	int pos = 0;
//	while ((pos = moveToRegex.indexIn(data, pos)) != -1) {
//		path.moveTo(moveToRegex.cap(1).toDouble(), moveToRegex.cap(2).toDouble());
//		pos += moveToRegex.matchedLength();
//	}
//
//	pos = 0;
//	while ((pos = lineToRegex.indexIn(data, pos)) != -1) {
//		path.lineTo(lineToRegex.cap(1).toDouble(), lineToRegex.cap(2).toDouble());
//		pos += lineToRegex.matchedLength();
//	}
//
//	pos = 0;
//	while ((pos = closePathRegex.indexIn(data, pos)) != -1) {
//		path.closeSubpath();
//		pos += closePathRegex.matchedLength();
//	}
//
//	return path;
//}

QPainterPath parsePathData(const QString& data) {
	QPainterPath path;
	QRegExp commandRegex("([MLZ])\\s*([\\d\\.\\s,]*)");
	int pos = 0;

	while ((pos = commandRegex.indexIn(data, pos)) != -1) {
		QString command = commandRegex.cap(1);
		QStringList coordinates = commandRegex.cap(2).split(QRegExp("\\s*,\\s*|\\s+"), Qt::SkipEmptyParts);

		if (command == "M" && coordinates.size() >= 2) {
			path.moveTo(coordinates[0].toDouble(), coordinates[1].toDouble());
		}
		else if (command == "L" && coordinates.size() >= 2) {
			path.lineTo(coordinates[0].toDouble(), coordinates[1].toDouble());
		}
		else if (command == "Z") {
			path.closeSubpath();
		}

		pos += commandRegex.matchedLength();
	}
	KPenLine::printPath(path);
	return path;
}

QRectF calculateBoundingRect(qreal cx, qreal cy, qreal rx, qreal ry) {
	qreal left = cx - rx;
	qreal top = cy - ry;
	qreal width = 2 * rx;
	qreal height = 2 * ry;

	return QRectF(left, top, width, height);
}

KShape* KShapeFactory::createShape(SvgManager::SvgElement element)
{
	KShape* ret;

	// 属性
	QString type = element.type;
	
	QString shapeColor = element.attributes.value("fill");
	QString linecolor = element.attributes.value("stroke");
	QString strokeOpacity = element.attributes.value("stroke - opacity"); // 边框透明度
	int linewidth = element.attributes.value("stroke - width").toInt();

	QString font = element.attributes.value("font - family"); // 字体
	int fontsize = element.attributes.value("font-size").toInt(); // 字号
	int fontWeight = element.attributes.value("font - weight").toInt(); // 字重
	QString fontStyle = element.attributes.value("font - style").toInt(); 
	
	int x = element.attributes.value("x").toInt();
	int y = element.attributes.value("y").toInt();
	int width = element.attributes.value("width").toInt();
	int height = element.attributes.value("height").toInt();

	if (type == "rect")
	{
		ret = new KRect;
		
	}
	else if (type == "ellipse")
	{
		ret = new KCircle;
		qreal cx = element.attributes.value("cx").toDouble();
		qreal cy = element.attributes.value("cy").toDouble();
		qreal rx = element.attributes.value("rx").toDouble();
		qreal ry = element.attributes.value("ry").toDouble();
		QRectF boundingRect = calculateBoundingRect(cx, cy, rx, ry);
		x = boundingRect.topLeft().x();
		y = boundingRect.topLeft().y();
		width = boundingRect.width();
		height = boundingRect.height();
		qDebug() << "cx = " << cx << " cy = " << cy;
		qDebug() << "x = " << x << " y = " << y;
	}
	else if (type == "line")
	{
		ret = new KLine;
	}
	else if (type == "path")
	{
		KPenLine* penLine = new KPenLine;
		QString pathData = element.attributes.value("d");
		QPainterPath path = parsePathData(pathData);
		//QRectF boundingRect = path.boundingRect();
		//x = boundingRect.topLeft().x();
		//y = boundingRect.topLeft().y();
		//width = boundingRect.width();
		//height = boundingRect.height();
		penLine->setPath(path);
		penLine->updateRect();
		return penLine;
	}
	else if (type == "text")
	{
		KText* text = new KText;
		QString textStr = element.text;
		text->setCurrentText(textStr);
		ret = text;
	}

	// 共有属性
	qDebug() << "x = " << x << " y = " << y << " width = " << width << " height = " << height;
	ret->setStartPoint(QPoint(x, y));
	ret->setShapeColor(shapeColor);
	ret->setEndPoint(QPoint(x+width, y+height));
	

	return ret;
}

