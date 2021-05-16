#include "Manager_Window.h"
#include "PM_GUI_.h"


Manager_Window::Manager_Window(int ID, QWidget *parent) : QWidget(parent)
{
	ui = new Ui::Manager_Window();
	ui->setupUi(this);

	database = QSqlDatabase::addDatabase("QODBC");
	database.setHostName("localhost");
	database.setDatabaseName("DRIVER={SQL Server};SERVER=localhost, 1434;DATABASE=Project_Manager;Trusted=true;");

	CredID = ID;
	CID = QString::number(ID);

	if (database.open())
	{

		querymodel = new QSqlQueryModel();
		querymodel1 = new QSqlQueryModel();


		querymodel1->setQuery("SELECT * FROM Projects");
		//ui->listEmp->setModel(querymodel1);



		/////////////////////////////////////////////////////////

		querymodel->setQuery("SELECT * FROM Users as U inner join Managers as M on M.User_Id = U.User_Id where Credential_Id = '" + CID + "'");
		QString ManID = querymodel->data(querymodel->index(0, 5)).toString();
		querymodel->setQuery("SELECT AD.First_Name FROM Users as U inner join Account_Details as AD on AD.Account_Id = U.Account_Id where U.Credential_Id = '" + CID + "'");
		QString Prenume = querymodel->data(querymodel->index(0, 0)).toString();
		ui->labelTitle->setText("Name: "+ Prenume +" /// Manager ID: " + ManID);

		
		ui->viewFN->setText("First Name  ->  " + Prenume);
		querymodel->setQuery("SELECT AD.Last_Name FROM Users as U inner join Credentials as C on C.Credential_Id = U.Credential_Id inner join Account_Details as AD ON U.Account_Id = AD.Account_Id WHERE C.Credential_Id = '" + CID + "'");
		QString Nume = querymodel->data(querymodel->index(0, 0)).toString();
		ui->viewLN->setText("Last Name  ->  " + Nume);

		querymodel->setQuery("SELECT Email From Credentials where Credential_Id = '" + CID + "'");
		QString Email = querymodel->data(querymodel->index(0, 0)).toString();
		ui->viewE->setText("Email  ->  " + Email);

		querymodel->setQuery("SELECT Password From Credentials where Credential_Id = '" + CID + "'");
		QString Pass = querymodel->data(querymodel->index(0, 0)).toString();
		ui->viewP->setText("Password  ->  " + Pass);


		querymodel1->setQuery("SELECT P.Project_Id, P.Name, P.Description, P.Period FROM Users as U inner join Managers as M on M.User_Id = U.User_Id inner join Projects as P on P.Manager_Id = M.Manager_Id where U.Credential_Id = '" + CID + "'");
		ui->tableProj->setModel(querymodel1);


		QSqlQueryModel QgetMID;
		QgetMID.setQuery("select M.Manager_id from Credentials as C inner join Users as U on C.Credential_ID = U.Credential_Id inner join Managers as M on U.User_Id=M.User_Id where C.Credential_Id=" + CID);
		ManagerID = QgetMID.data(QgetMID.index(0, 0)).toInt();

		/////////////////////////////////////////////////////////

		querymodel->setQuery("SELECT Company_Id FROM Users WHERE Credential_Id = '" + CID + "'");
		CompanyID = querymodel->data(querymodel->index(0, 0)).toString();

		querymodel->setQuery("SELECT AD.First_Name + ' ' + AD.Last_Name as 'Employee Name',E.Employee_Id  FROM Employees as E inner join Users as U on U.User_Id = E.User_Id inner join Account_Details as AD on AD.Account_Id = U.Account_Id WHERE E.Task_Id is NULL and U.Company_Id = '" + CompanyID + "'");
		ui->listTasks->setModel(querymodel);


	}
	else
	{
		QMessageBox::information(this, "Failed", "Connection failed");
	}

}

Manager_Window::~Manager_Window()
{
	database.close();
	delete ui;
}





void Manager_Window::on_Logout_Button_clicked()
{
	QPixmap pixmap("Resources/icon.png");

	QMessageBox msg;
	msg.setWindowTitle("LOGOUT");
	msg.setText("Are you sure you want to Logout?");
	msg.setIcon(QMessageBox::Question);
	msg.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
	msg.setWindowIcon(QIcon(pixmap));

	ret = msg.exec();
	switch (ret) {
	case QMessageBox::Yes:
		this->close();
		PM_GUI_* newmain = new PM_GUI_();
		newmain->show();
		break;
	}
}

void Manager_Window::on_Delete_Project_clicked()
{
	if (database.open())
	{
		QString ProjectID = ui->Project_Id_Delete->text();
		QSqlQuery QDelete;
		QDelete.prepare("delete from [Project_Manager].[dbo].[Projects] where ([Project_ID])=(:projectid)");
		QDelete.bindValue(":projectid", ProjectID);

		QPixmap pixmap("Resources/icon.png");
		QMessageBox msg;
		QMessageBox msg1;
		msg.setWindowTitle("???");
		msg.setText("Are you sure you want to delete this project?");
		msg.setIcon(QMessageBox::Question);
		msg.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
		msg.setWindowIcon(QIcon(pixmap));

		msg1.setWindowTitle("SUCCESS!");
		msg1.setIcon(QMessageBox::Information);
		msg1.setWindowIcon(QIcon(pixmap));
		msg1.setText("Project deleted successfully!");

		ret = msg.exec();
		switch (ret) {
		case QMessageBox::Yes:
			QDelete.exec();
			msg1.exec();
			break;
		}
		querymodel1->setQuery("SELECT P.Project_Id, P.Name, P.Description, P.Period FROM Users as U inner join Managers as M on M.User_Id = U.User_Id inner join Projects as P on P.Manager_Id = M.Manager_Id where U.Credential_Id = '" + CID + "'");
		ui->tableProj->setModel(querymodel1);
	}
}

void Manager_Window::on_Add_New_Project_clicked()
{
	if (database.open())
	{
		QSqlQuery QInsert;
		QString Manager_ID = QString::number(ManagerID);
		QString Name = ui->lineEdit_2->text();
		QString Description = ui->lineEdit_3->text();
		QString Period = ui->lineEdit_4->text();



		QInsert.prepare("INSERT INTO [Project_Manager].[dbo].[Projects] ([Manager_ID],[Name],[Description],[Period]) VALUES(:manager_id,:name,:description,:period)");
		QInsert.bindValue(":manager_id", Manager_ID);
		QInsert.bindValue(":name", Name);
		QInsert.bindValue(":description", Description);
		QInsert.bindValue(":period", Period);

		
		QInsert.exec();

		QPixmap pixmap("Resources/icon.png");
		QMessageBox msg1;
		msg1.setWindowTitle("SUCCESS!");
		msg1.setIcon(QMessageBox::Information);
		msg1.setWindowIcon(QIcon(pixmap));

		msg1.setText("Project added successfully!");
		msg1.exec();

		querymodel1->setQuery("SELECT P.Project_Id, P.Name, P.Description, P.Period FROM Users as U inner join Managers as M on M.User_Id = U.User_Id inner join Projects as P on P.Manager_Id = M.Manager_Id where U.Credential_Id = '" + CID + "'");
		ui->tableProj->setModel(querymodel1);

		ui->lineEdit_2->setText("");
		ui->lineEdit_3->setText("");
		ui->lineEdit_4->setText("");
	}
}

void Manager_Window::on_ShowTasks_clicked()
{
	ui->PDesc->setWordWrap(true);

	QString PID;
	querymodel = new QSqlQueryModel();
	ProjectID = ui->ProjectID_Show->text();
	querymodel->setQuery("SELECT T.Task_Id as 'Task ID', T.Name,T.Description,AD.First_Name + ' ' + AD.Last_Name as 'Employee Name', T.Deadline FROM Tasks as T inner join Employees as E on E.Task_Id = T.Task_Id inner join Users as U on U.User_Id = E.User_Id inner join Account_Details as AD on AD.Account_Id = U.Account_Id WHERE Project_Id = '" + ProjectID + "'");
	ui->tableTask->setModel(querymodel);

	querymodel1 = new QSqlQueryModel();
	querymodel1->setQuery("SELECT Name FROM Projects WHERE Project_Id = '" + ProjectID + "'");
	QString Name = querymodel1->data(querymodel1->index(0, 0)).toString();
	ui->Pname->setText(Name);

	querymodel1->setQuery("SELECT Description FROM Projects WHERE Project_Id = '" + ProjectID + "'");
	QString Desc = querymodel1->data(querymodel1->index(0, 0)).toString();
	ui->PDesc->setText(Desc);

	querymodel1->setQuery("SELECT Period FROM Projects WHERE Project_Id = '" + ProjectID + "'");
	QString Per = querymodel1->data(querymodel1->index(0, 0)).toString();
	ui->PPeriod->setText(Per);
}

void Manager_Window::on_AddTask_clicked()
{
	if (database.open())
	{
		QSqlQuery QInsertNewTask;
		QString Name = ui->lineNameTask->text();
		QString Description = ui->lineNameDesc->text();
		QString Deadline = ui->lineNameDead->text();
		QString EmpID = ui->lineEmpID->text();
		

		QInsertNewTask.prepare("INSERT INTO [Project_Manager].[dbo].[Tasks] ([Project_Id],[Name],[Description],[Deadline]) VALUES(:projid,:name,:description,:deadline)");
		QInsertNewTask.bindValue(":projid", ProjectID);
		QInsertNewTask.bindValue(":name", Name);
		QInsertNewTask.bindValue(":description", Description);
		QInsertNewTask.bindValue(":deadline", Deadline);
		QInsertNewTask.exec();

		querymodel = new QSqlQueryModel();
		querymodel->setQuery("SELECT * FROM Tasks WHERE Project_Id = '" + ProjectID + "' ORDER BY Task_Id DESC");
		QString TaskID = querymodel->data(querymodel1->index(0, 0)).toString();

		QInsertNewTask.prepare("UPDATE [Project_Manager].[dbo].[Employees] SET [Task_Id] = :taskid WHERE [Employee_Id] = :empid");
		QInsertNewTask.bindValue(":taskid", TaskID);
		QInsertNewTask.bindValue(":empid", EmpID);
		QInsertNewTask.exec();


		QPixmap pixmap("Resources/icon.png");
		QMessageBox msg1;
		msg1.setWindowTitle("SUCCESS!");
		msg1.setIcon(QMessageBox::Information);
		msg1.setWindowIcon(QIcon(pixmap));

		msg1.setText("Task added successfully!");
		msg1.exec();


		querymodel->setQuery("SELECT T.Task_Id as 'Task ID', T.Name,T.Description,AD.First_Name + ' ' + AD.Last_Name as 'Employee Name', T.Deadline FROM Tasks as T inner join Employees as E on E.Task_Id = T.Task_Id inner join Users as U on U.User_Id = E.User_Id inner join Account_Details as AD on AD.Account_Id = U.Account_Id WHERE Project_Id = '" + ProjectID + "'");
		ui->tableTask->setModel(querymodel);

		querymodel1 = new QSqlQueryModel();
		querymodel1->setQuery("SELECT AD.First_Name + ' ' + AD.Last_Name as 'Employee Name',E.Employee_Id  FROM Employees as E inner join Users as U on U.User_Id = E.User_Id inner join Account_Details as AD on AD.Account_Id = U.Account_Id WHERE E.Task_Id is NULL and U.Company_Id = '" + CompanyID + "'");
		ui->listTasks->setModel(querymodel1);

		ui->lineNameTask->setText("");
		ui->lineNameDesc->setText("");
		ui->lineNameDead->setText("dd-mm-yyyy");
		ui->lineEmpID->setText("");
	}
}

void Manager_Window::on_DeleteTask_clicked()
{
	if (database.open())
	{
		QString TaskID = ui->TaskID_Delete->text();
		QSqlQuery QDeleteTask;



		QDeleteTask.prepare("UPDATE [Project_Manager].[dbo].[Employees] SET [Task_Id] = NULL FROM [Project_Manager].[dbo].[Employees] as E inner join [Project_Manager].[dbo].[Tasks] as T on E.Task_Id = T.Task_Id  WHERE T.Task_Id = :taskid");
		QDeleteTask.bindValue(":taskid", TaskID);



		QPixmap pixmap("Resources/icon.png");
		QMessageBox msg;
		QMessageBox msg1;
		msg.setWindowTitle("???");
		msg.setText("Are you sure you want to delete this task?");
		msg.setIcon(QMessageBox::Question);
		msg.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
		msg.setWindowIcon(QIcon(pixmap));

		msg1.setWindowTitle("SUCCESS!");
		msg1.setIcon(QMessageBox::Information);
		msg1.setWindowIcon(QIcon(pixmap));
		msg1.setText("Task deleted successfully!");

		ret = msg.exec();
		switch (ret) {
		case QMessageBox::Yes:
			QDeleteTask.exec();
			

			QDeleteTask.prepare("DELETE Tasks WHERE Task_Id = :taskid");
			QDeleteTask.bindValue(":taskid", TaskID);
			QDeleteTask.exec();

			msg1.exec();

			break;
		}
		querymodel = new QSqlQueryModel();
		querymodel->setQuery("SELECT T.Task_Id as 'Task ID', T.Name,T.Description,AD.First_Name + ' ' + AD.Last_Name as 'Employee Name', T.Deadline FROM Tasks as T inner join Employees as E on E.Task_Id = T.Task_Id inner join Users as U on U.User_Id = E.User_Id inner join Account_Details as AD on AD.Account_Id = U.Account_Id WHERE Project_Id = '" + ProjectID + "'");
		ui->tableTask->setModel(querymodel);

		querymodel1 = new QSqlQueryModel();
		querymodel1->setQuery("SELECT AD.First_Name + ' ' + AD.Last_Name as 'Employee Name',E.Employee_Id  FROM Employees as E inner join Users as U on U.User_Id = E.User_Id inner join Account_Details as AD on AD.Account_Id = U.Account_Id WHERE E.Task_Id is NULL and U.Company_Id = '" + CompanyID + "'");
		ui->listTasks->setModel(querymodel1);
	}
}

void Manager_Window::on_Refresh_clicked()
{
	querymodel = new QSqlQueryModel();
	querymodel->setQuery("SELECT P.Project_Id, P.Name, P.Description, P.Period FROM Users as U inner join Managers as M on M.User_Id = U.User_Id inner join Projects as P on P.Manager_Id = M.Manager_Id where U.Credential_Id = '" + CID + "'");
	ui->tableProj->setModel(querymodel);

	querymodel1 = new QSqlQueryModel();
	querymodel1->setQuery("SELECT T.Task_Id as 'Task ID', T.Name,T.Description,AD.First_Name + ' ' + AD.Last_Name as 'Employee Name', T.Deadline FROM Tasks as T inner join Employees as E on E.Task_Id = T.Task_Id inner join Users as U on U.User_Id = E.User_Id inner join Account_Details as AD on AD.Account_Id = U.Account_Id WHERE Project_Id = '" + ProjectID + "'");
	ui->tableTask->setModel(querymodel1);

	querymodel2 = new QSqlQueryModel();
	querymodel2->setQuery("SELECT AD.First_Name + ' ' + AD.Last_Name as 'Employee Name',E.Employee_Id  FROM Employees as E inner join Users as U on U.User_Id = E.User_Id inner join Account_Details as AD on AD.Account_Id = U.Account_Id WHERE E.Task_Id is NULL and U.Company_Id = '" + CompanyID + "'");
	ui->listTasks->setModel(querymodel2);
}
