#include "PropertyConnectionSwitchboard.h"

#include "Novel/Data/Novel.h"

PropertyConnectionSwitchboard::PropertyConnectionSwitchboard()
{}

PropertyConnectionSwitchboard::~PropertyConnectionSwitchboard()
{}

void PropertyConnectionSwitchboard::nodeSelectionChanged(GraphNode* node)
{
	emit nodeSelectionChangedSignal(node);
}

void PropertyConnectionSwitchboard::objectSelectionChanged(QString sceneryObjectName)
{
	emit objectSelectionChangedSignal(Novel::getInstance().getDefaultSceneryObject(sceneryObjectName));
}