#include "Node.h"

Node::Node(Node* parent)
{
}

Node::Node(int32_t x, int32_t y, const QString& nodeVal, const QColor& fillColor, Node* parent)
	:x(x), y(y), nodeVal(nodeVal), parent(parent)
{
	boundRectWidth = 50 + std::pow(nodeVal.length(), 1.5);
	boundRectHeight = 50 + std::pow(nodeVal.length(), 1.5);
	nodeOutilePen = new QPen(Qt::black);
	nodeFillBrush = new QBrush(fillColor);
	nodeEllip = new QGraphicsEllipseItem();
}

Node::Node(const QString& nodeVal, const QColor& fillColor, Node* parent)
	:nodeVal(nodeVal), parent(parent)
{
	x = 0;
	y = 0;
	boundRectWidth = 50 + std::pow(nodeVal.length(), 1.5);
	boundRectHeight = 50 + std::pow(nodeVal.length(), 1.5);
	nodeOutilePen = new QPen(Qt::black);
	nodeFillBrush = new QBrush(fillColor);
	nodeEllip = new QGraphicsEllipseItem();
}

Node::Node(const QPointF& pos, const QString& nodeVal, const QColor& fillColor, Node* parent)
	:x(pos.x()), y(pos.y()), nodeVal(nodeVal), parent(parent)
{
	boundRectWidth = 50 + std::pow(nodeVal.length(), 1.5);
	boundRectHeight = 50 + std::pow(nodeVal.length(), 1.5);
	nodeOutilePen = new QPen(Qt::black);
	nodeFillBrush = new QBrush(fillColor);
	nodeEllip = new QGraphicsEllipseItem();
}

Node::~Node()
{
}

QRectF Node::boundingRect() const
{
	return QRectF(x,y,boundRectWidth, boundRectHeight);
}

void Node::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	QRectF rect = boundingRect();
	painter->setPen(*nodeOutilePen);
	painter->setBrush(*nodeFillBrush);
	painter->drawEllipse(rect);

	QPointF boundRectCenter = rect.center();


	int32_t textStartX = ((boundRectCenter.x() + x) / 2) + ((boundRectCenter.x() - x) / 4);
	int32_t textStartY = boundRectCenter.y();
	QPoint textStartPos(textStartX, textStartY);

	painter->drawText(textStartPos, nodeVal);
}

Node* Node::getParent()
{
	return this->parent;
}

void Node::setParent(Node* parent)
{
	this->parent = parent;
}

void Node::setPenColor(const QColor& color)
{
	nodeOutilePen->setColor(color);
}


bool Node::operator==(const Node& other)
{
	//assumes integer validation is checked in the QLineEdit while getting input
	int thisVal = std::stoi(this->nodeVal.toStdString());	
	int otherVal = std::stoi(other.nodeVal.toStdString());

	if (thisVal == otherVal) {
		return true;
	}
	return false;
}

bool Node::operator!=(const Node& other)
{
	return !(*this == other);
}

bool Node::operator<(const Node& other)
{
	int thisVal = std::stoi(this->nodeVal.toStdString());
	int otherVal = std::stoi(other.nodeVal.toStdString());

	if (thisVal < otherVal) {
		return true;
	}
	return false;
}

bool Node::operator<=(const Node& other)
{
	return !(*this > other);
}

bool Node::operator>(const Node& other)
{
	int thisVal = std::stoi(this->nodeVal.toStdString());
	int otherVal = std::stoi(other.nodeVal.toStdString());

	if (thisVal > otherVal) {
		return true;
	}
	return false;
}

bool Node::operator>=(const Node& other)
{
	return !(*this < other);
}



