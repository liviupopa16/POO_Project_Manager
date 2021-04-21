#pragma once

#include <QWidget>
#include "ui_Manager_Window.h"

class Manager_Window : public QWidget, public Ui::Manager_Window
{
	Q_OBJECT

public:
	Manager_Window(QWidget *parent = Q_NULLPTR);
	~Manager_Window();

private:
	Ui::Manager_Window *ui;
};
