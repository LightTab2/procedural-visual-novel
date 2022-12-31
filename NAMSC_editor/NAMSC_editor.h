#pragma once

#include <QtWidgets/QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "GraphNode.h"
#include "ui_NAMSC_editor.h"
#include "GraphView.h"
#include "CollapseButton.h"
#include <Novel/Data/Novel.h>
#include "GraphNodePropertiesPack.h"
#include "PropertyConnectionSwitchboard.h"
#include "PropertyTypes.h"

class NAMSC_editor : public QMainWindow
{
    Q_OBJECT

public:
    NAMSC_editor(QWidget *parent = nullptr);
    ~NAMSC_editor();
    void prepareAssetsTree();

    void prepareSwitchboard();

public slots:
    void propertyTabChangeRequested(void* object, PropertyTypes dataType);

private:
    Ui::NAMSC_editorClass ui;
    Novel novel;

    PropertyConnectionSwitchboard switchboard;
    QGraphicsScene* scene; // todo remove?
    GraphNode* node;
    GraphNode* node2;

    void debugConstructorActions();
};
