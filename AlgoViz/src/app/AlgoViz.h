#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_AlgoViz.h"
#include "tree/BinarySearchTree.h"

class AlgoViz : public QMainWindow
{
    Q_OBJECT

public:
    AlgoViz(QWidget *parent = nullptr);
    ~AlgoViz();

private slots:
    void on_bstButton_clicked();
    void moveHome();

private:
    Ui::AlgoVizClass ui;
    BinarySearchTree bst;
};
