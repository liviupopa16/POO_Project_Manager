#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_PM_GUI_.h"
#include "Manager_Window.h"
#include "Employee_Window.h"
//#include <QtSql/qsqldatabase.h>
//#include <QtSql/qsqlquerymodel.h>
//#include <QSqlQuery>

#include <QtCore>
#include <QtGui>
#include <QtSql>

namespace Ui
{
    class PM_GUI_;
}

class PM_GUI_ : public QMainWindow
{
    Q_OBJECT

public:
    explicit PM_GUI_(QWidget *parent = Q_NULLPTR);
    ~PM_GUI_();
    int credID;



public slots:
    void on_Login_Button_clicked();

private:
    Ui::PM_GUI_Class *ui;
    Manager_Window* managerWindow;
    Employee_Window* employeeWindow;
    QSqlDatabase database;
    

    QSqlQueryModel* querymodel;


};
