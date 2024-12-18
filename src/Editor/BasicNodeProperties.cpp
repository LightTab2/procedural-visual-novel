#include "BasicNodeProperties.h"

#include <QGraphicsScene>
#include <qmessagebox.h>

#include <pvnlib/Novel/Data/Novel.h>
#include <pvnlib/Novel/Event/EventChoice.h>

BasicNodeProperties::BasicNodeProperties(GraphNode* node, QWidget *parent)
	: QFrame(parent), currentlySelectedNode(node)
{
	ui.setupUi(this);
	ui.collapseButton->setContent(ui.content);
	ui.collapseButton->setText("Basic node properties");
	if (expanded) ui.collapseButton->toggle();
	//connect(scene, &QGraphicsScene::selectionChanged, this, &BasicNodeProperties::selectedNodeChanged);
	selectedNodeChanged();

	connect(ui.collapseButton, &CollapseButton::clicked, this, [] { expanded = !expanded; });
}

BasicNodeProperties::~BasicNodeProperties()
{
	//emit sceneUpdated(nullptr);
}

void BasicNodeProperties::updateConnections(bool b)
{
	instantTextChangeUpdate = b;

	if (not instantTextChangeUpdate) {
		connect(ui.nodeNameLineEdit, &QLineEdit::editingFinished, this, &BasicNodeProperties::updateLabelInNode);
	}
	else
	{
		connect(ui.nodeNameLineEdit, &QLineEdit::textChanged, currentlySelectedNode, &GraphNode::setLabel);
	}
}

void BasicNodeProperties::selectedNodeChanged()
{
	updateConnections(instantTextChangeUpdate);
	ui.nodeNameLineEdit->setText(currentlySelectedNode->getLabel());
}

void BasicNodeProperties::updateLabelInNode()
{
	QString lineEditText = ui.nodeNameLineEdit->text();
	if (currentlySelectedNode->getLabel() != lineEditText) {
		// Scene rename below
		if (Novel::getInstance().renameScene(currentlySelectedNode->getLabel(), lineEditText) == nullptr)
		{
			QMessageBox(QMessageBox::Critical, tr("Invalid scene name"), tr("Scene with this name already exists, please provide another name."), QMessageBox::Ok).exec();
			ui.nodeNameLineEdit->setText(currentlySelectedNode->getLabel()); // Revert change
			return;
		}
		else
		{
			// todo move this or similar to lib -> void renameSceneJumpsFor(QString scene, QString oldJumpToSceneName, QString newJumpToSceneName);
			// Update self jumps
			for (auto& ev : *Novel::getInstance().getScene(lineEditText)->getEvents())
			{
				switch (ev->getComponentEventType())
				{
				case EventSubType::EVENT_CHOICE:
					for (auto& choice : *static_cast<EventChoice*>(ev.get())->getChoices()) {
						if (choice.jumpToSceneName == currentlySelectedNode->getLabel()) const_cast<Choice&>(choice).jumpToSceneName = lineEditText;
					}
					break;
				case EventSubType::EVENT_JUMP:
					auto evj = static_cast<EventJump*>(ev.get());
					if (evj->jumpToSceneName == currentlySelectedNode->getLabel()) evj->jumpToSceneName = lineEditText;
					break;
				}
			}

			// Update jumps from other scenes to this
			for (auto& conn : currentlySelectedNode->getConnectionPoints(GraphConnectionType::In))
			{
				for (auto& ev : *Novel::getInstance().getScene(conn->getSourceNodeName())->getEvents())
				{
					switch (ev->getComponentEventType())
					{
					case EventSubType::EVENT_CHOICE:
						for (auto& choice : *static_cast<EventChoice*>(ev.get())->getChoices()) {
							if (choice.jumpToSceneName == currentlySelectedNode->getLabel()) const_cast<Choice&>(choice).jumpToSceneName = lineEditText; //todo: fix this monster
						}
						break;
					case EventSubType::EVENT_JUMP:
						auto evj = static_cast<EventJump*>(ev.get());
						if (evj->jumpToSceneName == currentlySelectedNode->getLabel()) evj->jumpToSceneName = lineEditText;
						break;
					}
				}
				
			}



			currentlySelectedNode->setLabel(lineEditText);
			currentlySelectedNode->update();
			emit sceneUpdated(Novel::getInstance().getScene(lineEditText));

		}
	}
}
