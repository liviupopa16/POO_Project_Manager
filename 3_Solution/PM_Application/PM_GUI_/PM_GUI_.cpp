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

    database = QSqlDatabase::addDatabase("QODBC");
    database.setHostName("localhost");
    database.setDatabaseName("DRIVER={SQL Server};SERVER=localhost, 1434;DATABASE=Project_Manager;Trusted=true;");
    
}

PM_GUI_::~PM_GUI_()
{
    delete ui;
    database.close();
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

  


    if (database.open())
    {
        querymodel = new QSqlQueryModel();
        querymodel->setQuery("select Credential_Id from Credentials where Email = '" + username + "'");
        credID = querymodel->data(querymodel->index(0, 0)).toInt();


        querymodel->setQuery("SELECT Email, Password FROM Credentials as C inner join Users as U on C.Credential_Id = U.Credential_Id inner join Managers as M on M.User_Id = U.User_Id WHERE Email='" + username + "' AND Password='" + password + "'");        
        if (querymodel->rowCount() == 1)
        {
            msg1.setText("You have been successfully \n  logged in as a manager!");
            msg1.exec();
            close();
            managerWindow = new Manager_Window(credID);
            managerWindow->show();
        }
        else
        {
            querymodel->setQuery("SELECT Email, Password FROM Credentials as C inner join Users as U on C.Credential_Id = U.Credential_Id inner join Employees as E on E.User_Id = U.User_Id WHERE Email='" + username + "' AND Password='" + password + "'");
            if (querymodel->rowCount() == 1)
            {
                msg1.setText("You have been successfully \n  logged in as a employee!");
                msg1.exec();
                hide();
                employeeWindow = new Employee_Window(credID);
                employeeWindow->show();
            }
            else
            {
                msg2.exec();
            }
        }
    }
    else
    {
        QMessageBox::information(this, "Failed", "Database connection failed");
    }
}


