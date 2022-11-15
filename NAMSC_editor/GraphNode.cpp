﻿#include "GraphNode.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

GraphNode::GraphNode(QGraphicsObject* parent)
	: QGraphicsObject(parent), nodeBody(GraphNodeBody(this, QRectF(0, 0, 300, 200)))
{
	//setParent(parent);
	setFlag(ItemHasNoContents);
	setFlag(ItemIsMovable);
	setCacheMode(DeviceCoordinateCache); // Not required - potentially increases performance
	setFlag(ItemSendsScenePositionChanges);
	//setZValue(-1); // example zvalue usage
}

GraphNode::~GraphNode()
{ }

QRectF GraphNode::boundingRect() const
{
	QRectF retVal = nodeBody.boundingRect();

	return retVal;
}

void GraphNode::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(painter);
	Q_UNUSED(option);
	Q_UNUSED(widget);
}

void GraphNode::setLabel(QString label)
{
	nodeBody.label = label;
}

void GraphNode::appendConnectionPoint(GraphConnectionType type)
{
	auto point = std::shared_ptr<GraphConnectionPoint>(new GraphConnectionPoint(this));
	QPointF pointPos{ pos().x(), pos().y() + boundingRect().height() / 5 };
	QPointF delta;
	
	switch (type) {
	case GraphConnectionType::In:
		pointPos.setX(pointPos.x() + 5);
		inputConnectionPointList.append(point);
		delta = { 0, boundingRect().height() / inputConnectionPointList.size() * 0.6 };
		for (auto elem : inputConnectionPointList) {
			elem->setPos(pointPos + (delta / 2) - QPointF{0, elem->boundingRect().height() / 2});
			pointPos += delta;
		}
		break;
	case GraphConnectionType::Out:
		pointPos.setX(pointPos.x() + boundingRect().width() - point->boundingRect().width() - 5);
		outputConnectionPointList.append(point);
		delta = { 0, boundingRect().height() / outputConnectionPointList.size() * 0.6 };
		for (auto elem : outputConnectionPointList) {
			elem->setPos(pointPos + (delta / 2) - QPointF{ 0, elem->boundingRect().height() / 2 });
			pointPos += delta;
		}
		break;
	default:
		// error?
		break;
	}

	update();
}

// TODO improve as append above
void GraphNode::insertConnectionPoint(GraphConnectionType type, size_t index)
{
	switch (type) {
	case GraphConnectionType::In:
		if (inputConnectionPointList.size() > index) {
			inputConnectionPointList.insert(index, std::make_shared<GraphConnectionPoint>(this));
		}
		break;
	case GraphConnectionType::Out:
		if (outputConnectionPointList.size() > index) {
			outputConnectionPointList.insert(index, std::make_shared<GraphConnectionPoint>(this));
		}
		break;
	default:
		// error?
		break;
	}
}

// TODO improve as append above
void GraphNode::removeConnectionPoint(GraphConnectionType type, size_t index)
{
	switch (type) {
	case GraphConnectionType::In:
		if (inputConnectionPointList.size() > index) {
			inputConnectionPointList.removeAt(index);
		}
		break;
	case GraphConnectionType::Out:
		if (outputConnectionPointList.size() > index) {
			outputConnectionPointList.removeAt(index);
		}
		break;
	default:
		// error?
		break;
	}
}

void GraphNode::connectPointTo(size_t index, GraphConnectionPoint* destination)
{
	if (outputConnectionPointList.size() > index) {
		auto arrow = std::make_shared<GraphArrow>(outputConnectionPointList.at(index).get(), destination);
		outputConnectionPointList.at(index)->connectArrow(arrow);
		destination->connectArrow(arrow);
		scene()->addItem(arrow.get());
	}
}

std::shared_ptr<GraphConnectionPoint> GraphNode::connectionPointAt(GraphConnectionType type, size_t index)
{
	switch (type) {
	case GraphConnectionType::In:
		if (inputConnectionPointList.size() > index) {
			return inputConnectionPointList.at(index);
		}
		break;
	case GraphConnectionType::Out:
		if (outputConnectionPointList.size() > index) {
			return outputConnectionPointList.at(index);
		}
		break;
	default:
		// error?
		break;
	}

	return nullptr;
}

void GraphNode::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) {
		lastLeftMousePressPoint = event->scenePos();
	}
}

QVariant GraphNode::itemChange(GraphicsItemChange change, const QVariant& value)
{
	int step = 100;
	if (change == ItemPositionChange) {
		//return QPoint(value.toPoint().x() - (value.toPoint().x() % step), value.toPoint().y() - (value.toPoint().y() % step));
		//std::function funX = value.toPoint().x() + lastLeftMousePressPoint.toPoint().x() > 0 ? std::ceilf : std::floorf;
		//std::function funY = value.toPoint().y() + lastLeftMousePressPoint.toPoint().y() > 0 ? std::ceilf : std::floorf;
		//auto res = QPoint((value.toPoint().x() + (lastLeftMousePressPoint.toPoint().x() % step)) / step * step, (value.toPoint().y() + (lastLeftMousePressPoint.toPoint().y() % step)) / step * step);

		auto res = QPoint(std::round((value.toPointF().x() + (lastLeftMousePressPoint.toPoint().x() % step)) / step) * step, std::round((value.toPointF().y() + (lastLeftMousePressPoint.toPoint().y() % step)) / step) * step);

		return res;
	}

	return QGraphicsItem::itemChange(change, value);
}