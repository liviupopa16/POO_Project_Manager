#include "PM_GUI_.h"
#include "Manager_Window.h"
#include "Employee_Window.h"
#include <QMessageBox>

PM_GUI_::PM_GUI_(QWidget *parent) : QMainWindow(parent), ui(new Ui::PM_GUI_Class)
{
    ui->setupUi(this);
}

PM_GUI_::~PM_GUI_()
{
    delete ui;
}


void PM_GUI_::on_Login_Button_clicked()
{
    QString username = ui->lineEdit_UserName->text();
    QString password = ui->lineEdit_Password->text();

    if (username == "manager" && password == "manager")
    {
        QMessageBox::information(this, "Login successfully", "You have been successfully \n  logged in as a manager!");
        hide();
        managerWindow = new Manager_Window;
        managerWindow->show();
    }
    else
    {
        if (username == "employee" && password == "employee")
        {
            QMessageBox::information(this, "Login successfully", "You have been successfully \n  logged in as a employee!");
            hide();
            employeeWindow = new Employee_Window;
            employeeWindow->show();
        }
        else
        {
            QMessageBox::information(this, "Unauthorized login", "Incorrect username or password!");
        }
    }
}


