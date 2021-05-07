#pragma once

#include <QWidget>
#include "ui_Manager_Window.h"
#include <QMessageBox>
#include <QtSql/qsqldatabase.h>
#include <QtSql/qsqlquerymodel.h>



class Manager_Window : public QWidget, public Ui::Manager_Window
{
	Q_OBJECT

public:
	Manager_Window(QWidget *parent = Q_NULLPTR);
	~Manager_Window();
public slots:
	void on_Logout_Button_clicked();
private:
	Ui::Manager_Window *ui;
	QSqlDatabase database;
	QSqlQueryModel* querymodel;
};
