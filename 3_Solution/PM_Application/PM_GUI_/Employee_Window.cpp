#include "Employee_Window.h"
#include "PM_GUI_.h"

Employee_Window::Employee_Window(int ID, QWidget *parent) : QWidget(parent)
{
	ui = new Ui::Employee_Window();
	ui->setupUi(this);

	database = QSqlDatabase::addDatabase("QODBC");
	database.setHostName("localhost");
	database.setDatabaseName("DRIVER={SQL Server};SERVER=localhost, 1434;DATABASE=Project_Manager;Trusted=true;");

	CredID = ID;
	CID = QString::number(ID);

	if (database.open())
	{
		connect(ui->horizontalSlider, SIGNAL(valueChanged(int)),ui->progressBar, SLOT(setValue(int)));
		connect(ui->progressBar, SIGNAL(valueChanged(int)),ui->horizontalSlider , SLOT(setValue(int)));
		Refresh();
	}
	else
	{
		QMessageBox::information(this, "Failed", "Connection failed");
	}
}

Employee_Window::~Employee_Window()
{
	database.close();
	delete ui;
}

void Employee_Window::on_Logout_Button_clicked()
{
	QPixmap pixmap("Resources/icon.png");

	QMessageBox msg;
	msg.setWindowTitle("LOGOUT");
	msg.setText("Are you sure you want to Logout?");
	msg.setIcon(QMessageBox::Question);
	msg.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
	msg.setWindowIcon(QIcon(pixmap));

	int ret = msg.exec();
	switch (ret) {
	case QMessageBox::Yes:
		this->close();
		PM_GUI_* newmain = new PM_GUI_();
		newmain->show();
		break;
	}
}

void Employee_Window::on_Refresh_2_clicked()
{
	Refresh();
}

void Employee_Window::on_ChangeP_2_clicked()
{
	if (database.open())
	{
		QString newPass = ui->xlineEdit_pass_2->text();
		QSqlQuery ChangePass;
		ChangePass.prepare("UPDATE [Project_Manager].[dbo].[Credentials] SET [Password] = :newpass WHERE [Credential_Id] = :cid");
		ChangePass.bindValue(":newpass", newPass);
		ChangePass.bindValue(":cid", CID);

		QPixmap pixmap("Resources/icon.png");
		QMessageBox msg;
		QMessageBox msg1;
		msg.setWindowTitle("Changing password");
		msg.setText("Are you sure you want to change your password?");
		msg.setIcon(QMessageBox::Question);
		msg.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
		msg.setWindowIcon(QIcon(pixmap));

		msg1.setWindowTitle("SUCCESS!");
		msg1.setIcon(QMessageBox::Information);
		msg1.setWindowIcon(QIcon(pixmap));
		msg1.setText("Password changed successfully!");

		ret = msg.exec();
		switch (ret) {
		case QMessageBox::Yes:
			ChangePass.exec();
			msg1.exec();
			ui->xlineEdit_pass_2->setText("");
			break;
		}
	}
}

void Employee_Window::on_Mark_Done_clicked()
{
	if (database.open())
	{
		querymodel = new QSqlQueryModel();
		querymodel->setQuery("select T.Task_ID from Employees as E inner join Tasks as T on E.Task_Id = T.Task_Id inner join Projects as P on P.Project_Id=t.Project_Id where Employee_Id = '" + EmpID + "'");
		QString TaskID = querymodel->data(querymodel->index(0, 0)).toString();

		QSqlQuery QDeleteTask;
		QDeleteTask.prepare("UPDATE [Project_Manager].[dbo].[Employees] SET [Task_Id] = NULL FROM [Project_Manager].[dbo].[Employees] as E inner join [Project_Manager].[dbo].[Tasks] as T on E.Task_Id = T.Task_Id  WHERE T.Task_Id = :taskid");
		QDeleteTask.bindValue(":taskid", TaskID);

		QPixmap pixmap("Resources/icon.png");
		QMessageBox msg;
		QMessageBox msg1;
		msg.setWindowTitle("???");
		msg.setText("Are you sure you want to finish this task?");
		msg.setIcon(QMessageBox::Question);
		msg.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
		msg.setWindowIcon(QIcon(pixmap));

		msg1.setWindowTitle("SUCCESS!");
		msg1.setIcon(QMessageBox::Information);
		msg1.setWindowIcon(QIcon(pixmap));
		msg1.setText("Task completed successfully!");

		ret = msg.exec();
		switch (ret) {
		case QMessageBox::Yes:
			QDeleteTask.exec();


			QDeleteTask.prepare("DELETE Tasks WHERE Task_Id = :taskid");
			QDeleteTask.bindValue(":taskid", TaskID);
			QDeleteTask.exec();
			Refresh();
			msg1.exec();

			break;
		}
	}
}

void Employee_Window::on_setProgress_clicked()
{
	if (database.open())
	{
		barStat = ui->progressBar->value();
		QSqlQuery SetStat;
		SetStat.prepare("UPDATE Tasks SET Status = :stat FROM Tasks as T inner join Employees as E on E.Task_Id = T.Task_Id WHERE E.Employee_Id = :empID");
		SetStat.bindValue(":stat", barStat);
		SetStat.bindValue(":empID", EmpID);

		QMessageBox msg1;
		QPixmap pixmap("Resources/icon.png");
		msg1.setWindowTitle("SUCCESS!");
		msg1.setIcon(QMessageBox::Information);
		msg1.setWindowIcon(QIcon(pixmap));
		msg1.setText("Progress updated successfully!");

		SetStat.exec();
		msg1.exec();
	}
}

void Employee_Window::Refresh()
{
	if (database.open())
	{
		querymodel = new QSqlQueryModel();
		querymodel1 = new QSqlQueryModel();

		querymodel->setQuery("SELECT * FROM Users as U inner join Employees as E on E.User_Id = U.User_Id where Credential_Id = '" + CID + "'");
		EmpID = querymodel->data(querymodel->index(0, 5)).toString();
		querymodel->setQuery("SELECT AD.First_Name + ' ' + AD.Last_Name FROM Users as U inner join Account_Details as AD on AD.Account_Id = U.Account_Id where U.Credential_Id = '" + CID + "'");
		QString Prenume = querymodel->data(querymodel->index(0, 0)).toString();
		ui->labelTitle->setText("Name: " + Prenume + " /// Employee ID: " + EmpID);

		ui->viewFN_2->setText("First Name  ->  " + Prenume);
		querymodel->setQuery("SELECT AD.Last_Name FROM Users as U inner join Credentials as C on C.Credential_Id = U.Credential_Id inner join Account_Details as AD ON U.Account_Id = AD.Account_Id WHERE C.Credential_Id = '" + CID + "'");
		QString Nume = querymodel->data(querymodel->index(0, 0)).toString();
		ui->viewLN_2->setText("Last Name  ->  " + Nume);

		querymodel->setQuery("SELECT Email From Credentials where Credential_Id = '" + CID + "'");
		QString Email = querymodel->data(querymodel->index(0, 0)).toString();
		ui->viewE_2->setText("Email  ->  " + Email);

		querymodel->setQuery("SELECT AD.City FROM Users as U inner join Credentials as C on C.Credential_Id = U.Credential_Id inner join Account_Details as AD ON U.Account_Id = AD.Account_Id WHERE C.Credential_Id = '" + CID + "'");
		QString City = querymodel->data(querymodel->index(0, 0)).toString();
		ui->viewCity_2->setText("City  ->  " + City);

		querymodel->setQuery("SELECT AD.State FROM Users as U inner join Credentials as C on C.Credential_Id = U.Credential_Id inner join Account_Details as AD ON U.Account_Id = AD.Account_Id WHERE C.Credential_Id = '" + CID + "'");
		QString State = querymodel->data(querymodel->index(0, 0)).toString();
		ui->viewState_2->setText("State  ->  " + State);

		querymodel->setQuery("SELECT AD.Description FROM Users as U inner join Credentials as C on C.Credential_Id = U.Credential_Id inner join Account_Details as AD ON U.Account_Id = AD.Account_Id WHERE C.Credential_Id = '" + CID + "'");
		QString Description = querymodel->data(querymodel->index(0, 0)).toString();
		ui->viewAdress_2->setText(Description);

		querymodel->setQuery("SELECT AD.Phone_Number FROM Users as U inner join Credentials as C on C.Credential_Id = U.Credential_Id inner join Account_Details as AD ON U.Account_Id = AD.Account_Id WHERE C.Credential_Id = '" + CID + "'");
		QString Phone = querymodel->data(querymodel->index(0, 0)).toString();
		ui->viewPN_2->setText("Phone Number  ->  " + Phone);

		querymodel->setQuery("SELECT C.Name FROM Users as U inner join Company as C on C.Company_Id = U.Company_Id WHERE U.Credential_Id = '" + CID + "'");
		QString Company = querymodel->data(querymodel->index(0, 0)).toString();
		ui->viewCompany_2->setText("Working at " + Company);

		querymodel1->setQuery("select P.Name from Employees as E inner join Tasks as T on E.Task_Id = T.Task_Id inner join Projects as P on P.Project_Id=t.Project_Id where Employee_Id = '" + EmpID + "'");
		QString Project = querymodel1->data(querymodel->index(0, 0)).toString();
		ui->ProjectName->setText("From project  ->  " + Project);

		querymodel1->setQuery("select T.Name from Employees as E inner join Tasks as T on E.Task_Id = T.Task_Id where Employee_Id = '" + EmpID + "'");
		QString Taskname = querymodel1->data(querymodel->index(0, 0)).toString();
		ui->TaskName->setText("Task Name  ->  " + Taskname);

		querymodel1->setQuery("select T.Description from Employees as E inner join Tasks as T on E.Task_Id = T.Task_Id where Employee_Id = '" + EmpID + "'");
		QString Taskdesc = querymodel1->data(querymodel->index(0, 0)).toString();
		ui->TaskDesc->setText("Description  ->  " + Taskdesc);

		querymodel1->setQuery("select T.Deadline from Employees as E inner join Tasks as T on E.Task_Id = T.Task_Id where Employee_Id = '" + EmpID + "'");
		QString Taskdead = querymodel1->data(querymodel->index(0, 0)).toString();
		ui->TaskDead->setText("Deadline  ->  " + Taskdead);

		querymodel1->setQuery("select T.Status from Employees as E inner join Tasks as T on E.Task_Id = T.Task_Id where Employee_Id = '" + EmpID + "'");
		QString Status = querymodel1->data(querymodel->index(0, 0)).toString();

		ui->progressBar->setOrientation(Qt::Horizontal);
		ui->progressBar->setRange(0, 100); 
		ui->progressBar->setValue(Status.toInt()); 
	}
}
