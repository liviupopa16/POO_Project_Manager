#pragma once

#include <QWidget>
#include "ui_Manager_Window.h"
#include <QMessageBox>

#include <QtCore>
#include <QtGui>
#include <QtSql>
#include <QListWidgetItem>




class Manager_Window : public QWidget, public Ui::Manager_Window
{
	Q_OBJECT

public:
	Manager_Window(int ID, QWidget *parent = Q_NULLPTR);
	~Manager_Window();
	QString CID;
	QString ProjectID;
	QString CompanyID;
	int ret;

public slots:
	void on_Logout_Button_clicked();
	void on_Delete_Project_clicked();
	void on_Add_New_Project_clicked();
	void on_ShowTasks_clicked();
	void on_AddTask_clicked();
	void on_DeleteTask_clicked();
	void on_Refresh_clicked();


private:
	Ui::Manager_Window *ui;
	QSqlDatabase database;

	QSqlQueryModel* querymodel;
	QSqlQueryModel* querymodel1;
	QSqlQueryModel* querymodel2;


	int ManagerID;
	int CredID;



};
