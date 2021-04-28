#include "PM_GUI_.h"
#include <QMessageBox>
#include <QKeyEvent>
#include <QShortcut>
#include <QIcon>

PM_GUI_::PM_GUI_(QWidget *parent) : QMainWindow(parent), ui(new Ui::PM_GUI_Class)
{
    ui->setupUi(this);
    QShortcut* returnShortcut = new QShortcut(QKeySequence("Return"), ui->centralWidget);
    QObject::connect(returnShortcut, SIGNAL(activated()), ui->Login_Button, SLOT(click()));
}

PM_GUI_::~PM_GUI_()
{
    delete ui;
}

void PM_GUI_::on_Login_Button_clicked()
{
    QString username = ui->lineEdit_UserName->text();
    QString password = ui->lineEdit_Password->text();

    QPixmap pixmap("Resources/icon.png");

    QMessageBox msg1, msg2;
    msg1.setWindowTitle("Login successfully");
    msg1.setIcon(QMessageBox::Information);
    msg1.setWindowIcon(QIcon(pixmap));

    msg2.setWindowTitle("Unauthorized login");
    msg2.setIcon(QMessageBox::Critical);
    msg2.setText("Incorrect username or password!");
    msg2.setWindowIcon(QIcon(pixmap));


    if (username == "manager" && password == "manager")
    {
        msg1.setText("You have been successfully \n  logged in as a manager!");
        msg1.exec();
        close();
        managerWindow = new Manager_Window;
        managerWindow->show();
    }
    else
    {
        if (username == "employee" && password == "employee")
        {
            msg1.setText("You have been successfully \n  logged in as a employee!");
            msg1.exec();
            hide();
            employeeWindow = new Employee_Window;
            employeeWindow->show();
        }
        else
        {
            msg2.exec();
        }
    }
}


