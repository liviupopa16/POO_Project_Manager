#pragma once

#include <QWidget>
#include "ui_Employee_Window.h"
#include <QMessageBox>

#include <QtCore>
#include <QtGui>
#include <QtSql>
#include <QListWidgetItem>


class Employee_Window : public QWidget, public Ui::Employee_Window
{
	Q_OBJECT

public:
	Employee_Window(int ID, QWidget *parent = Q_NULLPTR);
	~Employee_Window();
	QString CID;
	QString EmpID;
	int ret;
public slots:
	void on_Logout_Button_clicked();
	void on_Refresh_2_clicked();
	void on_ChangeP_2_clicked();
	void on_Mark_Done_clicked();
	void on_setProgress_clicked();
private:
	Ui::Employee_Window *ui;
	QSqlDatabase database;
	void Refresh();

	QSqlQueryModel* querymodel;
	QSqlQueryModel* querymodel1;
	QSqlQueryModel* querymodel2;
	int barStat;
	int CredID;

};
