#pragma once

#include <QWidget>
#include "ui_Employee_Window.h"

class Employee_Window : public QWidget, public Ui::Employee_Window
{
	Q_OBJECT

public:
	Employee_Window(QWidget *parent = Q_NULLPTR);
	~Employee_Window();

private:
	Ui::Employee_Window *ui;
};
