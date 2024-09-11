#include "svgmanager.h"
#include "kshapefactory.h"
#include <QStack>
// 解析文件
void SvgManager::parseFile(const QString& filePath) {
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qWarning() << "Failed to open file" << filePath;
	}

	QXmlStreamReader xml(&file);

	QStack<SvgElement> groupStack; // group 属性栈

	while (!xml.atEnd() && !xml.hasError()) {
		QXmlStreamReader::TokenType token = xml.readNext();

		if (token == QXmlStreamReader::StartElement) 
		{
			if (xml.name() == "g") 
			{
				SvgElement element;
				element.type = xml.name().toString();
				foreach(const QXmlStreamAttribute & attr, xml.attributes()) 
				{
					element.attributes.insert(attr.name().toString(), attr.value().toString());
				}
				groupStack.push_back(element); // 压栈
			}
			if (xml.name() == "rect" || xml.name() == "ellipse" || xml.name() == "line" || xml.name() == "path" || xml.name() == "text")
			{
				SvgElement element;
				if (!groupStack.empty())
				{
					element = groupStack.top(); // 栈顶(值拷贝)
				}
				
				element.type = xml.name().toString(); // 从g更换为图形
				foreach(const QXmlStreamAttribute & attr, xml.attributes()) // 添加多余的属性
				{ 
					element.attributes[attr.name().toString()] =  attr.value().toString(); // 覆盖
				}
				element.text = xml.readElementText();
				elements.push_back(element); // 存结果
			}
		}
		else if (token == QXmlStreamReader::EndElement) 
		{
			if (xml.name() == "g") 
			{
				groupStack.pop();
			}
		}
	}

	qDebug() << "elements size is " << elements.size();
	for (auto it : elements)
	{
		dumpElement(it);
	}
	file.close();
	return;
}

void SvgManager::dumpElement(SvgElement ele)
{
	qDebug() << "[DEBUG] >>>> " << ele.type;
	for (auto it = ele.attributes.constBegin(); it != ele.attributes.constEnd(); ++it) {
		qDebug() << it.key() << ":" << it.value();
	}
	qDebug() << "[DEBUG] >>>> \n";
}


QList<KShape*> SvgManager::loadElements()
{
	QList<KShape*> ret;
	for (auto ele : elements)
	{
		KShape* shape = KShapeFactory::createShape(ele);
		ret.push_back(shape);
	}
	return ret;
}

SvgManager::SvgManager()
{
}

SvgManager::~SvgManager()
{
}

