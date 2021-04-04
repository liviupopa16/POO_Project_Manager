#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_PM_GUI.h"

class PM_GUI : public QMainWindow
{
    Q_OBJECT

public:
    PM_GUI(QWidget *parent = Q_NULLPTR);

private:
    Ui::PM_GUIClass ui;
};
