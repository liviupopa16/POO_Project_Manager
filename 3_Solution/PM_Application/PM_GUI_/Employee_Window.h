#pragma once

#include <QWidget>
#include "ui_Employee_Window.h"
#include <QMessageBox>


class Employee_Window : public QWidget, public Ui::Employee_Window
{
	Q_OBJECT

public:
	Employee_Window(QWidget *parent = Q_NULLPTR);
	~Employee_Window();
public slots:
	void on_Logout_Button_clicked();
private:
	Ui::Employee_Window *ui;
};
