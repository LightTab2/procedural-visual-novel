#pragma once

#include <QFrame>

#include "GraphNode.h"
#include "ui_BasicNodeProperties.h"
#include "CollapseButton.h"

class BasicNodeProperties : public QFrame
{
	Q_OBJECT

public:
	BasicNodeProperties(QGraphicsScene* scene, QWidget *parent = nullptr);
	~BasicNodeProperties();

	void setScene(QGraphicsScene* scene);

	void updateConnections(bool b);

public slots:
	void selectedNodeChanged();

private:
	bool instantTextChangeUpdate = false;
	QGraphicsScene* scene;

	Ui::BasicNodePropertiesClass ui;

private slots:
	void updateLabelInNode();
};
